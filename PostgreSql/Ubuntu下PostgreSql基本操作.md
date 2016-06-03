
# 参考资料

http://wiki.ubuntu.com.cn/PostgreSQL

试验环境（Ubuntu16.04）

## 安装

`sudo apt install postgresql pgadmin3`

安装后会新建用户postgres。`grep postgres /etc/passwd`，home目录是 /var/lib/postgresql 。

### 启动、关闭数据库

sudo /etc/init.d/postgresql start
sudo /etc/init.d/postgresql stop

### 设置密码

sudo passwd postgres
sudo su postgres -c psql template1
	ALTER USER postgres WITH PASSWORD 'xxx';

### 创建数据库

su  postgres
	createdb zjh

## 数据类型

http://www.php100.com/manual/PostgreSQL8/datatype.html

## 创建表

```sql
CREATE TABLE zjh.idc_scan
(
   csegment character varying(40), 
   type character varying(40), 
   key character varying(40), 
   value numeric(12,0)[]
);
```

## 数据导入

```sql
COPY my_table FROM 'C:/Users/testUser/Desktop/TSV/ratings.list.tsv' DELIMITER '\t';
COPY zjh.idc_scan FROM '/var/lib/postgresql/idc_scan_parse';
COPY (SELECT * FROM zjh.idc_scan) TO '/var/lib/postgresql/idc.copy';
```

数据最好放到用户postgres的home目录，并将owner改为 postgres 。

http://stackoverflow.com/questions/20455378/insert-tsv-files-into-postgresql-db
https://www.postgresql.org/docs/current/static/sql-copy.html

## 连接数据库

`psql -h 127.0.0.1 -U postgres -d zjh`

切换数据库：`\connect postgres` 或 `\c zjh`

退出psql：`\q`

## SQL

### with as 语句

https://www.postgresql.org/docs/current/static/queries-with.html

### rank() row_number()

http://www.cnblogs.com/gaojian/archive/2012/09/06/2672982.html

### 获取当前时间的时间戳

select extract(epoch FROM now())

### minus

```sql
select c1 from t1
except
select c1 from t2
```

http://www.cnblogs.com/sekihin/archive/2009/01/14/1375329.html

















## 参考资料

https://wiki.postgresql.org/wiki/9.1%E7%AC%AC%E4%B8%80%E7%AB%A0
http://www.postgres.cn/docs/9.3/app-psql.html

### 显示行数

从第2行开始，最多输出5行。
`SELECT * FROM TABLENAME LIMIT 5 OFFSET 1;`

http://blog.csdn.net/seraph021724/article/details/8136788
http://stackoverflow.com/questions/1485391/how-to-get-first-and-last-record-from-a-sql-query

## Creating a Database Cluster

CH 17.2. Creating a Database Cluster，P435

database cluster指数据库存储（database storage area on disk）。默认路径是 /var/lib/pgsql/9.4/data 。

database cluster初始化后，会自动创建一个名为postgres的数据库，as a default database for use by utilities, users and third party applications. 
database server自己的运行并不需要postgres数据库，但很多第三方应用都假定这个库是存在的。

同时还会创建一个template1库，这个库作为后续创建的数据库的模版。template1库不应被实际使用。

初始化数据库存储的命令（P435）：

	su postgres
	export PATH=$PATH:/usr/pgsql-9.4/bin
	cd ~
	initdb -D /var/lib/pgsql/9.4/data	# 或 pg_ctl -D /var/lib/pgsql/9.4/data initdb

## 启动数据库

pg_ctl -D /var/lib/postgresql/9.5/data -l logfile start


## 配置文件

CH 4.2.6 Using Option Files，P301列出了配置文件的地址，P303/304给出了`!include/!includedir`指令用法，以及基本配置方法。如5.7中，conf.d目录中的配置文件不会默认加载。

如 `!include /etc/my.cnf.d/zjh.cnf` 

## 大小写

Schema Object是否区分大小写，取决于Server的操作系统、数据库的参数设置等。具体参考 CH 9.2 Language Structure/Schema Object Names。
列名、索引名等是不区分大小写的。（P1265）

默认情况下，rhel7中是区分table名的大小写的。

## 注释

mysql支持的注释格式：(1) /* ... */ (2) # ... (3) -- ...

## SQL Mode

CH 5.1.7 Server SQL Modes

### 查询当前的SQL Mode

	select @@global.sql_mode;
	select @@session.sql_mode;

# 数据类型

Chapter 11 Data Types

## 数值类型

SERIAL is an alias for BIGINT UNSIGNED NOT NULL AUTO_INCREMENT UNIQUE。（P1370）

bit最长只能到64位（P1370，1381）。

tinyInt 是1字节，smallInt 是2字节，mediumInt 是3字节, int 是4字节，bigInt 是8字节。int是integer的别名。（P1379）

