# redis概要

**安装**
sudo apt-get install redis-server # port 6379

**命令行客户端**
redis-cli

**查看server状况**
info

**hash table**
hmset resume_recommend "jdkey" '["resid1", "resid2"]'
hget resume_recommend "jdkey"

**将整个redis实例作为一个hash table**
set "mykey" '["v1", "v2"]'
get "mykey"

**查看所有key**
keys pattern

删除key
del keyname
删除选中数据库的所有key
flushdb
删除所有数据库的所有key
flushall

**expire**
redis可以对顶层key设置expire，不能对hash table对象的二级key设置expire。
redis 不能设置全局的 ttl ，但如果将整个实例作为一个hash表， `set key1 value1 ex 300`可以设置expire为300秒。
https://stackoverflow.com/questions/25618045/can-i-set-global-ttl-in-redis
https://redis.io/commands/set

**批量数据导入**

`cat data.txt | redis-cli --pipe --pipe-timeout 0`
https://redis.io/topics/mass-insert

```shell
# 批量导入list的多个值
redis-cli rpush list_name $(cat new_id.txt)
```

data.txt的样例：
```
SET Key0 Value0
SET Key1 Value1
...
SET KeyN ValueN
```

**redis-cli查询配置**
```
config get loglevel
config get rdb*
```

## 字符集

redis内部貌似默认是utf8存储。例如`lpush chn '中国' 'a云脑'`，在redis-cli查询后，结果是utf8编码。
或许redis只是对string照单全收，因为Ubuntu默认编码是utf8，所以redis存储的也是utf8。但是在Python中，即使将string设置为unicode，redis存储的也是utf8。
`redis-cli --raw`这样查询显示的就是中文。

## redis-py

```python
client = redis.StrictRedis(host='localhost', port=6379)

# 将list存入redis的list。如果没有序列解包，会存入二进制对象
# https://stackoverflow.com/questions/15850112/redis-how-to-parse-a-list-result
client.lpush('foo', *[1,2,3,4])
# foo是list对象，但其元素是字符串
foo = client.lrange('foo', 0, -1)

# 保存dict对象为hash
# https://stackoverflow.com/questions/32276493/how-to-store-and-retrieve-a-dictionary-with-redis
d = {'a':1, 'b':2}
client.hmset('zoo', d)
# d1是dict对象
d1 = client.hgetall('zoo')

# list<dict>
dl = [{'a':1, 'b':2}, {'a1':11, 'b1':21}]
client.rpush('dl', *dl)
# 从redis读出来，dl0是list，但其中的元素是str，而不是dict。
dl0 = client.lrange('dl', 0, -1)

# 综上所述，redis内部基本数据类型只有string，没有int等，也不支持复杂嵌套。
# https://stackoverflow.com/questions/15219858/how-to-store-a-complex-object-in-redis-using-redis-py

# 将list<dict>以json存入redis，再读取、解析、显示
data = [{'id':'1', 'tags':['中国', '云脑']}]
client.set('test', json.dumps(data[0]))
s = client.get('test')
j = json.loads(s)
# j['tags'][0] 是unicode类型
print j['tags'][0]
```

Java从redis读取list json数据：
```java
import java.io.*;
import java.util.*;
import redis.clients.jedis.Jedis;
import io.vertx.core.json.JsonObject;

//data = [{'id':'1', 'tags':['中国', '云脑']}]
public class Test {
    public static void main (String[] argv) {
        try (Jedis jedis = new Jedis("localhost", 6379)) {
          String s = jedis.get("test");
          System.out.println(s);
          JsonObject json = new JsonObject(s);
          System.out.println(json);
          System.out.println(json.getJsonArray("tags").getValue(0));
        } catch(Exception e){
            e.printStackTrace();
        }
    }
}
```

## 持久化存储

### RDB

```
# redis-cli
CONFIG SET dir /home/zjh/temp/redis_data
CONFIG SET dbfilename temp.rdb
BGSAVE
# 2秒内数据集存在1000次修改时，自动触发bgsave（Redis开发与运维，P155）
SAVE 2 1000
```

### AOF

Redis开发与运维，P157
```
# redis.conf
appendonly yes
dir /data
```

## Jedis

Jedis不是线程安全的，多线程环境下推荐使用`JedisPool`。
https://github.com/xetorthio/jedis/wiki/Getting-started#using-jedis-in-a-multithreaded-environment

`Jedis`实现了`Closeable`，按如下方式使用会自动关闭：
```java
JedisPool pool = new JedisPool(new JedisPoolConfig(), "localhost");
try (Jedis jedis = pool.getResource()) {
  /// ... do stuff here ... for example
  jedis.set("foo", "bar");
  String foobar = jedis.get("foo");
  jedis.zadd("sose", 0, "car"); jedis.zadd("sose", 0, "bike"); 
  Set<String> sose = jedis.zrange("sose", 0, -1);
}
```

[JedisPool的默认设置](http://shift-alt-ctrl.iteye.com/blog/1885910)

## redis-py

github: https://github.com/andymccurdy/redis-py

### 连接不需要显示关闭

https://stackoverflow.com/questions/24875806/redis-in-python-how-do-you-close-the-connection

redis客户端实例的大部分操作都是线程安全的，但是PubSub or Pipeline不是线程安全的。另外，连接对象底层使用了连接池。
https://pypi.python.org/pypi/redis

## 高可用

几种方案：
* Sentinel。至少3台机器。只支持在master故障时将某个slave切换为master，不支持slave的高可用，适合较小规模。（Redis开发与运维，P271）
* codis。可以水平扩展，客户端类似单机redis，需要zookeeper。主从切换需要人工介入。
* redis cluster。比较重。具体参考下面的资料。

## mget

https://blog.csdn.net/Jinlu_npu/article/details/79744689

## 普通用户安装 install without root

```
curl -O http://download.redis.io/redis-stable.tar.gz
tar -xzf redis-stable.tar.gz
cd redis-stable
make
make test # optional
src/redis-server
src/redis-cli
```

参考资料：
http://www.infoq.com/cn/articles/effective-ops-part-03
https://github.com/CodisLabs/codis/blob/release3.2/doc/tutorial_zh.md
https://github.com/CodisLabs/codis/blob/release3.2/doc/FAQ_zh.md
http://weibo.com/ttarticle/p/show?id=2309403956185182119232

