# MongoDB笔记

https://www.w3cschool.cn/mongodb/
http://www.runoob.com/mongodb/mongodb-tutorial.html

## 安装

```shell
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 2930ADAE8CAF5059EE73BB4B58712A2291FA4AD5
echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu xenial/mongodb-org/3.6 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-3.6.list
sudo apt-get update
sudo apt-get install -y mongodb-org=3.6.6 mongodb-org-server=3.6.6 mongodb-org-shell=3.6.6 mongodb-org-mongos=3.6.6 mongodb-org-tools=3.6.6
echo "mongodb-org hold" | sudo dpkg --set-selections
echo "mongodb-org-server hold" | sudo dpkg --set-selections
echo "mongodb-org-shell hold" | sudo dpkg --set-selections
echo "mongodb-org-mongos hold" | sudo dpkg --set-selections
echo "mongodb-org-tools hold" | sudo dpkg --set-selections
```

### 删除

```shell
sudo apt-get purge mongodb-org*
sudo rm -r /var/log/mongodb
sudo rm -r /var/lib/mongodb
```

### 基本管理

```shell
sudo service mongod start
sudo service mongod stop
mongo --host 127.0.0.1:27017
```

https://www.mongodb.com/download-center#community
https://docs.mongodb.com/master/tutorial/install-mongodb-on-ubuntu/
https://docs.mongodb.com/v3.6/tutorial/install-mongodb-on-ubuntu/

###　创建用户

```
db.createUser(
   {
     user: "writer",
     pwd: "123456",
     roles: [ { role: "readWrite", db: "corpus" } ]
   }
)
```
https://docs.mongodb.com/manual/reference/method/db.createUser/
https://blog.csdn.net/zZ_life/article/details/78664794
https://docs.mongodb.com/manual/reference/built-in-roles/

### 特殊数据库名

* admin: 从权限的角度来看，这是"root"数据库。要是将一个用户添加到这个数据库，这个用户自动继承所有数据库的权限。一些特定的服务器端命令也只能从这个数据库运行，比如列出所有的数据库或者关闭服务器。
* local: 这个数据永远不会被复制，可以用来存储限于本地单台服务器的任意集合。
* config: 当Mongo用于分片设置时，config数据库在内部使用，用于保存分片的相关信息。
* test: mongo shell的默认数据库。
http://www.runoob.com/mongodb/mongodb-databases-documents-collections.html

## mongo shell

```shell
mongo
# 帮助
db.col.help()
# 显示所有数据库列表
show dbs
# 显示当前数据库对象或集合
db
# 创建数据库，如果已经存在就连接
use zjh
# 插入数据。如果没有数据，show dbs不会显示新创建的数据库
db.zjh.insert({"name":"zjh"})
# 删除当前数据库
db.dropDatabase()
# 创建集合。当你插入一些文档时，MongoDB 会自动创建集合。
db.createCollection(name, options)
#查看已有集合
show collections
# 删除集合
db.collection_name.drop()
# 插入文档 insertOne/insertMany/save
db.COLLECTION_NAME.insert(document)
# 查看集合所有文档
db.col.find()
# 插入文档也可以使用 db.col.save(document) 命令。如果不指定 _id 字段 save() 方法类似于 insert() 方法。如果指定 _id 字段，则会更新该 _id 的数据
```


## 存储限制

单个数据库最大32T。
https://docs.mongodb.com/manual/reference/limits/
This means that a single MMAPv1 database has a maximum size of 32TB.






如何根据配置动态制定集合名字？动态删除集合？
文档_id的作用是什么？可否用url作为_id？


