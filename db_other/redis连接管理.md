# 代码版本

以下分析、测试的环境基于：
* Python 2.7.16
* [redis-py](https://github.com/andymccurdy/redis-py) 3.2.1 或 2.10.6

# Redis客户端、连接与连接池

## Redis的常规使用

Python中使用redis通常是创建一个`Redis`对象，这个`Redis`对象是一个client，通过client直接执行redis命令、获取结果。
`Redis` client对象内部有一个连接池`ConnectionPool`。可以在构造`Redis` client时传入预先创建的pool，这样多个client可以共享一个pool；否则，`Redis` client就自己创建一个pool。（以上可以参考`client.py`中的`Redis.__init__()`。）

`Redis` client中没有`close/reset/disconnect`之类的方法。它只是一个持有pool的client。在`Redis.execute_command()`中，先从pool拿一个连接，然后执行命令、解析结果，最后通过`finally`确保连接被放回pool。在这种模式下，应用代码不用关心连接的释放，只执行命令、处理结果就好。一般来说，不太容易出现连接泄漏问题。

## redis的线程安全性

`Redis` client是[线程安全的](https://github.com/andymccurdy/redis-py#thread-safety)；除了`select`命令，因为它要切换数据库。client并不存储redis命令相关的状态。
就实现来看，`Redis` client的线程安全性是基于gil锁以及`list/set`这些内置对象的线程安全性。（参考`connection.py`中`ConnectionPool.get_connection()`）

但是`PubSub/Pipeline`对象不是线程安全的。

如果是多线程环境，可以考虑预先创建一个pool，各个线程创建自己的`Redis` client、复用预先创建的pool。

## 多进程问题

在[redis-py](https://github.com/andymccurdy/redis-py)的实现中，每个进程最终都会持有自己独立的连接池，以免多个进程同时操作一个socket、出现异常现象。
如果父进程中已经创建了连接池，子进程的pool在获取新连接，或者将连接放回连接池时，都会检查连接池缓存的pid与当前pid是否一致。如果pid不一致，pool会清空父进程创建的所有连接，后续执行命令时重新创建自己的连接。（参考参考`connection.py`中`ConnectionPool.get_connection()`和`release()/_checkpid()`）

## 控制连接池的最大连接数

`ConnectionPool`中的连接一旦创建就不会释放，除非检测到网络异常。而每个连接池的默认最大连接数几乎是无限的。（默认是`2**32`，参考`connection.py`中`ConnectionPool.__init__()`）
如果一个服务创建了大量python进程应对并发问题，每个进程都有一个独立的、数量几乎无限的连接池。如果很多进程都处理过瞬间流量爆发（结合gevent/threading），可能会占用大量redis连接而一直不释放。

为避免单个服务占用大量redis连接、影响其它服务，可以设置连接池的`max_connections`。这样在并发很多、占用的连接超过上限后会直接抛出异常。

需要注意的是，`redis-py 2.10.6`并不支持通过`from_url()`方式设置`max_connections`（高版本支持），但可以通过`json/dict`设置。

# Redis连接的其它参数

* `socket_keepalive`。根据操作系统参数定时发送tcp keepalive包。如果操作系统的keepalive间隔太长，可以根据实际需要调整`socket_keepalive_options`。
* `socket_connect_timeout`。建立tcp连接的超时。
* `socket_timeout`。执行redis命令时的tcp读写超时。这是在cpython中自己实现的。以上参考`Connection._connect()`，以及[sock_call_ex/sock_sendall](https://github.com/python/cpython/blob/master/Modules/socketmodule.c)等函数。
* `retry_on_timeout`。如果设置`retry_on_timeout=True`，在出现网络异常时，可以重新连接、执行redis命令。参考`Redis.execute_command()`。

# 参考资料

https://docs.python.org/2/library/socket.html#socket.socket.settimeout
https://www.douban.com/note/174791641/
http://www.chengweiyang.cn/2017/02/18/linux-connect-timeout/
http://www.blogjava.net/yongboy/archive/2015/04/14/424413.html
http://hengyunabc.github.io/why-we-need-heartbeat/socket_connect_timeout 也需要设置，避免连接时间太长
/proc/sys/net/ipv4/tcp_keepalive_time tcp_keepalive_probes tcp_keepalive_intvl

# 问题

redis keepalive的具体作用是什么？？？？如果socket断开了，conn的状态是什么，什么时候能感知到？？？？测试一下。conn是否就不再重试连接？