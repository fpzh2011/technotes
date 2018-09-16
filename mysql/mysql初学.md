mysql初学
=========

# 口令

yum安装时需要输入口令，如 `rootpwd`

# 修改字符集

[参考资料](http://blog.chinaunix.net/uid-545411-id-2385599.html)

	set names utf8;
	
创建配置文件 `/etc/mysql/conf.d/zjh.cnf` （rhel7-mariadb是 `/etc/my.cnf.d/`，Ubuntu16是`/etc/mysql/mysql.conf.d/`，mysql5.7则要使用include指令）：

```
[mysqld]
character-set-server=utf8
lower_case_table_names=1
sql-mode=""

[client]
default-character-set=utf8

[mysql]
default-character-set=utf8
```

显示字符集设置（如果提示Table 'performance_schema.session_variables' doesn't exist，停止mysql数据库，运行`mysql_upgrade -u root -p --force`，再重启数据库）

	show variables like 'character%';

另外，可以参考 CH 5.1.4 Server System Variables；以及 CH 4.2.6 Using Option Files，这里列出了配置文件的地址，以及基本配置方法。

## Unicode问题

https://stackoverflow.com/questions/13653712/
https://dev.mysql.com/doc/refman/5.5/en/charset-unicode-conversion.html
https://dev.mysql.com/doc/refman/5.7/en/charset-connection.html
mysql对unicode仅支持basic multilingual plane。有些同步的表中包含表情字符，比如 😊 ，不在basic multilingual plane中。
解决办法是设置字符集为 utf8mb4 ，支持4字节编码。尤其是要修改数据库（以及table）和客户端的字符集。如果数据库已经是utf8mb4，Server的字符集可以是utf8。
参考的配置如下：
```
[mysqld]
character-set-server=utf8
[client]
default-character-set=utf8mb4
```
对于JDBC，如果服务端没有设置，还需要执行`SET NAMES utf8mb4`。因为JDBC驱动没有utf8mb4的字符集参数，只有`UTF-8`。
https://dev.mysql.com/doc/refman/5.7/en/charset-connection.html

```sql
-- 修改字符集
ALTER database sxs DEFAULT CHARACTER SET utf8mb4;

ALTER TABLE t1 DEFAULT CHARACTER SET utf8mb4;

ALTER TABLE company
  DEFAULT CHARACTER SET utf8mb4,
  MODIFY status VARCHAR(32)
    CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL;

-- 查看字符集 https://stackoverflow.com/questions/1049728/
SELECT default_character_set_name FROM information_schema.SCHEMATA 
WHERE schema_name = "sxs";

SELECT CCSA.character_set_name FROM information_schema.`TABLES` T,
       information_schema.`COLLATION_CHARACTER_SET_APPLICABILITY` CCSA
WHERE CCSA.collation_name = T.table_collation
  AND T.table_schema = "sxs"
  AND T.table_name = "resume";

SELECT character_set_name FROM information_schema.`COLUMNS` 
WHERE table_schema = "sxs"
  AND table_name = "resume"
  AND column_name = "uuid";

-- utf8mb4_unicode_ci
SELECT column_name, COLLATION_NAME FROM information_schema.`COLUMNS` 
WHERE table_schema = "main"
  AND table_name = "resume_deliver"
  AND column_name like "%uuid%";

-- 查询时指定字段的字符集
select * from tag_resume where resume_uuid in ('😊' COLLATE utf8mb4_general_ci,'a' COLLATE utf8mb4_general_ci);
```

# 启动/停止数据库

[参考资料](http://www.cnblogs.com/linjiqin/p/3544472.html)

	sudo service mysql start
	sudo service mysql stop
	
或（mysql5.7/rhel7）

	sudo service mysqld start

# 登录服务器

	mysql --default-character-set=utf8mb4 -h localhost -u root -p

p之后直接回车，以密文传输口令。也可以在p之后空格间隔dbName。

# 创建/打开/删除数据库

	create database zjh;
	CREATE DATABASE sxs CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
	drop database zjh;

显示已经创建的数据库

	show databases;

打开zjh数据库为当前数据库

	use zjh;

# DDL & DML语句

	create table test (c1 varchar(10));

	insert into test (c1) values ('中国');

	commit;

	select * from test;

增加字段：
ALTER TABLE tag_intern 
  ADD c1 varchar(10),
  ADD c2 varchar(10);
删除索引：`alter table resume drop index uuid; --uuid is column name`
创建索引：`alter table tag_intern add unique index idx_name2 (id);`
查询索引：`show index from resume;`

执行Sql脚本

	登录mysql后执行：

		source home/chf/bookstore.sql   #导入bookstore脚本

	或在shell执行： 

		mysql -u root -p bookstore < home/chf/bookstore.sql

当前日期

	select now(), curdate(), curtime() from dual;

## 查询版本、当前数据库、连接等状态信息 

```
status;
show status like 'Conn%';
SHOW VARIABLES LIKE 'max_connections';
```

## udpate select

https://stackoverflow.com/questions/1262786/mysql-update-query-based-on-select-query
```sql
update next_question_log a
join suite_temp b on a.sessionId = b.sessionId
set a.suiteId = b.suiteId;
-- where clause can go here
```

## 从shell发起命令

mysql -h 192.168.0.245 -u ipip -p db_name -e "select ip, name from host limit 10;" | tail -n +2

## load数据

```sql
--默认用tab分隔各列，换行符分隔行。
load data local infile "1.txt" into table books;

-- 用逗号分隔各列。
load data local infile 'myfile' into table mytable fields terminated by ',' lines terminated by '\n';
```

如果不指定`local`关键字，文件必须在服务器上；有时会报错：`ERROR 1290 (HY000): The MySQL server is running with the --secure-file-priv option so it cannot execute this statement`。

http://www.cnblogs.com/ggjucheng/archive/2012/11/05/2755683.html
https://dev.mysql.com/doc/refman/5.7/en/load-data.html

### 导出数据

```
# shell
# dump schema def & data
mysqldump -h localhost \
	-u root -p \
	--databases main --default-character-set=utf8mb4 \
	--tables tb1 tb2 \
	> export.sql
# only schema def
mysqldump -h localhost -d \
        -u root -p \
        --databases zjh \
        --tables question question_kn question_suite \
        > def.sql
```

### 导出查询结果

```sql
select *, s.delay - a.delay diff
from 
	( 
	select substring(stime,1,5) time, round(avg(delay),3) delay from sxs 
	group by time
	) s,
	(
	select substring(stime,1,5) time, round(avg(delay),3) delay from ali 
	group by time
	) a
where s.time = a.time
INTO OUTFILE '/var/lib/mysql-files/delay.csv'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';
```

### 导入数据

```
mysql -h localhost -D main -u root -p < export.sql
```

## 抽取json字段

https://dev.mysql.com/doc/refman/8.0/en/json.html
```sql
SELECT name, tags->"$[0]" AS `tag1` FROM `book`;
SELECT name, profile->"$.twitter" AS `twitter` FROM `user`;
```

## virtual column

```sql
create table test (
...
`qPk` VARCHAR(256) GENERATED ALWAYS AS (concat(clientKey,'_',suiteId,'_',lastAnswer->"$.qid")) VIRTUAL
);

ALTER TABLE next_question_log ADD `qPk` VARCHAR(256) GENERATED ALWAYS AS (concat(clientKey,'_',suiteId,'_',lastAnswer->"$.qid")) VIRTUAL;
```

## JDBC

[mysql的JDBC驱动默认一次性读取ResultSet的全部数据](https://dev.mysql.com/doc/connector-j/5.1/en/connector-j-reference-implementation-notes.html)
如果查询大数据集，可参考如下代码：
```java
conn = DriverManager.getConnection("jdbc:mysql://localhost/?useCursorFetch=true", "user", "s3cr3t");
stmt = conn.createStatement();
stmt.setFetchSize(100);
rs = stmt.executeQuery("SELECT * FROM your_table_here");
```

### batch

https://stackoverflow.com/questions/14264953/how-is-jdbc-batch-update-helpful
https://stackoverflow.com/questions/26307760/mysql-and-jdbc-with-rewritebatchedstatements-true
https://dev.mysql.com/doc/connector-j/5.1/en/connector-j-reference-configuration-properties.html
https://www.journaldev.com/2494/jdbc-batch-insert-update-mysql-oracle
https://mariadb.com/resources/blog/mariadb-java-connector-driver-performance
https://blog.jooq.org/2014/01/16/what-you-didnt-know-about-jdbc-batch/

## python

```python
import MySQLdb
# fetch size || cursor
# https://stackoverflow.com/questions/337479/how-to-get-a-row-by-row-mysql-resultset-in-python
import MySQLdb.cursors
MySQLdb.connect(user="root", passwd="root", db="sxs", cursorclass = MySQLdb.cursors.SSCursor)
```

## 连接丢失，time out

https://dev.mysql.com/doc/refman/8.0/en/gone-away.html

## history命令历史记录

`more ~/.mysql_history`

# 疑问

create table 的列名与数据类型之间,不能是tab？最后一列不能有`--`注释？

