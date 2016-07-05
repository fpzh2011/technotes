# sqlite说明

目前这个版本是 3.13.0。
下载地址：https://sqlite.org/download.html

## insert优化的编译选项

在 sqlite3.c 增加：
```c
#define SQLITE_OMIT_LOAD_EXTENSION 1
#define SQLITE_THREADSAFE 0

```
通过sql语句执行`PRAGMA synchronous=OFF`。

SQLite的journal log机制，修改直接写到数据库文件、而将旧数据备份到journal log，很适合将旧数据全部清空、新数据重新导入的场景。写前日志（WAL）反而不一定适合（没有测试）。

## 数据类型

http://www.sqlite.org/datatype3.html
http://www.oschina.net/translate/data-types-in-sqlite-version-3?cmp

## SQL语言

https://sqlite.org/lang.html

## 建表 create table 

https://sqlite.org/lang_createtable.html

除了`WITHOUT ROWID`表，每一行都有一个64位带符号整数作为主键，即rowid。如果主键是`INTEGER`、且只含一列，这个主键列就会作为rowid的别名。注意，必须是`INTEGER`，INT之类的都不行。

### 插入数据　insert into

https://sqlite.org/lang_insert.html

### 查询 select 语句

https://www.sqlite.org/lang_select.html
http://www.tutorialspoint.com/sqlite/sqlite_limit_clause.htm
http://stackoverflow.com/questions/3325515/sqlite-limit-offset-query

### 删除表 drop table

https://sqlite.org/lang_droptable.html

## API接口

https://sqlite.org/cintro.html

## 样例程序

http://www.runoob.com/sqlite/sqlite-c-cpp.html
http://www.thegeekstuff.com/2012/09/sqlite-command-examples/
http://stackoverflow.com/questions/10850375/c-create-database-using-sqlite-for-insert-update
http://zetcode.com/db/sqlitec/

## 编译

### 编译方法

https://sqlite.org/howtocompile.html
http://stackoverflow.com/questions/10574933/compiling-of-sqlite3-in-c

### 编译选项

http://www.sqlite.org/compile.html

### 静态编译

如果直接进行静态编译，会提示：
```
sqlite3.o：在函数‘unixDlOpen’中：
sqlite3.c:(.text+0x104ee): 警告： Using 'dlopen' in statically linked applications requires at runtime the shared libraries from the glibc version used for linking
```
原因是sqlite3默认开启extension load，在Linux下用dlopen函数加载动态链接库。如果是静态链接，链接程序会提示，防止动态库与静态库版本不兼容。
可以在sqlite3.c第一行增加如下选项，禁止sqlite的扩展加载：
```c
#define SQLITE_OMIT_LOAD_EXTENSION 1
```

## 多线程支持

检验是否支持多线程：`sqlite3_threadsafe()`
https://www.sqlite.org/c3ref/threadsafe.html

## Command-Line Shell 常用命令

https://sqlite.org/cli.html

.tables 列出库表
.schema 查看库表结构

## insert 性能评估

使用预编译sql语句。

单线程插入1万行、每行3个64位整数。
* 不开启事务，40.897s。
* 开启事务，0.025s。

单线程开启事务，插入3000万行、每行3个64位整数，679M。
* 仅开启事务，48.482s。
* `#define SQLITE_THREADSAFE 0`，有效果，42.942s。
* `PRAGMA synchronous = OFF`有效果但不明显，需要46.879s。
* `PRAGMA journal_mode = OFF`效果不明显。
* `PRAGMA page_size`效果不明显。

参考：
https://sqlite.org/pragma.html
http://stackoverflow.com/questions/1711631/improve-insert-per-second-performance-of-sqlite
http://blog.quibb.org/2010/08/fast-bulk-inserts-into-sqlite/
http://codificar.com.br/blog/sqlite-optimization-faq/

## Java

http://www.runoob.com/sqlite/sqlite-java.html
http://www.tutorialspoint.com/sqlite/sqlite_java.htm
https://bitbucket.org/xerial/sqlite-jdbc
https://en.wikibooks.org/wiki/Java_JDBC_using_SQLite/Connecting
http://stackoverflow.com/questions/41233/java-and-sqlite
http://stackoverflow.com/questions/1525444/how-to-connect-sqlite-with-java
http://www.sqlitetutorial.net/sqlite-java/


# 疑问

无符号整数问题。转为64位，是否需要清零高32位？如果就使用32位，比较的时候是否有问题，是否影响逻辑？
http://stackoverflow.com/questions/17363094/sqlite-c-and-unsigned-integer-manipulation
http://sqlite.1065341.n5.nabble.com/How-to-create-a-data-of-unsigned-int-td52475.html

也可以考虑blob

