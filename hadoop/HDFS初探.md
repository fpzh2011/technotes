# HDFS初探

```shell
# hadoop-2.8.0 | centos7.3_64 | jdk1.8.0_121-b13

# hdfs基本结构（重要）
# http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/HdfsDesign.html
# Hadoop Commands Guide，各个命令通用的选项，如 --config
# http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/CommandsManual.html
# FileSystem Shell
# http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/FileSystemShell.html
# 	HDFS Users Guide. 主要是Cluster管理的相关内容
# 	http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/HdfsUserGuide.html


# ============================================================================ #
# 单节点
# http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/SingleCluster.html
export JAVA_HOME=/etc/alternatives/jre
bin/hadoop
cp etc/hadoop/core-site.xml etc/hadoop/core-site.xml.bak
# 修改core-site.xml
# <configuration>
#     <property>
#         <name>fs.defaultFS</name>
#         <value>hdfs://localhost:9000</value>
#     </property>
# </configuration>
cp etc/hadoop/hdfs-site.xml etc/hadoop/hdfs-site.xml.bak
# 修改hdfs-site.xml。注意要增加 dfs.datanode.data.dir 配置，否则不会启动datanode
# <configuration>
#     <property>
#         <name>dfs.replication</name>
#         <value>1</value>
#     </property>
#     <property>
# 		<name>dfs.namenode.name.dir</name>
#         <value>/home/zjh/hadoop-2.8.0/namenode-data</value>
#     </property>
#   <!-- DataNode配置 -->
#   <property>
#     <name>dfs.datanode.data.dir</name>
#         <value>file:///home/zjh/hadoop-2.8.0/data</value>
#   </property>
# </configuration>

# 配置ssh免登录
ssh-keygen -t rsa -P '' -f ~/.ssh/id_rsa
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
chmod 0600 ~/.ssh/authorized_keys
# ssh localhost

# http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/HDFSCommands.html#namenode
# Formats the specified NameNode. It starts the NameNode, formats it and then shut it down.
# 格式化namenode数据目录，即 etc/hadoop/hdfs-site.xml 的 dfs.namenode.name.dir 属性指定的目录，NameNode元数据，即fsimage。
# 单节点下默认目录为 /tmp/hadoop-xxx/dfs/name 。
# 可通过etc/hadoop/hdfs-site.xml的dfs.namenode.name.dir参数进行设置。
# 不会涉及 DataNode 的数据目录 dfs.datanode.data.dir 。
# 这个命令是在 NameNode 本地执行的，执行时 NameNode 还没有启动（ format 之后 NameNode 才能启动），所以不涉及网络交互。
# 可参考 http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/ClusterSetup.html
bin/hdfs namenode -format

# Start NameNode daemon and DataNode daemon
# 在 hdfs-site.xml 中配置 dfs.datanode.data.dir 才会启动datanode
# datanode第一次启动时，会初始化 dfs.datanode.data.dir 目录。datanode启动后，该目录下会创建文件 in_use.lock ，文件内容是 datanodepid@hostname
# ~/.bashrc 必须设置JAVA_HOME。
# 因为start-dfs.sh调用sbin/hadoop-daemons.sh，后者以exec方式调用脚本，这会执行.bashrc。
# 通过 jps 或 ps aux | grep java 或 netstat -ntlp可以看到，下面这条命令会启动NameNode、SecondaryNameNode和DataNode。
# NameNode 通过ssh登录到 DataNode 等节点启动服务。即使 DataNode 和 NameNode 在同一服务器，也用ssh登录启动服务。
# 可以访问  http://localhost:50070/ 和 http://localhost:50090/
# 通过 jps 、netstat -ntlp 、netstat -ntp 、 wireshark 可以看到：
#	DataNode 和 SecondaryNameNode 分别与 NameNode 的9000端口建立tcp连接。
#	50010 端口是 DataNode 用于跨进程/网络传输数据的侦听端口。
#	50020 端口是 DataNode 用于跨进程/网络传输元数据的侦听端口。
#	50070 端口是 NameNode 的Web访问侦听端口（Jetty）。
#	50075 端口是 DataNode 的Web访问侦听端口。
#	50090 端口是 SecondaryNameNode 的Web访问侦听端口，但如果NameNode是active的，50090没有实际内容。
#	DataNode还有一个localhost侦听端口 45575（不固定），通过 DataNode 的日志可以看到是 Jetty 的端口，但不清楚是干什么用的。
# 以上参考：
#	https://ambari.apache.org/1.2.3/installing-hadoop-using-ambari/content/reference_chap2_1.html
#	https://stackoverflow.com/questions/22855232/default-namenode-port-of-hdfs-is-50070-but-i-have-come-across-at-some-places-802
#	http://www.cnblogs.com/ggjucheng/archive/2012/04/17/2454590.html
# 启动后，还会在 /tmp 目录创建其它一些目录，如 hadoop-zjh (namesecondary) 、Jetty* 、 hsperfdata_zjh 、 hadoop-*.pid 等。
# 通过观察 logs 目录下的启动日志，也可以了解一些启动信息。
sbin/start-dfs.sh

# Make the HDFS directories required to execute MapReduce jobs
# 向NameNode的9000端口发起创建目录请求，不涉及DataNode
bin/hdfs dfs -mkdir /user
bin/hdfs dfs -mkdir /user/zjh
bin/hdfs dfs -mkdir /user/test
# 拷贝本地目录 etc/hadoop 为hdfs的 /user/zjh/input
# 向 NameNode 的 9000 端口发起创建文件请求，再向 DataNode的 50010 端口传输文件数据。
bin/hdfs dfs -put etc/hadoop input
# 向 NameNode 的 9000 端口发起查询，不涉及 DataNode
bin/hdfs dfs -ls /user/zjh/output
# 在 NameNode 本地向 9000 端口发起查询。如果是在 DataNode 上运行命令，而且数据在该节点，则不会发生数据传输。
bin/hdfs dfs -cat /user/zjh/output/*
# 在 NameNode 本地通过localhost访问
bin/hdfs dfs -cat hdfs://localhost:9000/user/zjh/output/*
# 在远程通过HdfsShell访问。 core-site.xml 必须配置为 192.168.1.107:9000 。可以用 --config confdir 指定配置文件目录。
# NameNode 会告诉客户端 DataNode 的IP地址、数据目录的本地路径。
bin/hdfs dfs -cat hdfs://192.168.1.107:9000/user/zjh/LICENSE.txt
# 在远程通过HdfsShell访问。 --config 指定的目录下必须有配置文件 core-site.xml ，该配置文件中设置了NameNode的地址和端口。
bin/hdfs --config ./ dfs -cat /user/zjh/LICENSE.txt
# 同时显示本地文件和hdfs中的文件
bin/hadoop fs -cat file:///home/zjh/hadoop-2.8.0/README.txt /user/zjh/LICENSE.txt

# 运行grep任务，并保存到 /user/zjh/output 目录
bin/hadoop jar ./share/hadoop/mapreduce/hadoop-mapreduce-examples-2.8.0.jar grep /user/zjh/input/* output 'dfs[a-z.]+'

# 停止
sbin/stop-dfs.sh

# 关于 bin/hdfs 、 bin/hadoop fs 、 bin/hadoop dfs 的区别
#	https://dzone.com/articles/difference-between-hadoop-dfs
#	https://stackoverflow.com/questions/18142960/whats-the-difference-between-hadoop-fs-shell-commands-and-hdfs-dfs-shell-co
#	http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/FileSystemShell.html
#	http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/HdfsUserGuide.html
#	default scheme 应该是由 core-site.xml 的属性 fs.defaultFS 定义

# http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/ClusterSetup.html
#	NameNode and ResourceManager 是集群中的Master。DataNode and NodeManager 是slave。
#	默认配置，4个配置文件：core-default.xml, hdfs-default.xml, yarn-default.xml and mapred-default.xml
#	Site-specific configuration: etc/hadoop/core-site.xml, etc/hadoop/hdfs-site.xml, etc/hadoop/yarn-site.xml and etc/hadoop/mapred-site.xml

# hadoop架构解剖
#	http://ercoppa.github.io/HadoopInternals/HadoopArchitectureOverview.html


```
