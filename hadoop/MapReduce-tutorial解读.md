# MapReduce Tutorial解读

本文档是对[MapReduce Tutorial](http://hadoop.apache.org/docs/stable/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html)的总结与解析。
[Apache Hadoop (MapReduce) Internals](http://ercoppa.github.io/HadoopInternals/)也提供了MapReduce的详细图解。

系统环境：hadoop-2.8.0 | jdk1.8.0_121-b13 | centos7.3_64

## 例子

```java
//counts the number of occurrences of each word in a given input set

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class WordCount {

  public static class TokenizerMapper
       extends Mapper<Object, Text, Text, IntWritable>{

    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();

    public void map(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString());
      while (itr.hasMoreTokens()) {
        word.set(itr.nextToken());
        context.write(word, one);
      }
    }
  }

  public static class IntSumReducer
       extends Reducer<Text,IntWritable,Text,IntWritable> {
    private IntWritable result = new IntWritable();

    public void reduce(Text key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
      int sum = 0;
      for (IntWritable val : values) {
        sum += val.get();
      }
      result.set(sum);
      context.write(key, result);
    }
  }

  public static void main(String[] args) throws Exception {
    # 根据配置文件，确定是通过ResourceManager还是本地单节点执行。
    Configuration conf = new Configuration();
    Job job = Job.getInstance(conf, "word count");
    job.setJarByClass(WordCount.class);
    job.setMapperClass(TokenizerMapper.class);
    job.setCombinerClass(IntSumReducer.class);
    job.setReducerClass(IntSumReducer.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(IntWritable.class);
    //实际是将文件路径添加到 Job 的 Configuration 对象。
    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    //提交Job，并等待Job执行结束
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
```

## Overview

MapReduce在结构上是独立于HDFS的。从上面的WordCount的例子可以看到，它与HDFS只通过`addInputPath/setOutputPath`函数调用建立联系。
MapReduce框架中的Master是ResourceManager，担负调度、监控等管理职责；slave是NodeManager，执行具体的计算任务。通常DataNode和NodeManager是同一个节点。

MRAppMaster？？？？？？？？？？？？？？一个Job对应一个MRAppMaster？
The MapReduce framework consists of a single master ResourceManager, one slave NodeManager per cluster-node, and MRAppMaster per application (see YARN Architecture Guide).

### 用Java以外的语言运行MapReduce

虽然Hadoop是用Java编写的，也可以用其它语言实现MapReduce任务，有两种方式：
* Hadoop Streaming, 可用Shell、Python等实现MapReduce。
* Hadoop Pipes, a SWIG-compatible C++ API to implement MapReduce applications (non JNI based).

## Job提交

运行MapReduce时，通常会先创建一个`Configuration`对象，它会读取默认（或在命令行中指定的）配置文件，这样就会知道ResourceManager和NameNode的地址，也就知道如何提交Job、从哪里获取输入、输出保存到哪里。
`Job`对象表示MapReduce任务配置，是客户端与ResourceManager交互的接口。通过`Job`注册需要运行的MapReduce程序、输入/输出规范等。
在`Job.submit()`方法中可以看到，这里会通过`connect()`与ResourceManager建立连接，如果是本地单点运行，应该会创建一个虚拟的连接。

通过`job.waitForCompletion`追踪可以看到，任务提交实际是由`org.apache.hadoop.mapreduce.JobSubmitter`实现的，这不是一个`public`类，文档中看不到。
在`JobSubmitter.submitJobInternal()`的注释中可以看到，这个方法主要完成以下工作：
* Checking the input and output specifications of the job.
* Computing the InputSplit for the job.
* Setup the requisite accounting information for the DistributedCache of the job, if necessary.
* Copying the job's jar and configuration to the map-reduce system directory on the distributed file-system. 
* Submitting the job to the JobTracker and optionally monitoring it's status.

从上面可以看到，MapReduce的jar包通常是拷贝到HDFS上，NodeManager再从HDFS下载。

Job提交后，ResourceManager负责将程序、配置发布到NodeManager，调度并监控任务执行，重新执行失败的任务，向客户端提供状态、诊断信息。

命令`mapred job -history output.jhist`可以打印job的明细、失败信息等。

## Inputs and Outputs

`JobSubmitter`还有一个很重要的任务，就是计算`InputSplit`。

通过上面的分析，MapReduce框架已经知道从哪里读取数据、从哪里获取可执行程序、各个阶段调用哪些程序。
MapReduce是利用多个节点、分布式、并行执行的。MapReduce框架负责对输入数据进行分割。数据是保存在若干文件的多个block中的，一条记录可能跨越多个block、甚至多个DataNode；不同对象的存储格式也可能不一样。MapReduce框架通过`InputFormat`来解耦这种变化。

[InputFormat](http://hadoop.apache.org/docs/stable/api/org/apache/hadoop/mapreduce/InputFormat.html)抽象类只有两个方法。一个是`getSplits`，另一个是`createRecordReader`。

`getSplits`返回`List<InputSplit>`对象，每个[InputSplit](http://hadoop.apache.org/docs/stable/api/org/apache/hadoop/mapreduce/InputSplit.html)分配给一个[Mapper](http://hadoop.apache.org/docs/stable/api/org/apache/hadoop/mapreduce/Mapper.html)（参考`InputFormat`的说明）。
`InputSplit`是一个逻辑分割，物理文件并没有被真正的分割、复制。比如对于[FileSplit](http://hadoop.apache.org/docs/stable/api/org/apache/hadoop/mapreduce/lib/input/FileSplit.html)，用`<filepath, start, length>`等表示一个分割，即指定文件路径、文件起点偏移、分割长度。

`InputSplit`并没有作为输入直接提供给`Mapper`，因为数据记录可能跨越分割边界，不同的数据也有不同的解析格式。`InputFormat`的另一个方法`createRecordReader`返回一个[RecordReader](http://hadoop.apache.org/docs/stable/api/org/apache/hadoop/mapreduce/RecordReader.html)对象。`RecordReader`负责读取、解析`InputSplit`指定的数据，并处理记录跨越边界的问题。`RecordReader`将`InputSplit`指定的数据分解为key/value pairs，框架将单个key/value pair传递给`Mapper`的`map`方法。因为记录可能跨越边界，即使`Mapper`处理的数据就在本地，也可能会发生远程的读操作。

WordCount的例子中并没有显式指定`InputFormat`，如果通过配置参数、或在代码中显式指定，`Job`的父类`JobContextImpl`的`getInputFormatClass`方法会返回`TextInputFormat`类，这是框架默认的`InputFormat`（[参考](http://hadoop.apache.org/docs/stable/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html##Job_Input)）。[stackoverflow的这篇文章](https://stackoverflow.com/questions/14291170/how-does-hadoop-process-records-split-across-block-boundaries)剖析了`TextInputFormat`对跨越分割边界的行的处理机制。
下面的代码示例了默认的`InputFormat`：
```java
/*
这个例子显示，默认的 InputFormat 是 TextInputFormat ，默认值是在JobContextImpl.getInputFormatClass中写死的，
也可以在 core-site.xml 配置 mapreduce.job.inputformat.class 设置默认值。
InputFormat 是在 Job 创建时设置的，实际保存在 JobConf 对象中，在代码中可以手工修改 InputFormat 。
InputFormat 与 Mapper 无关，因为 Mapper 接收的是Split之后的数据。
javac -cp `bin/hadoop classpath` Test.java
java -cp .:`bin/hadoop classpath` Test
*/
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.mapreduce.Job;

public class Test {
	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		Job job = Job.getInstance(conf, "test");
		System.out.println(job.getInputFormatClass());
	}
}
```

## Mapper/Reducer

### 概要
MapReduce的用户接口处理的都是key/value pair。key/value类必须是serializable并实现`Writable`，key类必须实现`WritableComparable`。org.apache.hadoop.io包预定义了很多内置类型的 Writable 类。
MapReduce典型过程示例： `(input) <k1, v1> -> map -> <k2, v2> -> combine -> <k2, v2> -> reduce -> <k3, v3> (output)`

通常情况下，Job的输入、输出都保存到文件系统，所以不用太担心内存溢出问题。

### Mapper

MapReduce框架为每个`InputSplit`生成一个map task。

`Mapper`只有两个`public`方法：没有参数的默认构造函数；`run`方法。`Mapper`只通过`run`方法传入的`Context`参数与外界进行交互，包含获取输入数据。

在`Mapper.run()`方法中：
* 首先调用`Mapper.setup()`方法.
* 然后，通过`Context获取`InputSplit`中的每个key/value pair，并调用`Mapper.map()`方法，`map`方法一次处理一个key/value pair。
* 最后调用`Mapper.cleanup()`方法。
一个`Mapper`对象如果多次调用`run`方法，就可能处理多个`InputSplit`，具体要看框架实现。

`Mapper`的输出也是一个key/value pair集合。一个key的所有value会被框架排序、分组、并传递到同一个`Reducer`。用户可以通过设置`RawComparator`控制排序和分组，通过设置`Partitioner`控制key与`Reducer`的映射关系。
如果配置了combiner，`Mapper`的输出结果会在本地进行合并后，再传输给`Reducer`。
如果`Reducer`的个数为0，`Mapper`的输出结果会不经过排序、直接写到`Job`的输出路径。

`Mapper`的输出会暂存到本地文件系统，不会写到HDFS（[Map_Parameters](http://hadoop.apache.org/docs/stable/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html#Map_Parameters), [stackoverflow](https://stackoverflow.com/questions/8603435/in-hadoop-where-does-the-framework-save-the-output-of-the-map-task-in-a-normal-m)）。应用在`map`方法中可以用`Counter`报告统计信息。

以上可以参考Tutorial和[Mapper](http://hadoop.apache.org/docs/stable/api/org/apache/hadoop/mapreduce/Mapper.html)的文档。

### Reducer

`Reducer`将`Mapper`输出的中间key/value pairs，按key约减为一个更小的集合。`Reducer`个数通过`Job.setNumReduceTasks(int)`进行设置。

reduce主要分为以下三个阶段：
* shuffle。mappers的输出经过排序后，由框架通过http拷贝到reducer。key的排序、key与reducer的映射由`Job.setPartitionerClass()`决定。
* sort。同一个key的数据可能来自不同的mapper，框架需要对reducer收到的数据再进行排序。sort与shuffer是同时进行的。
* reduce。框架调用`reduce`方法对同一个key的数据进行合并后输出。输出不会进行排序，直接写到指定的文件系统。

`Reducer.reduce(KEYIN key, Iterable<VALUEIN> values, context)`方法接受的是一个key，以及key对应value的`Iterable`集合。
`MapReduce`中的group是指将`Mapper`输出的key/value pairs集合，按key进行分组，重新整合为`<key, Iterable>`集合。

在`Reducer.run()`方法中：
* 首先调用`Reducer.setup()`方法.
* 然后，通过`Context`逐个获取key，对每个key的数据调用`Reducer.reduce()`方法，`reduce`方法一次处理一个key及其对应的若干value。
* 最后调用`Reducer.cleanup()`方法。

应用在`reduce`方法中可以用`Counter`报告统计信息。

#### Secondary Sort

比如要统计某年的最高气温，Secondary Sort可以更高效的完成。
这里用到了一个组合key的概念，温度也作为key的一部分。但是`setPartitionerClass`中，只按年份确定将数据分配到哪个`Reducer`。
`setOutputValueGroupingComparator`设定在`Reducer`中按什么进行分组，这里也是按年份进行分组。
`setOutputKeyComparatorClass`设置key的比较器，先按年份进行比较，再按温度进行比较。`reduce`方法中取每个年份的第一个记录即可。
可以参考这个[资料](http://blog.csdn.net/amuseme_lu/article/details/6956171)，以及[Reducer](http://hadoop.apache.org/docs/stable/api/org/apache/hadoop/mapreduce/Reducer.html)文档中的例子。

### Task Execution & Environment

MapReduce是在独立的JVM进程中执行的。

2.8.0似乎没有`MRAppMaster`这个类。只是一个组件角色名称？
http://blog.csdn.net/lipeng_bigdata/article/details/51288673

