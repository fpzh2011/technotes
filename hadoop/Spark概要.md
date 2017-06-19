# Spark概要

[Spark快速大数据分析](https://book.douban.com/subject/26616244/)笔记
以Python作为代码例子。
```shell
# ipython P11
# 版本低于Spark2.0
# IPYTHON=1 ./bin/pyspark
# Spark 2.0+
PYSPARK_DRIVER_PYTHON=ipython ./bin/pyspark
```


待阅读的文档：
https://spark.apache.org/docs/latest/programming-guide.html
https://spark.apache.org/docs/latest/
https://spark.apache.org/docs/latest/configuration.html



## Spark的优势

相对于Hadoop，Spark有如下优势：
* 支持内存计算，速度更快。
* 支持批处理、迭代计算（persist）、交互式查询、流处理等多种计算模式。
* 组件多，开发友好。sql/stream/MLlib/graphx等。
* 提供Scala、Java、Python接口。
* 支持Hadoop、Cassandra等数据源。

弹性的一个含义是，如果出现故障，Spark在任何时候都可以自动重算需要的数据（P22脚注，P39）。
不应该把RDD看作存放着特定数据的数据集，而应视为构建数据的指令列表。（P27）从另一个角度看，RDD有点像SQL，只提供数据规格描述，具体计算由框架完成。


交互式查询。是指SQ？这一点是和内存计算相关吗？Spark貌似没有自己的存储，交互式是不是也依赖于存储？？？？
迭代计算，应用是指MLlib？
如何监控集群、任务执行情况？？？？

### pyspark

这篇关于[PySpark Internals](https://cwiki.apache.org/confluence/display/SPARK/PySpark+Internals)的文章描述了pyspark与JVM的一些细节，涉及py4j和pipe。
另可参考： https://www.slideshare.net/thegiivee/pysaprk

## Spark Core

Spark Core包含任务调度、内存管理、错误恢复、与存储系统交互等模块，以及对RDD的API定义（P2）。

Spark支持在各种集群管理器上运行，包括Hadoop YARN、Apache Mesos，以及Spark自带的一个简易调度器（P4）。

Spark支持多种存储系统，包括HDFS、本地文件、Amazon S3、Cassandra、Hive、HBase等。Spark支持任何实现了Hadoop接口的存储系统。
Spark支持的Hadoop输入格式包括文本文件、SequenceFile、Avro、Parquet等（P6）。

### 驱动器

每个Spark应用都由一个驱动器程序发起集群上的各种并行操作（P12）。驱动器程序包含应用的main函数。
驱动器程序通过一个`SparkContext`对象来访问Spark环境。这个对象代表对计算集群的一个连接。shell启动时已经自动创建了一个`SparkContext`对象，是一个叫作`sc`的变量。

驱动器程序一般要管理多个执行器（executor）节点。

在应用中连接集群（P15）：
```python
from pyspark import SparkConf, SparkContext
conf = SparkConf.setMaster('local').setAppName('My App')
sc = SparkContext(conf = conf)
sc.stop()
```

## RDD

A [Resilient Distributed Dataset](http://spark.apache.org/docs/latest/api/python/pyspark.html#pyspark.RDD) (RDD), the basic abstraction in Spark. Represents an immutable, partitioned collection of elements that can be operated on in parallel.
弹性分布式数据集（Resilient Distributed Dataset）。RDD是Spark对分布式数据和计算的基本抽象（P11），是**不可变的**分布式的元素集合（P21）。
在Spark中，对数据的所有操作不外乎创建RDD、转化已有RDD、调用RDD操作进行求值。Spark会自动将RDD中的数据分发到集群上，并将操作并行化执行。

每个RDD都被分为多个分区，这些分区运行在集群中的不同节点上。

RDD支持两种类型的操作：转化操作（transformation）和行动操作（action）。P21-22
转化操作会由一个RDD生成一个新的RDD；行动操作会对RDD计算出一个结果，并把结果返回到驱动程序中，或保存到存储系统。（P22）
简单的判断准则：转化操作返回的是RDD；行动操作返回的是其它数据类型。（P24）

Spark使用谱系图记录不同RDD之间的依赖关系。（P25）

Spark对RDD进行**惰性**计算，只有**第一次**在一个**行动**操作中用到时，才会真正计算RDD（P22）。
默认情况下，Spark的RDD会在**每次**对它们施加**行动**操作时**重新**计算。如果想在多个行动操作中**重用**RDD，可以使用`RDD.persist()`让Spark把这个RDD缓存下来。

### 创建RDD的方式

```python
# 读取一个外部数据集（P21）
# sc.textFile读取操作也是惰性的，也可能被多次执行（P27）。
sc.textFile()
# 在驱动器程序中对一个集合parallelize（P23）
# 注意：这会将整个数据集先放在一台机器的内存中
lines = sc.parallelize(['pandas', 'test'])
```

### PairRDD

PairRDD的创建、接口

## 普通RDD的转换操作（transform）

### filter(f)

Return a new RDD containing only the elements that satisfy a predicate.
`filter`接收一个函数f，把这个函数f用于RDD中的每个元素x，若`f(x) == True`则将`x`放入新的结果RDD。（P31）

`f(x)`形式的调用必须是合法的。即，f至少有一个参数；若有多个参数，其它参数必须有默认值。
就目前的实现看，`f`可以不返回`bool`值，可以是列表、字符串、元组，只要`if f(x):`合法即可。

### map相关转换操作

#### map(f, preservesPartitioning=False)

Return a new RDD by applying a function to each element of this RDD.
`map`接收一个函数f作为参数，把这个函数f用于RDD中的每个元素，将函数f的返回结果作为结果RDD中对应元素的值，f的返回值类型不需要和输入类型一致。（P30/31）

`f(x)`形式的调用必须是合法的。即，f至少有一个参数；若有多个参数，其它参数必须有默认值。

结果RDD中的元素个数，与原RDD中的元素个数相同。考虑两种特殊情况：
* 函数f没有显示返回值。Python会设置返回值为`None`。如果原RDD有3元素，`map`后的结果RDD就有3个`None`。
* 函数返回多个值，Python会自动组装为一个元组。函数f返回列表或元组，结果RDD会将列表或元组视为一个元素。可以和`flatMap`对比一下。

```python
rdd = sc.parallelize(["b", "a", "c"])
rdd.map(lambda x: (x, 1)).collect()

# 没有返回值，结果RDD的元素都是None
def f1(x):
	pass

rdd.map(f1).collect()

# 返回多个值，RDD的元素是元组
def f2(x):
	return x, 1

rdd.map(f2).collect()

# 返回列表，RDD的元素是列表
def f3(x):
	return [x, '1']

rdd.map(f3).collect()
```

#### flatMap(f, preservesPartitioning=False)

Return a new RDD by first applying a function to all elements of this RDD, and then flattening the results.
`flatMap`和`map`类似，只是对每个输入元素生成多个输出元素（P31）。`flatMap`接收一个函数f作为参数。f被应用到输入RDD的每个元素上，并返回一个支持迭代器协议的对象，如列表、元组、字符串、字典等。迭代器的每个元素都被放入新的结果RDD。
`flatMap`的典型场景如文本分词（P32）。`flat`可以理解为将一个很长的元素`拍扁`为一系列短元素。

`f(x)`形式的调用必须是合法的。即，f至少有一个参数；若有多个参数，其它参数必须有默认值。
函数f的返回值类型不必与输入值类型一致，但返回值类型必须支持迭代器协议。

```python
rdd = sc.parallelize([2, 3, 4])
rdd.flatMap(lambda x: range(1, x)).collect()

def f(x):
	return 'ab'

rdd.flatMap(f).collect()
```

### 伪集合操作

#### cartesian(other)

Return the Cartesian product of this RDD and another one, that is, the RDD of all pairs of elements (a, b) where a is in self and b is in other.
`cartesian`操作返回两个RDD的笛卡尔积。

```python
rdd = sc.parallelize([1, 1, 2])
rdd.cartesian(rdd).collect()
```

#### distinct(numPartitions=None)

Return a new RDD containing the distinct elements in this RDD.
`distinct()`生成一个只包含不同元素的新RDD。（P32）
**注意**：`distinct()`的操作开销很大，因为它需要将所有数据通过网络进行shuffle。

#### intersection(other)

Return the intersection of this RDD and another one. The output will not contain any duplicate elements, even if the input RDDs did.
This method performs a shuffle internally.
`intersection`返回两个RDD中都有的元素，合并所有重复的元素。（P33）
**注意**：`intersection()`的操作开销很大，因为它需要将所有数据通过网络进行shuffle。

#### subtract(other, numPartitions=None)

Return each value in self that is not contained in other.
`rdd3 = rdd1.subtract(rdd2)`返回新的rdd3，rdd3中的元素在rdd1中存在，而不在rdd2中存在。（P33）
`subtract()`不会对重复数据进行合并。

#### union(other)

Return the union of this RDD and another one.
`rdd3 = rdd1.union(rdd2)`只接收一个RDD参数，返回一个包含两个RDD中所有元素的新RDD。`union`操作也会包含两个RDD中的重复数据，实际执行的是`union all`操作。（P33）

### 排序

`sortBy(keyfunc, ascending=True, numPartitions=None)`可以对RDD的元素进行排序。参数keyfunc是一个函数，接收RDD的元素作为参数，根据keyfunc返回值进行排序。

```python
x = sc.parallelize([1, 3, 0, 2])
print x.collect()
print x.sortBy(lambda x: x).collect()
print x.sortBy(lambda x: -x).collect()
```

## 普通RDD的行动操作（action）

### reduce(f)

Reduces the elements of this RDD using the specified commutative and associative binary operator. Currently reduces partitions locally.
`reduct(f)`接收一个函数f作为参数，`f(x,y)`接收两个参数，返回值其实可以和输入值的类型不一样（这是在单机Python Shell试验的情况，也许在分布式环境下，在JVM中运行时，必须要求类型是一致的，包括下面的`fold`和`aggregate`）。
伪码如下：
```python
if RDD只有一个元素x:
	return x
选择RDD中的两个元素x, y
z = f(x, y)
while RDD还有元素:
	获取新元素x
	z = f(z, x)
return z
```

示例：
```python
rdd = sc.parallelize([1, 2])
# 输入整数，返回字符串
rdd.reduce(lambda x, y: str(x) + str(y))

rdd = sc.parallelize([1])
# 这时返回的是整数1
rdd.reduce(lambda x, y: str(x) + str(y))
```

### fold(zeroValue, op)

Aggregate the elements of each partition, and then the results for all the partitions, using a given associative function and a neutral “zero value.”
The function op(t1, t2) is allowed to modify t1 and return it as its result value to avoid object allocation; however, it should not modify t2.
`fold`和`reduce`类似。`fold`的第2个参数op也是一个接收两个参数的函数，返回对两个函数归约的结果。`fold`的第1个参数zeroValue是每个**分区**进行归约的初始值（P35）。最后，调用op对各个分区的结果进行归约并将最终结果返回。

`op(t1, t2)`允许对第一个参数t1进行修改并返回t1，以减少对象创建。但是不允许修改第2个参数t2。
怎么理解这个约束呢？设想我们要将一个分区的所有值拼接为一个列表。Spark会将zeroValue作为第一个参数传入函数op，并始终将op的返回值作为第一个参数再传入op。而如果第2个参数（如整数）不支持op内的操作，比如`append`，就会报错。参考如下的例子：
```python
# 如果是多核机器，rdd的元素个数不要设置的太小，Spark可能会根据核的个数确定分区个数。
# 比如4核机器，rdd元素个数不超过4，可能每个元素一个分区。参考P54、P49
# rdd元素数量至少为cpu核数的2倍，观察效果较好
rdd = sc.parallelize(range(0,32))

# 将分区内的元素拼接为列表
# 如果修改y，运行时会报错
def f(x, y):
	if x == []:
		# 标记分区个数
		print '==========='
	x.append(y)
	return x

print rdd.fold([], f)
```

### aggregate(zeroValue, seqOp, combOp)

Aggregate the elements of each partition, and then the results for all the partitions, using a given combine functions and a neutral “zero value.”
The functions op(t1, t2) is allowed to modify t1 and return it as its result value to avoid object allocation; however, it should not modify t2.
The first function (seqOp) can return a different result type, U, than the type of this RDD. Thus, we need one operation for merging a T into an U and one operation for merging two U.

可以这样说，`fold`是`reduce`的升级版，`aggregate`是fold的升级版。
虽然理论上讲，`fold`和`reduce`的函数参数op，其输入参数类型与返回值类型可以不一致，但通常情况下使用相同的类型会更安全可靠。而`aggregate`可以安全可靠的实现从类型A到类型B的归约。
`aggregate`的第2个参数seqOp是一个函数，其输入参数类型与返回值类型可以不一样。对每个分区的数据用seqOp进行归约，以zeroValue作为初始值。
`aggregate`的第3个参数combOp也是一个函数，其输入参数类型与返回值类型通常是一致的。各分区的归约结果用combOp进行归约，同样以zeroValue作为初始值。
combOp归约的结果作为`aggregate`的返回值，其类型通常与zeroValue一致。
`aggregate`的典型场景如求平均值，需要将一个分区的数值归约为元组`(sum, count)`。（P35）

类似于`fold`，`op(t1, t2)`允许对第一个参数t1进行修改并返回t1，以减少对象创建。但是不允许修改第2个参数t2。

```python
# rdd元素数量至少为cpu核数的2倍，观察效果较好
rdd = sc.parallelize(range(0,32))

def seqOp(x, y):
	if x == []:
		# 标记分区个数
		print '==========='
	x.append(y)
	return x

def combOp(x, y):
	if x == []:
		# 这里可以显示，combOp也是从zeroValue开始归约
		print '***********'
	x = x + y
	return x

print rdd.aggregate([], seqOp, combOp)
```

### foreach(f)

对RDD中的每个元素运行函数f，比如打印。[foreach通常用于更新Accumulator。](https://spark.apache.org/docs/latest/programming-guide.html)
[如果对RDD执行foreach，只会在Executor端有效，而并不是Driver端。](http://lxw1234.com/archives/2015/07/399.htm)
```python
# 打印每个元素
def f(x):
	print x

sc.parallelize([1, 2, 3, 4, 5]).foreach(f)

# “修改”列表元素无效。
def f2(x):
	print x.__class__.__name__
	x[0] = 0

rdd = sc.parallelize([[1], [2], [3], [4], [5]])
rdd.foreach(f2)
rdd.collect()

# RDD返回的元素似乎是复制品，而不是引用。这也保证了RDD是不可变的。
# 这也从一个角度说明：不应该把RDD看作存放着特定数据的数据集，而应视为构建数据的指令列表。（P27）
# RDD通常不会存储，即使persist后可能也只是缓存部分数据在内存。
rdd.first() is rdd.first()
rdd.first() == rdd.first()
```

### 其它行动操作

P36-37
* collect 将RDD全部数据加载到驱动程序所在进程内存。
* count 返回RDD的元素个数。
* countByValue 以字典形式返回RDD中各值的计数。
* first 返回RDD第一个元素。
* take(n) 返回前n个元素。Spark先扫描一个分区，根据这个分区的数据估计需要几个分区才能得到n个元素。对元素顺序不能有预期。
* takeOrdered(n, key=None) （按kye指定的升序）返回前n个元素。
* top(n, key=None) （按kye指定的降序）返回前n个元素。
* takeSample(withReplacement, n, seed=None) 采样RDD的n个元素并返回。withReplacement指是否可放回。

## RDD持久化（缓存）

如果不做持久化，每次调用行动操作，都会重新计算RDD。（P22）
详细资料可以参考[Spark Programming Guide](https://spark.apache.org/docs/latest/programming-guide.html#rdd-persistence)

### persist

Set this RDD’s storage level to persist its values across operations after the first time it is computed.
`rdd.persist()`设置rdd的存储级别（默认是`MEMORY_ONLY`），但persist操作本身不会触发强制求值并持久化（P40），只有第一次调用行动操作才会触发求值并持久化。

当我们让Spark持久化存储一个RDD时，计算出RDD的节点会分别保存它们所求出的分区数据。如果一个有持久化数据的节点发生故障，Spark会在需要用到缓存数据时重算丢失的数据分区。也可以把持久化数据备份到多个节点。

存储级别可以参考P39或[StorageLevel](http://spark.apache.org/docs/latest/api/python/pyspark.html#pyspark.StorageLevel)的文档。

如果要缓存的数据太多，内存中放不下，Spark会自动利用“最近最少使用（LRU）”的缓存策略把最老的分区从内存中移除。（P40）
对于仅把数据存放在内存中的缓存级别，下一次要用到已经被移除的分区时，这些分区就需要**重新计算**。
但是对于使用内存与磁盘的缓存级别的分区来说，被移除的分区都会被写入磁盘。

### cache()

Persist this RDD with the default storage level (MEMORY_ONLY).
标记RDD为默认存储级别，即MEMORY_ONLY。

### unpersist

Mark the RDD as non-persistent, and remove all blocks for it from memory and disk.
`rdd.unpersist`可以手动把持久化的RDD从缓存中移除。

## PairRDD

PairRDD的元素是`<key, value>`类型。Spark为PairRDD提供了一些专有操作。
Java和Scala中有单独的PairRDD类型。Python中RDD涵盖了PairRDD，只是有些操作只适用于PairRDD，如果RDD对象调用这些操作会报错。

### 创建PairRDD

很多存储`<key, value>`的数据格式会在读取时直接返回PairRDD。（P42）

```python
rdd = sc.textFile('LICENSE')
# 将一个普通RDD转为PairRDD，可以用map函数，传递的函数需要返回一个二元元组（P42）
pairs = rdd.map(lambda x: ('' if not x else x.split(' ')[0], x))
pairs.take(10)

# 用字典创建PairRDD
x = sc.parallelize([("a", ["apple", "banana", "lemon"]), ("b", ["grapes"])])
x.collect()

# 用二元元组列表创建
rdd = sc.parallelize([("a", 1), ("b", 1), ("a", 1)])

# 用groupBy函数将普通RDD转为PairRDD
# groupBy的参数f也是一个函数。f接收RDD的元素作为参数，这个参数也是新的PairRDD的元素的value，f的返回值作为新的PairRDD的key。
rdd = sc.parallelize([1, 1, 2, 3, 5, 8])
result = rdd.groupBy(lambda x: x % 2).collect()
[(x, sorted(y)) for (x, y) in result]
```

## PairRDD的转化操作

标准RDD上的转化操作都可用于PairRDD（P42），只是传递的函数应操作二元元组而不是独立的元素（P43）。

### map操作

很多时候，我们只想操作PairRDD的value部分，这时操作二元元组就比较麻烦。Spark提供了mapValues等函数方便这种操作（P44）。

#### mapValues(f)

Pass each value in the key-value pair RDD through a map function without changing the keys; this also retains the original RDD’s partitioning.
`mapValues`的唯一参数f是一个函数。函数f接收一个参数，即PairRDD中一个元素（key/value）的value，key与f的返回值构建二元元组，作为新的结果PairRDD的元素。（P44）

```python
x = sc.parallelize([("a", ["apple", "banana", "lemon"]), ("b", ["grapes"])])
# 计算每个key的value个数。lambda的参数x，是二元元组中的第2个元素
x.mapValues(lambda x: len(x)).collect()
```

#### flatMapValues(f)

Pass each value in the key-value pair RDD through a flatMap function without changing the keys; this also retains the original RDD’s partitioning.
`flatMapValues`的唯一参数f是一个函数。函数f接收一个参数，即PairRDD中一个元素（key/value）的value，并返回一个支持迭代器协议的对象。

```python
# 将value处理为一个迭代器对象，将一个长value拍扁为多个v，从而由一条key/value生成多个key/v
x = sc.parallelize([("a", ["x", "y", "z"]), ("b", ["p", "r"])])
x.flatMapValues(lambda x: x).collect()
```

### 聚合操作（reduce）

在普通RDD中，reduce/fold/aggregate是行动操作。而在PairRDD中，有相对应的、针对key的转化操作，可以对相对key的value进行组合/聚合。这些操作返回RDD，因此它们是转化操作而不是行动操作（P45）

#### reduceByKey(func, numPartitions=None, partitionFunc=<function portable_hash>)

Merge the values for each key using an associative and commutative reduce function.
This will also perform the merging locally on each mapper before sending results to a reducer, similarly to a “combiner” in MapReduce.
Output will be partitioned with numPartitions partitions, or the default parallelism level if numPartitions is not specified. Default partitioner is hash-partition.

PairRDD可能有多个key值，每个key值可能有多个value。顾名思义，`reduceByKey`就是对每个key的多个value进行`reduce`操作，key和`reduce`的结果构建二元元组作为新的结果PairRDD的元素。（P45）
和普通RDD的`reduce`类似，`reduceByKey`的第一个参数func是一个函数，它接收两个参数（参数来自PairRDD的value），返回这两个参数值归约的结果。
```python
from operator import add
rdd = sc.parallelize([("a", 1), ("b", 1), ("a", 2)])
rdd.reduceByKey(add).collect()
```

普通RDD的`reduce`动作操作，而PairRDD的`reduceByKey`是转化操作。这是因为`reduceByKey`的结果通常会再进行其它后续计算。

`reduceByKey`可以指定结果PairRDD的分区方式，新的分区方式和原PairRDD可能不一样，所以新的结果PairRDD的数据可能会重新分配到不同的节点。

上述官方文档的第2句话涉及`locally combine`，这个该怎么理解呢？
Spark可以确保同一个key的数据出现在同一个节点上（P53）。但是一个key的数据可能很多，从而在一个节点上跨多个分区。Spark应该是以分区为逻辑单位对数据进行并行处理的，每个任务处理一个分区。因为一个key的数据可能跨分区，所以一个key的数据可能被多个任务并行处理，需要对多个任务的处理结果再进行归约、合并，才能得到key的唯一结果。`reduceByKey`没有指定对任务结果的归约函数，应该就是用func函数进行归约。
上述分析依据如下：
* 分区是RDD的[logical chunk](https://jaceklaskowski.gitbooks.io/mastering-apache-spark/content/spark-rdd-partitions.html)。
* 在[techmagie](https://techmagie.wordpress.com/2015/12/19/understanding-spark-partitioning/)的这篇资料中提到分区与任务的关系：`Number of Tasks on per stage basis = Number of partitions`。
* 分区数决定了在RDD上执行操作时的并行度（P49）。前面对`fold`和`aggregate`的分析也可以提供证据。这里也可以进一步理解，为什么`fold`和`aggregate`的初始值应用于每个分区的第一次计算，因为是以分区为单位执行任务。

#### foldByKey(zeroValue, func, numPartitions=None, partitionFunc=<function portable_hash>)

Merge the values for each key using an associative function “func” and a neutral “zeroValue” which may be added to the result an arbitrary number of times, and must not change the result (e.g., 0 for addition, or 1 for multiplication.).
和`fold`类似，`foldByKey`是在`reduceByKey`的基础上，为每个key和每个分区指定一个初始值zeroValue。（P45）

#### combineByKey(createCombiner, mergeValue, mergeCombiners, numPartitions=None, partitionFunc=<function portable_hash>)

Generic function to combine the elements for each key using a custom set of aggregation functions.
Turns an RDD[(K, V)] into a result of type RDD[(K, C)], for a “combined type” C.

Users provide three functions:
* createCombiner, which turns a V into a C (e.g., creates a one-element list)
* mergeValue, to merge a V into a C (e.g., adds it to the end of a list)
* mergeCombiners, to combine two C’s into a single one.

In addition, users can control the partitioning of the output RDD.
Note V and C can be different – for example, one might group an RDD of type (Int, Int) into an RDD of type (Int, List[Int]).

`combineByKey`类似于`aggregate(zeroValue, seqOp, combOp)`，可以转换PairRDD的value的数据类型（key的类型不变）。
各参数的作用也是对应的（P47）：
* `createCombiner`对应于`zeroValue`。zeroValue是一个值对象。而createCombiner是一个函数，在处理每个分区、第一次遇到一个key时，提供这个key的value提供初始值。createCombiner的输入参数是key的value，返回一个根据value创建的初始值对象。Combiner可以理解为组合器、转换器。
* `mergeValue`对应于`seqOp`。在处理每个分区、不是第一次遇到一个key时，将key的value与之前`createCombiner`提供的初始值进行归约、合并。可以是add加和，也可以是添加到列表。`mergeValue`接收2个参数，第一个参数与`createCombiner`返回的初始值对象类型一致，第2个参数是后遇到的key的value；返回值的类型与`createCombiner`一致，用于后续归约。
* `mergeCombiners`对应于`combOp`。将同一个key分布在多个分区的结果进行合并。合并的逻辑类似`reduce`而不是`fold`，因为没有初始值（`createCombiner`的初始值只在分区内使用）。`mergeCombiners`接收两个参数，参数和返回值类型与`createCombiner`返回的初始值对象类型一致。

```python
rdd = sc.parallelize(zip(([1] * 4 + [2] * 4 + [3] * 4 + [4] * 4)*4, range(0,32)))
print 'partition num: ', rdd.getNumPartitions()

def createCombiner(x):
	# 每个分区、每个key打印一次
	print 'createCombiner xxxxxxxxxxxxxxxxxx\n'
	return []

def mergeValue(x, y):
	if x == []:
		# 标记分区个数。每个分区、每个key打印一次。
		print 'mergeValue ==========='
	x.append(y)
	return x

def mergeCombiners(x, y):
	if x == []:
		# 与reduce的combOp不同，mergeCombiners不是从zeroValue开始归约，
		# 因为combineByKey通常有多个key，不存在单一的zeroValue。所以下面的*不会输出
		print 'mergeCombiners ***********'
	# 验证mergeCombiners的执行
	# 因为没有同步，所以下面的输出可能有多行。
	if not hasattr(mergeCombiners, 'accessed'):
		mergeCombiners.accessed = True
		print 'mergeCombiners accessed >>>>>>>>>'
	x = x + y
	return x

print rdd.combineByKey(createCombiner, mergeValue, mergeCombiners).collect()
```

#### aggregateByKey(zeroValue, seqFunc, combFunc, numPartitions=None, partitionFunc=<function portable_hash>)

类似于`combineByKey`，只是用对象zeroValue替换函数createCombiner，对每个key使用同样的初始值。参考[源代码](https://github.com/apache/spark/blob/master/python/pyspark/rdd.py)。

### 数据分组

#### groupByKey(numPartitions=None, partitionFunc=<function portable_hash>)

Group the values for each key in the RDD into a single sequence. Hash-partitions the resulting RDD with numPartitions partitions.
Note If you are grouping in order to perform an aggregation (such as a sum or average) over each key, using reduceByKey or aggregateByKey will provide much better performance.

如果原来的PairRDD的元素是`<K,V>`类型，`groupByKey`返回的PairRDD的元素是`<K,Iterable<V>>`类型。（P49）
`groupByKey`将一个key的所有value拼接为一个Iterable对象，将key和Iterable对象形成二元元组作为新的结果PairRDD的元素。
如果`groupByKey`之后还有再聚合，不如直接用`reduceByKey`或`aggregateByKey`更为高效。（P50）

```python
rdd = sc.parallelize([("a", 1), ("b", 1), ("a", 2)])
rdd.groupByKey().mapValues(list).collect()
```

#### cogroup(other, numPartitions=None)

For each key k in self or other, return a resulting RDD that contains a tuple with the list of values for that key in self as well as other.
对两个类型均为K而值的类型分别为V和W的PairRDD进行`cogroup`操作时，得到的结果PairRDD的类型为`[(K, (Iterable[V], Iterable[W]))]`，两个迭代对象分别是对应key在两个PairRDD的value的集合。（P50）
`x.cogroup(y)`和`y.cogroup(x)`的结果是类似的，类似全外连接，二者都包含两个PairRDD中的所有key和value。如果某个PairRDD中不包含某个key，则结果PairRDD中对应的迭代对象为空。
实际上，`cogroup`是`join`操作的构成要素。

```python
x = sc.parallelize([("a", 1), ("b", 4)])
y = sc.parallelize([("a", 2)])
# x.cogroup(y).collect()
# y.cogroup(x).collect()
[(x, tuple(map(list, y))) for x, y in sorted(list(y.cogroup(x).collect()))]
```

#### groupWith(other, *others)

Alias for cogroup but with support for multiple RDDs.
类似于`cogroup`，只是可以同时接收多个PairRDD参数。

### join连接

join类似于数据库，将两个PairRDD拼接为一个PairRDD，结果PairRDD中的key来自原PairRDD，value是一个二元元组，二元元组的元素分别来自两个原PairRDD。（P50）
外连接中如果存在空值，在Python中用None表示。（P51）
join操作包括以下几种：
* fullOuterJoin 全外连接、交叉连接
* join 内连接
* leftOuterJoin 左外连接
* rightOuterJoin 右外连接

```python
x = sc.parallelize([("a", 1), ("b", 4)])
y = sc.parallelize([("a", 2), ("a", 3)])
x.join(y).collect()
# [('a', (1, 2)), ('a', (1, 3))]
```

### 排序

`sortByKey(ascending=True, numPartitions=None, keyfunc)`可以按PairRDD的key进行排序并返回排序后的新PairRDD。参数keyfunc是一个函数，接收PairRDD的key作为参数，根据keyfunc返回值进行排序。

## PairRDD的行动操作

普通RDD的所有行动操作都可以用在PairRDD上。PairRDD特有的行动操作包括（P52）：
* countByKey 统计每个key的元素个数
* collectAsMap 以字典的形式返回PairRDD的数据
* lookup 返回给定key的所有值

## 数据分区

为分布式数据集选择**正确的分区方式**和为本地数据集选择合适的数据结构很相似（P41）。Spark程序可以通过控制RDD分区方式来减少通信开销，提升整体性能。（P52）

分区并不是对所有应用都有好处的——比如，如果给定RDD只需要被扫描一次，就没有必要预先进行分区处理。只有当数据集多次在诸如连接这种基于键的操作中使用时，分区才会有帮助。

Spark没有给出显式控制每个key具体落在哪个节点的方法（部分原因是Spark会重算因故障丢失的分区数据，也可能会将数据复制到多个节点）。但**Spark可以确保同一组的key出现在同一个节点上**（P53），虽然一个key的数据可能跨多个分区。
RDD默认是不分区的。如果没有显式分区，一个key的数据可能会分布在多个节点，从第53页最后一段join的例子、第56页最后一段reduceByKey的分析也可以印证这一点。

`partitionBy(numPartitions, partitionFunc)`是一个转化操作，可以设置新的分区方式并返回分区后的新PairRDD（P54/55）。其第1个参数指定分区个数，第2个参数可以指定分区方式（P60）。
这个哈希函数会被与其它RDD的分区函数区分开。如果想对多个RDD使用相同的分区方式，应该使用同一个函数对象，比如一个全局函数，而不是为每个RDD创建一个新的函数对象。（P61）

`sortByKey`生成范围分区的PairRDD，`groupByKey`生成哈希分区的PairRDD。而`map`这样的操作会导致新的RDD失去父RDD的分区信息（P55）

Python API没有提供查询分区方式的方法。（P56）

从分区中获益的至少包括以下操作（P56）：
* cogroup
* groupWith
* join相关操作
* groupByKey
* reduceByKey

### 影响分区的操作

Spark内部知道各操作会如何影响分区方式，并将*会对数据进行分区的操作*的*结果RDD*自动设置为对应的分区器。

下列操作会为生成的*结果RDD*设好分区方式（P57）：
* cogroup
* groupByKey
* reduceByKey
* combineByKey
* partitionBy
* sortBy
* sortByKey
* mapValues/flatMapValues/filter，如果父RDD有分区方式。

对于二元操作，结果RDD的分区方式取决于父RDD的分区方式（P57）。默认情况下采用哈希分区，分区的数量和操作的并行度一样。如果其中一个父RDD已经设置过分区方式，结果RDD会采用这种分区方式。如果两个父RDD都设置过分区方式，结果RDD会采用第一个父RDD的分区方式。

### 分区与并行度调优

每个RDD都有**固定数目**的分区，Spark始终尝试根据集群大大小推断出一个有意义的默认值，也可以人为指定。分区数决定了在RDD上执行操作时的并行度。
以下函数可以强制RDD重新分区：
* `repartition(numPartitions)` 这会返回一个新RDD，通常会shuffle to redistribute data。如果只是减少分区数量，用`coalesce`可以减少shuffle。
* `coalesce()` Return a new RDD that is reduced into numPartitions partitions.

Python中可以用`rdd.getNumPartitions()`获取分区数量。

另参考`reduceByKey`的分析。


