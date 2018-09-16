# Azure HDInsight 概要

## Ambari

HDInsight 使用 Ambari 管理 Hadoop 生态集群，包括 Hadoop 、 HBase 、Hive 、 ZooKeeper 、 Spark 、 Storm 等。
Ambari 自身也是一个分布式架构的软件，主要由两部分组成： Ambari Server 和 Ambari Agent 。 Ambari 的 Server 会 SSH 到 Agent 的机器，拷贝并执行一些命令。
用户通过 Ambari Server 通知 Ambari Agent 安装对应的软件；Agent 会定时地发送各个机器每个软件模块的状态给 Ambari Server ，最终这些状态信息会呈现在 Ambari 的 GUI ，方便用户了解到集群的各种状态，并进行相应的维护。

Stack 相当于一个 Hadoop 生态圈软件的集合。

https://www.ibm.com/developerworks/cn/opensource/os-cn-bigdata-ambari/index.html
https://www.ibm.com/developerworks/cn/opensource/os-cn-bigdata-ambari2/index.html
https://www.ibm.com/developerworks/cn/opensource/os-cn-bigdata-ambari3/index.html

## Hive

数据库是表的目录或命名空间。如果没有显式创建、指定数据库，将会使用默认数据库default。

Hive目前也支持行级别的数据更新、插入、删除操作。但更多是批处理操作，比如将HDFS中的文件导入Hive。
Hive也支持Java/Python客户端，但更多是用于查询，通常不用jdbc进行数据加载。

[Hive开发规范、最佳实践](https://wenku.baidu.com/view/85055bb56f1aff00bed51e80.html)
[DML and ACID Transactions](https://zh.hortonworks.com/tutorial/using-hive-acid-transactions-to-insert-update-and-delete-data/)





