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

	[client]
	default-character-set=utf8

	[mysql]
	default-character-set=utf8

显示字符集设置（如果提示Table 'performance_schema.session_variables' doesn't exist，停止mysql数据库，运行`mysql_upgrade -u root -p --force`，再重启数据库）

	show variables like 'character%';

另外，可以参考 CH 5.1.4 Server System Variables；以及 CH 4.2.6 Using Option Files，这里列出了配置文件的地址，以及基本配置方法。

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

# 疑问

create table 的列名与数据类型之间,不能是tab？最后一列不能有`--`注释？

