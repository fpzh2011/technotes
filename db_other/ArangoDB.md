# ArangoDB概要

## 安装

[Ubuntu 16.04](https://www.arangodb.com/download-major/ubuntu/)
```shell
curl -L https://download.arangodb.com/arangodb33/xUbuntu_16.04/Release.key | sudo apt-key add -
echo 'deb https://download.arangodb.com/arangodb33/xUbuntu_16.04/ /' | sudo tee /etc/apt/sources.list.d/arangodb.list
sudo apt-get install apt-transport-https
sudo apt-get update
sudo apt-get install arangodb3=3.3.5
```

安装过程提示：
```
2018-03-29T10:27:23Z [27986] WARNING {memory} /proc/sys/vm/overcommit_memory is set to '0'. It is recommended to set it to a value of 2
2018-03-29T10:27:23Z [27986] WARNING {memory} execute 'sudo bash -c "echo 2 > /proc/sys/vm/overcommit_memory"'
```

其它：
* 默认的存储引擎是MMFiles，数据都放在内存。安装过程中设置为RocksDB。
* ArangoDB侦听tcp端口8529。可以访问[web控制台](http://127.0.0.1:8529)
* 禁止自动启动：`sudo systemctl disable arangodb3`
* 手动启动：`sudo systemctl start arangodb3`

### ArangoDB程序组件

https://docs.arangodb.com/3.3/Manual/GettingStarted/#arangodb-programs

## 创建数据库和用户

https://docs.arangodb.com/3.3/Manual/GettingStarted/Authentication.html
```shell
# arangosh
db._createDatabase("example");
var users = require("@arangodb/users");
users.save("zjh@example", "pwd");
users.grantDatabase("zjh@example", "example");
# shell, password=pwd
arangosh --server.username "zjh@example" --server.database example
```

## 数据模型

https://docs.arangodb.com/3.3/Manual/DataModeling/Concepts.html#data-model
* 支持document、graph、key/value模型。
* multi tenant数据库。
* collection对应关系数据库中的table。
* schema-less。document对应关系数据库中的row，保存document的接口接受json格式的数据。
* 图数据库。普通的document collection构成vertex collections。edge collections也是documents，但只包含两个特殊属性：`_from`和`_to`。

## 数据库操作

### 改变当前数据库

```shell
arangosh --server.username "zjh@example" --server.database example
# or
db._useDatabase("example")
```

https://docs.arangodb.com/3.3/Manual/DataModeling/Databases/
* ArangoDB的数据库连接是无状态的，所有状态信息都保存在http的request/response中。
* 一个命令/查询，只能访问一个数据库的数据。命令/查询不能跨数据库访问数据。

### 数据库常规操作

https://docs.arangodb.com/3.3/Manual/DataModeling/Databases/WorkingWith.html
部分操作只能在`_system`数据库进行。
```shell
# arangosh
require("@arangodb");
require("@arangodb").db._name();
require("@arangodb").db._id();
db._useDatabase("example");
db._databases();
# 创建数据库后，当前数据库并未改变，需要调用db._useDatabase()修改当前数据库。
# 如果不指定用户，会自动创建root用户，口令为空字符串。
db._createDatabase(name, options, users);
# 用户操作
require("@arangodb/users").save(username, password, true);
require("@arangodb/users").update(username, password, true);
require("@arangodb/users").remove(username);
# 删除数据库
db._dropDatabase("example");
# 存储统计
db._engineStats();
```

[注意事项](https://docs.arangodb.com/3.3/Manual/DataModeling/Databases/Notes.html)
* 每个tenant数据库有自己独立的Replication配置和system collections

## Collections操作

### 创建Collention

https://docs.arangodb.com/3.3/Manual/DataModeling/Collections/
```shell
# arangosh
# 创建Collection。默认是document collection
db._create("movie")
# 获取Collection
db._collection("movie")
# 获取Collection(仅适用non-system collections)
db.movie
```

### Collection Methods

https://docs.arangodb.com/3.3/Manual/DataModeling/Collections/CollectionMethods.html
```shell
# arangosh
# Drop
db._collection("movie").drop()
# Truncate
db._collection("movie").truncate()
db._truncate("movie") # https://docs.arangodb.com/3.3/Manual/DataModeling/Collections/DatabaseMethods.html
# 查询collention属性
# keyOptions.allowUserKeys=true允许用户提供_key字段。默认为true
db._collection("movie").properties()
# 设置collention属性。collection创建后，好像就不能改keyOptions了。
db.movie.properties({waitForSync : true});
# statistics about the collection
db.movie.figures();
# 加载collection到内存
db.movie.load()
# load的逆操作。不适用于cluster环境
db.movie.unload()
# 重命名。不适用于cluster环境
db.movie.rename("movie2")

# https://docs.arangodb.com/3.3/Manual/DataModeling/Collections/DatabaseMethods.html
# 创建Edge Collection
db._createEdgeCollection("edge")
# 创建document colletion
db._createDocumentCollection("doc")
# 创建collection时指定keyOptions
db._createDocumentCollection("doc", {keyOptions : {allowUserKeys: false}})
# 列出全部collection
db._collections();
# 删除collection
db._drop("doc")
```

## document操作

### document概要

[ArangoDB中的文档是JSON对象。](https://docs.arangodb.com/3.3/Manual/DataModeling/Documents/DocumentAddress.html)
[在一个Collection中，_key属性作为主键，区分不同文档。](https://docs.arangodb.com/3.3/Manual/Appendix/Glossary.html#document-key)
`_key`的值在文档创建后就不可更改。如果用户没有指定`_key`的值，数据库会自动生成一个值。
[_key的值需符合规范](https://docs.arangodb.com/3.3/Manual/DataModeling/NamingConventions/DocumentKeys.html)

参考：
https://docs.arangodb.com/3.3/Manual/DataModeling/Documents/DocumentAddress.html
https://docs.arangodb.com/3.3/Manual/Appendix/Glossary.html#document

### Collection Methods

https://docs.arangodb.com/3.3/Manual/DataModeling/Documents/DocumentMethods.html
```shell
# 查询所有文档，返回cursor。可以用toArray/next/hasNext访问文档。、
# select * from t1;
collection.all()
# select * from t1 where a.c = 1;
collection.byExample({ "a.c" : 1 })
# collection的文档个数
collection.count()
# 根据_key或id查询document
db.example.document("2873916");
db.example.document(["2873916","2873917"]);
# 根据_key或_id检测文档是否存在。返回的不是文档对象，而是只包含_key或_id字段的对象
collection.exists(object)
# 插入一个文档。如果指定_key，collection中不能存在同样的_key。
# 返回对象包含字段：_id, _key, _rev
# insert is an alias for save
collection.insert(data[, options])
# 将array中的每个对象作为单独的文档存入collection
collection.insert(array[, options])
# 根据_id或_key替换一个或多个已经存在的文档。文档被整个替换，而不是更改个别字段。
collection.replace(selector, data[, options])
collection.replace(selectorarray, dataarray[, options])
# 根据_id或_key更新一个或多个已经存在的文档。可以追加新字段，也可以修改已有字段的值。
collection.update(selector, data[, options])
collection.update(selectorarray, dataarray[, options])
# 根据_id或_key删除已经存在的文档。
collection.remove(selector[, options])
collection.remove(selectorarray[,options])
# 删除符合某种特征的文档
collection.removeByExample(document[[, waitForSync], limit])
# 判断collection是document还是edge
collection.type()

# 上述方法都适用于edge
# edge特有的方法：
# 与节点相连的所有边
edge-collection.edges(vertex)
edge-collection.edges(vertices)
edge-collection.inEdges(vertices)
```



https://docs.arangodb.com/3.3/Manual/DataModeling/Documents/DatabaseMethods.html

edge能否带属性？
一个文档会保留多少个版本？




