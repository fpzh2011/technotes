# memsql基础

## 安装

参考：
http://docs.memsql.com/latest/setup/setup_onprem/
http://docs.memsql.com/latest/setup/quick_start/

从`http://memsql.com/download`下载安装介质。

运行`tar zxf memsql-ops-<version>.tar.gz`解压。

运行`sudo ./install.sh`安装。如果安装单机版本，可以运行`sudo ./install.sh --simple-cluster --host=127.0.0.1`。
安装程序主要做以下几件事：
* 创建操作系统的 memsql 用户。
* 在目录 /var/lib/memsql-ops 安装 memsql-ops 。
* 设置在系统启动时初始化 MemSQL Ops 。
* 修改操作系统参数，最大化 MemSQL 数据库性能。
* 可能还要下载文件，必要时手工下载。 http://download.memsql.com/releases/commit/d78c76bb960feb6e38a46977e7c95c2de7838c1e/memsqlbin_amd64.tar.gz

通过 http://primary_host_name:9000 可以访问memsql的管理界面，进行后续的安装配置。
* 选择社区版。
* 选择单机/多机部署。
* 配置memsql节点，点击按钮部署memsql集群。之后memsql会再次从官网下载安装介质，如果不成功，需要手工下载。

登录memsql：`mysql -u root -h localhost -P 3306 --prompt="memsql> "`。注意：安装时如果没有指定`--simple-cluster --host=127.0.0.1`，用localhost可能会无法登录，可以用hostname。
登录memsql后创建自己的用户和数据库：
```sql
-- create a new user
grant all on *.* to 'ipip'@'%';
-- create a new database
create database ipip;
show databases;
use ipip;
-- create a new rowstore table
create table mytable (id INT PRIMARY KEY, name varchar(10));
-- create a new columnstore table
create table mycolumnstoretable (id INT, groupid INT, name varchar(10), key (groupid) using clustered columnstore, shard key (id));
show tables;
describe mytable;
describe mycolumnstoretable;
-- perform data query operations
insert into mytable values (1, 'row');
select * from mytable;
insert into mycolumnstoretable values (1, 2, 'column');
select * from mycolumnstoretable;
select mytable.name, mycolumnstoretable.name from mytable inner join mycolumnstoretable on mytable.id = mycolumnstoretable.id;
-- determine which memsql version you are using
select @@memsql_version;
```

http://docs.memsql.com/latest/setup/best_practices/#memsql-ops-configuration
sudo apt-get install numactl

http://docs.memsql.com/latest/setup/best_practices/#memsql-database-configuration
修改limits

手动安装
http://docs.memsql.com/latest/setup/offline/

### 操作系统内核参数优化配置

参考：http://docs.memsql.com/latest/setup/best_practices/#memsql-database-configuration

修改`/etc/security/limits.conf`：
```
* soft NOFILE 1000000
* hard NOFILE 1000000
* soft NPROC 128000
* hard NPROC 12800
```

修改`/etc/sysctl.conf`：
```
vm.max_map_count=1000000000
vm.min_free_kbytes=500000
```

另外，还需要增大pipe size：`ulimit -p 128000`（这个命令不是每个Linux版本都支持），以及创建swap文件。

### 部署目录

数据库目录：`/var/lib/memsql`
程序目录：`/var/lib/memsql-ops`

### 客户端安装

安装mysql客户端：`sudo apt-get install mysql-client libmysqlclient-dev libmysql++-dev`。
其中dev是用C/C++连接memsql需要的库。

### 修改参数，优化写入性能

http://docs.memsql.com/latest/admin/memsql.cnf/#all-memsql-conf-options
http://docs.memsql.com/latest/admin/ops_settings.conf/

修改参数：
```
snapshot_trigger_size = 256m
transaction_buffer = 64m
core_file
critical_diagnostics
```

这两个文件都要修改？
```
/var/lib/memsql/leaf-3307/memsql.cnf
/var/lib/memsql/master-3306/memsql.cnf
```

http://docs.memsql.com/latest/admin/durability_recovery/
http://docs.memsql.com/latest/admin/memsql.cnf/

#### 其它可以关注的变量

```
SHOW GLOBAL VARIABLES;
snapshots_to_keep
maximum_memory
maximum_table_memory

locked_in_memory ?
local_infile ?
default_partitions_per_leaf
```

## 集群管理

http://docs.memsql.com/latest/admin/cluster/

首先需要启动节点，登录memsql客户端。

查询集群中的节点：`SHOW LEAVES;`

ATTACH LEAF 'localhost':3307;
ATTACH LEAF '127.0.0.1':3307;

删除叶子节点：REMOVE LEAF 'host':port


建表：
```sql
create table `test` ( `c1` int(10), PRIMARY KEY (`c1`));
```

## 自动提交

`SHOW GLOBAL VARIABLES;` 显示 `autocommit | ON`，memsql数据库默认是自动提交的。

## 其它命令

`memsql-ops help`
http://docs.memsql.com/latest/ops/cli/

查看memsql状态：`memsql-ops status`
卸载memsql：`memsql-ops uninstall`
查看agent：`memsql-ops agent-list`
查看节点：`memsql-ops memsql-list`
启动节点：`memsql-ops memsql-start 3E274DA2972BF2D7D455BD7D3D9483F2957DC66F`
停止节点：`memsql-ops memsql-stop 3E274DA2972BF2D7D455BD7D3D9483F2957DC66F`
新增节点：`memsql-ops memsql-deploy`
删除节点及其数据：`memsql-ops memsql-delete`
查看占用内存等信息：`show status;`


memsql写入行级锁。http://docs.memsql.com/latest/faq/#how-does-memsql-s-in-memory-lock-free-storage-engine-work
maximum_table_memory global variable 设置表的最大内存全局变量。
| maximum_memory               | 7884
| maximum_table_memory         | 6860



为什么这个无法登录，但是127可以？：mysql -u root -h localhost -P 3306 --prompt="memsql> "
Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock

error code。其它错误的error code是什么？比如drop不存在的表？


## 故障处理

如果机器重启后，lead node总是处于offline状态，无法启动，可以运行命令重启cluster: `memsql-ops memsql-stop --all; memsql-ops memsql-start --all`


如何禁止启动后自动加载数据库，或者进行数据库恢复？或者禁止某个数据库自动加载？


memsql-ops start	# Start the MemSQL Ops agent, port 9000
memsql-ops memsql-list	# List MemSQL nodes in the cluster.
memsql-ops memsql-start $masterId	# 启动master节点，port 3306
mysql -u root -h 127.0.0.1 -P 3306 --prompt="memsql> "
	show databases;
	drop database ipip;

memsql-ops memsql-delete DCDAE61 # 或者删除叶子节点。
sudo memsql-ops memsql-deploy -r leaf -P 3307 --community-edition # 新增叶子节点
修改内存配置 sudo vim /var/lib/memsql/leaf-3307/memsql.cnf
memsql-ops memsql-stop --all; memsql-ops memsql-start --all

## 问题

始终无法阻止写磁盘，尝试了很多办法：将transaction-buffer和snapshot-trigger-size加大到2G，禁止core-file。但从插入任务一启动，就开始有几十M每秒的磁盘写。

后续可以尝试使用ssd硬盘的效果。


