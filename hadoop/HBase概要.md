# HBase 概要

[Apache HBase Reference Guide](http://hbase.apache.org/book.html)笔记

## 单节点安装

* 编辑`conf/hbase-env.sh`修改`JAVA_HOME`。
* 编辑`conf/hbase-site.xml`修改`hbase.rootdir`和`hbase.zookeeper.property.dataDir`，配置HBase和ZooKeeper存储数据的目录。不要创建HBase数据目录，否则系统会尝试migration。
* `bin/start-hbase.sh`启动HBase。单节点模式下，HMaster、HRegionServer、ZooKeeper都在一个JVM中，所以用`jps`查看只有一个`HMaster`进程。
* `./bin/hbase shell`连接到HBase。
* `help 'list'`查看命令帮助。
* `create 'test', 'cf'`在`cf`创建表。
* `list 'test'`列出HBase中的表，可以用正则表达式过滤表名。
* `describe 'test'`查看表结构。
* `put 'test', 'row1', 'cf:a', 'value1'`插入记录。
* `scan 'test'`扫描表中的数据。
* `get 'test', 'row1'`检索一行数据。
* `count 'test'`统计行数。
* `disable 'test'`删除或修改一个表，需要先`disable`它，再进行后续处理。也可以再`enable`。
* `drop 'test'`删除表。
* `exit`或`quit`退出Shell。
* `./bin/stop-hbase.sh`关闭HBase daemons。

在单机模式下使用hdfs：http://hbase.apache.org/book.html#standalone.over.hdfs

## Data Model

http://jimbojw.com/#understanding hbase
http://0b4af6cdc2f0c5998459-c0245c5c937c5dedcca3f1764ecc9b2f.r43.cf2.rackcdn.com/9353-login1210_khurana.pdf

it can be helpful to think of an HBase table as a **multi-dimensional map**.
A Bigtable is a sparse, distributed, persistent multidimensional sorted map.

HBase的[数据模型](http://hbase.apache.org/book.html#datamodel)
* table
* [column family](http://hbase.apache.org/book.html#columnfamily)
* column

### 命名空间

http://hbase.apache.org/book.html#_namespace

命名空间类似于关系数据库中数据库或用户的概念，是为多租户特性引入的，可以帮助实现以下特性：资源限额、安全、Region server groups等。

`create 'my_ns:my_table', 'fam'`，用冒号分隔的表名，冒号前面是命名空间，冒号后面是表名。如果不带冒号，就属于默认命名空间。

HBase有两个预定义的命名空间：hbase和default。

### Row

通过[row key](http://hbase.apache.org/book.html#_row)标识不同的行。row key没有类型，只是[字节数组](http://0b4af6cdc2f0c5998459-c0245c5c937c5dedcca3f1764ecc9b2f.r43.cf2.rackcdn.com/9353-login1210_khurana.pdf)。

row key[不能被修改](http://hbase.apache.org/book.html#changing.rowkeys)。

### Column Family

[column family](http://hbase.apache.org/book.html#columnfamily)必须提前定义，而column不必提前定义。Column qualifiers没有类型，可以是任意[字节数组](http://hbase.apache.org/book.html#columnfamily)。

每行的[column family](http://hbase.apache.org/book.html#columnfamily)都是一样的，虽然可以不实际存储数据。但每行的column可以有很大差异。
一个table一般不超过[几十个](http://hbase.apache.org/book.html#schema)column family。column family不能修改的太频繁（HBase权威指南，P17）。
一个column family可以有几百万column。一个column可以有多个版本（可指定最大版本数），每个版本是一个[cell](http://hbase.apache.org/book.html#_cells)，每个cell都有时间戳。
column family是schema的一部分，而column不是，[HBase并不存储column的元数据](http://hbase.apache.org/book.html#dm.column.metadata)。
比如一行是一个html文档，anchor是一个column family，文档中的每个超链接都是anchor下的column，如`anchor:cnn.com`。

HBase是面向列的存储（或column family），可以动态增加列。在Shell中用`get`查询一行时，各列分多行输出。

column family的column members[在一起存储](http://hbase.apache.org/book.html#columnfamily)。

### 数据模型操作

[数据模型操作](http://hbase.apache.org/book.html#_data_model_operations)
* put
* get
* scan
* delete

`delete`并不会物理删除，只是打`tombstones`标记，在major compactions时再清理。

上述操作返回的数据都是[有序](http://hbase.apache.org/book.html#dm.sort)的。

## Schema Design

http://hbase.apache.org/book.html#schema

修改ColumnFamily前需要[disableTable](http://hbase.apache.org/book.html#schema.creation)。

### RegionServer Sizing设计的经验法则

[RegionServer Sizing设计的经验法则](http://hbase.apache.org/book.html#table_schema_rules_of_thumb)
* region size在10~50GB。
* cell不要超过10MB，如果使用MOB不要超过50MB。否则将数据保存在HDFS，cell中保存HDFS路径。
* 每个表不要超过3个column families。最好只保留[一个column family](http://hbase.apache.org/book.html#number.of.cfs)。
* column family的名字尽可能短，最好只用一个字符。因为每个值都要保存column family名。

row key设计的一些方法：
* 避免单调增长
* salt
* hash
* reverse
另外可参考关于["Tall/Wide/Middle" Schema Design](http://hbase.apache.org/book.html#schema.smackdown)







## 非Java客户端

### C/C++

通常使用thrift

https://github.com/mapr/libhbase
https://github.com/hinaria/native-cpp-hbase-client
https://github.com/apache/hbase/tree/master/hbase-native-client






minor compaction， major compaction
m2obj如果有两个family，一个保存计数，一个保存实际的ip数据。查询计数是否会很慢？大路由器很少，也可以单独建一个表，或者保存到hdfs。不同的业务（上下游、转发计数、m2obj等），可以用不同的设计。
Hbase貌似不支持索引？或者只能自己建索引表？

mysql可以直接按C段保存到HBase。
如何查询部分列。
C++如何并发查询？