所有的计算都使用signed BIGINT or DOUBLE。（P1372）所以不能对超过63位的bigInt进行算术运算。

decimal最大可以支持65位，计算应该会比各种int更耗费资源（P1372）。

auto_increment列如果设置了一个值，就从这个值开始递增，每次仍增1（P1381-1382）。如

	create table t2 (c2 varchar(20), c1 int(1) primary key auto_increment);
	insert into t2 (c1, c2) values(21, 'a');
	insert into t2 (c2) values('b');
	select * from t2;

**注意：**int(4)表示显示宽度，而不是存储的宽度。resultset返回的元数据为4，但是仍然可以正常显示超过4位的数值。如

	create table t1 (c1 int(1));
	insert into t1(c1) values(23);
	select * from t1;  --显示23

## 日期类型

datetime, timestamp 支持分数秒，最大到微妙，即百万分之一秒（6位精度）。（P1373）
timestamp是存储的UTC时间，检索时再转换为当前时区。（P1385）

time类型取值范围是正负838小时。time不仅可以表示时间点，还可以标示两个时间的差，所以取值范围比24小时大。（P1386）

year是单字节类型，取值范围是1900-2155。可以用1999直接赋值。

date不带时分秒；datetime和timestamp带时分秒，但是日期的取值范围不同；datetime的日期范围更大。（P1374）

定义秒级精度的模式是 type_name(fsp) ，如 `CREATE TABLE t1 (t TIME(3), dt DATETIME(6));` ，字段t的精度是豪秒，dt的精度是微秒。（P1373）

在某些sql mode和环境配置下，date和datetime支持1999-12-00这样的数据（如只知道生日的月份，不知道具体日期）；超过range范围的日期，会被转换为0。但是本试验的默认环境不可行。（P1384）

**注意：**如果没有特别约定，在插入/修改表的数据时，表的第一个timestamp字段如果没有显式赋值，会自定设置为插入/修改日期和时间（P1374。最好还是显式赋值吧）。如：

	create table t3 (c1 int, c2 timestamp);
	insert into t3 (c1) values (1);
	select * from t3;
	update t3 set c1 = 2;
	select * from t3;

datetime和timestamp的默认值在建表时可被设置为：`default current_timestamp` 或 `on update current_timestamp`（P1374，CH 11.3.5）

### 日期的加减运算

日期减1，其实是将日期转换为数字（从年到秒），然后减1秒，如 `select now(), now()-1 from dual`。
日期运算，需要调用日期函数。如

	select date_add('2008-01-02', interval 31 day);	-- 增加的单位可以从微秒到年。day表明增加的单位是天。
	select date_add('2008-01-02', interval -1 day);	-- 负数也可以加
	select date_add('2008-01-31', interval 1 month); 	-- 可以正确处理闰月
	select datediff('2008-01-31', '2008-01-02') from dual;	-- 截取整天，不带时分秒
	-- 使用timestampdiff计算精确的、带时分秒的时差
	select timestampdiff(minute,'2003-02-01','2003-05-01 12:05:55')/(60*24.0), timestampdiff(day,'2003-02-01','2003-05-01 12:05:55'); 

## 字符类型

varchar最大长度是65535字节。varchar是`character varying`的简写。（P1377，1396）
如果因为末尾的空白字符导致超长，varchar和char都会截掉末尾的空白，不论是什么sql mode。（P1396）

	create table t5 (c1 varchar(4));
	insert into t5(c1) values('1234  ');
	select * from t5;

binary 和 varbinary 是基于字节，而不是字符。binary字段的实际长度如果小于预定义的长度，向右补0（而不是空格）。（P1398）

blob是二进制字符串，没有字符集；text是字符组成的字符串（非二进制）。（P1399）

enum在内部表示为整数，但在查询、输出时自动转换为字符串。（P1378，1400）

## 数据类型的存储

CH 11.8 Data Type Storage Requirements

mysql一行（row）的最大长度是65535字节，不论是什么存储引擎。（P1434）

# 函数与操作符

## 操作符

<=> 安全的null比较操作符，如 `select 1 <=> 1, NULL <=> NULL, 1 <=> NULL;`（P1460）

case when then end 操作符。（P1468）

IN操作符，如 `SELECT 2 IN (0,3,5,7);`

IF函数相当于decode，ifNull相当于nvl

bin返回一个整数的二进制表示，便于将IP地址转换为二进制表示，如 `SELECT BIN(12);`

concat连接字符串，如 `SELECT CONCAT('My', 'S', 'QL');`

strstring相当于substr

INET_ATON()将IP地址转换为数字。（P1662）

GROUP_CONCAT可以连接字符串。（P1672）

支持 COALESCE()、GREATEST()、LEAST()、ISNULL()、

# 不支持的弱项

不支持 full outer join ； 不支持 row_number/rank_number 等分析窗口函数；貌似不支持 with as 语句。

