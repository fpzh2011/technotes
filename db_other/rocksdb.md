# rocksdb

https://github.com/facebook/rocksdb/releases/

## 依赖组建

### snappy

使用yum安装的snappy库进行读取时，读数据库时可能会报错：“Corruption: Snappy not supported or corrupted Snappy compressed block contents”。
编译构建snappy的过程（库文件安装到 /usr/local/lib/）：
```shell
sudo yum remove snappy snappy-devel
git clone git://github.com/google/snappy
cd snappy
./autogen.sh
./configure
make
sudo make install
```

## 安装

如果需要部署到生产环境，运行`make static_lib`，生成的 librocksdb.a 有225M。程序仅有put代码，静态链接后50M。
运行`make shared_lib`生成共享库，完成后运行`ldd librocksdb.so`检查动态链接库是否正常。必要时设置`export LD_LIBRARY_PATH=/usr/local/lib`。

https://github.com/facebook/rocksdb/blob/master/INSTALL.md

### RocksDBLite

如果要编译为精简的 RocksDBLite ，需要在Makefile开头增加`OPT=-DROCKSDB_LITE`，生成的 librocksdb.a 有85M。但是静态链接后的程序也有33M。

https://github.com/facebook/rocksdb/blob/master/ROCKSDB_LITE.md

### rocksdbjava

如果编译RocksDB自带的java包，运行`export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64 & make rocksdbjava`。生成的jar包和so共享库在目录`java/target/`下。
如果不设置JAVA_HOME，会报错找不到jni.h。

编译前，将Makefile的DEBUG_LEVEL改为0。否则，`db.close();`会报错，对应如下代码（或者将这行代码注释掉）：
```cpp
//db/column_family.cc
//ColumnFamilyData::~ColumnFamilyData()
assert(dummy_versions_->TEST_Next() == dummy_versions_);
```

https://github.com/facebook/rocksdb/wiki/RocksJava-Basics

## 写优化

https://github.com/facebook/rocksdb/wiki/RocksDB-FAQ  What's the fastest way to load data into RocksDB?
https://github.com/facebook/rocksdb/wiki/Creating-and-Ingesting-SST-files
https://github.com/facebook/rocksdb/wiki/RocksDB-Tuning-Guide
https://github.com/facebook/rocksdb/wiki/Basic-Operations
https://github.com/facebook/rocksdb/wiki/RocksDB-Basics

最快的方式是`PrepareForBulkLoad()`，可以参照这个函数中的选项设置，在调用该函数后，加大个别选项的值。
更快的方式是直接写SST文件再添加到DB。

环境：rhel6.8，磁盘，384G内存，40核

禁止WAL
disableDataSync
WriteBatch

Parallelism options
options.env->SetBackgroundThreads(num_threads, Env::Priority::HIGH);
options.env->SetBackgroundThreads(num_threads, Env::Priority::LOW);
max_background_compactions
max_background_flushes

其它可以考虑的：
Options::write_buffer_size 这2个参数似乎还变慢了
max_write_buffer_number
Universal Compaction
num_levels

## 操作参考资料

doc/index.html
https://github.com/facebook/rocksdb/wiki
https://github.com/facebook/rocksdb/tree/master/examples

## Java接口参考资料

https://github.com/facebook/rocksdb/wiki/RocksJava-Basics
java/src/test
java/samples
java/target/apidocs/ （编译之后）

## 限制

根据[RocksDB Basics](https://github.com/facebook/rocksdb/wiki/rocksdb-basics): There is no limit to the size of a key or a value. 
但是如果key/value的size过大，对性能可能有影响。
另可参考： https://github.com/facebook/rocksdb/issues/513

## 相关项目

[pika](https://github.com/Qihoo360/pika)，兼容redis的分布式存储，底层存储是rocksdb的变种。


## 常用工具

查询管理工具：ldb
https://github.com/facebook/rocksdb/wiki/Administration-and-Data-Access-Tool
`make ldb`


