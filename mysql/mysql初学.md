mysql初学
=========

# 口令

yum安装时需要输入口令，如 `rootpwd`

# 修改字符集

[参考资料](http://blog.chinaunix.net/uid-545411-id-2385599.html)

	set names utf8;
	
创建配置文件 `/etc/mysql/conf.d/zjh.cnf` （rhel7-mariadb是 `/etc/my.cnf.d/`，Ubuntu16是`/etc/mysql/mysql.conf.d/`，mysql5.7则要使用include指令）：
	
	[mysqld]
	character-set-server=utf8
	lower_case_table_names=1

	[client]
	default-character-set=utf8

	[mysql]
	default-character-set=utf8

显示字符集设置（如果提示Table 'performance_schema.session_variables' doesn't exist，停止mysql数据库，运行`mysql_upgrade -u root -p --force`，再重启数据库）

	show variables like 'character%';

另外，可以参考 CH 5.1.4 Server System Variables；以及 CH 4.2.6 Using Option Files，这里列出了配置文件的地址，以及基本配置方法。

## Unicode问题

https://stackoverflow.com/questions/13653712/
https://dev.mysql.com/doc/refman/5.5/en/charset-unicode-conversion.html
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
```

# 启动/停止数据库

[参考资料](http://www.cnblogs.com/linjiqin/p/3544472.html)

	sudo service mysql start
	sudo service mysql stop
	
或（mysql5.7/rhel7）

	sudo service mysqld start

# 登录服务器

	mysql -h localhost -u root -p

p之后直接回车，以密文传输口令。也可以在p之后空格间隔dbName。

# 创建/打开/删除数据库

	create database zjh;
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

查询版本、当前数据库、连接等状态信息 

	status;

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

## JDBC

[mysql的JDBC驱动默认一次性读取ResultSet的全部数据](https://dev.mysql.com/doc/connector-j/5.1/en/connector-j-reference-implementation-notes.html)
如果查询大数据集，可参考如下代码：
```java
conn = DriverManager.getConnection("jdbc:mysql://localhost/?useCursorFetch=true", "user", "s3cr3t");
stmt = conn.createStatement();
stmt.setFetchSize(100);
rs = stmt.executeQuery("SELECT * FROM your_table_here");
```

# 疑问

create table 的列名与数据类型之间,不能是tab？最后一列不能有`--`注释？

