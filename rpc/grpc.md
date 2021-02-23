# grpc笔记

问题：
* 为什么实际deadline会远高于设置的值？偶发的。

概述
https://grpc.io/

这个下面有更详细、深入的文档，包括各个子模块
https://github.com/grpc/grpc/tree/master/doc

GRPC C++
https://grpc.io/docs/tutorials/basic/cpp/
https://grpc.github.io/grpc/cpp/index.html
https://grpc.github.io/grpc/cpp/pages.html

https://developers.google.com/protocol-buffers/docs/overview

## gRPC Concepts

https://grpc.io/docs/guides/concepts.html
https://doc.oschina.net/grpc?t=56831

### Deadlines/Timeouts 单次调用超时

deadline应该是通过HTTP2 headers实现的
https://github.com/grpc/grpc/blob/master/doc/PROTOCOL-HTTP2.md

Always set a deadline. 默认的timeout一般都很大。C++/Go/Java设置deadline的方法。
https://grpc.io/blog/deadlines/
https://www.grpc.io/docs/guides/concepts/#deadlines

deadline是一个时间点，timeout是一段时间（比如3秒）
https://github.com/grpc/grpc-java/issues/1495

超时远高于deadline
* network connectivity: iptables, elb restart
* the size of the message we were trying to send was relevant
* the trigger for the blocking was the TCP send buffer filling up
* It will stay that way until either connectivity is restored, or the kernel gives up on that particular connection, closing it.
* 基本解决方案
  * keepalive + TCP_USER_TIMEOUT 由原来的block 15-20分钟，降低到block 2分钟
    * TCP_USER_TIMEOUT比超时时间略大一些
  *  tcp_syn_retries
https://www.hostedgraphite.com/blog/deadlines-lies-and-videotape-the-tale-of-a-grpc-bug
https://news.ycombinator.com/item?id=18382149

### channel

Channel Connectivity 连通性
这里讲述了channel几个状态的详细定义。
https://github.com/grpc/grpc/blob/master/doc/connectivity-semantics-and-api.md
https://grpc.github.io/grpc/python/grpc.html#channel-connectivity

channel代表一个虚拟连接，它屏蔽了物理连接。通过channel可以实现grpc的4种调用方式，而不必关注底层是如何实现的。channel可以同时处理多个rpc、多个流、多个server实例。
https://grpc.io/blog/grpc_on_http2/

A gRPC channel is created between a client and a 'target'. The 'target' may resolve in to one or more backend servers.
A sub-channel is the 'connection' from a client to the backend server
https://github.com/grpc/grpc/blob/master/doc/core/grpc-client-server-polling-engine-usage.md

设置channel options。backoff应该可以通过options设置。
```python
options = [('grpc.max_message_length', 30 * 1024 * 1024)]
return grpc.secure_channel("127.0.0.1:4000", creds, options=options)
```
https://stackoverflow.com/questions/48170761/grpc-python-server-options

channel包含两种状态：connected and idle
https://www.grpc.io/docs/guides/concepts/#channels
python检查channel状态(但是这个字段启动后一段时间是None)：
```python
ch._connectivity_state.connectivity
```

### Stub

* Stub就是client，它实现了service interface。
* 从C++的视角看，Stub只是基于Channel作了简单封装，成本很低。
* 多个Stub可以复用一个Channel。官方鼓励只使用一个Channel。
* [Stream Stub不是线程安全的](https://groups.google.com/forum/#!topic/grpc-io/1lh14L5ZaEk)
https://stackoverflow.com/questions/47022097/should-i-share-grpc-stubs-or-channels
https://groups.google.com/forum/#!topic/grpc-io/jc-cZak29V4

### 相较于thrift的特点

* 支持4种模式: unary requests; stream...
* http2

## protoc

protobuf 终极教程
https://colobu.com/2019/10/03/protobuf-ultimate-tutorial-in-go/

### download & install

```shell
curl https://github.com/google/protobuf/releases/download/v3.6.0/protoc-3.6.0-linux-x86_64.zip
unzip protoc-3.6.0-linux-x86_64.zip -d protoc3
sudo mv protoc3/bin/* /usr/local/bin/
sudo mv protoc3/include/* /usr/local/include/
# python
sudo pip install grpcio grpcio-tools
```
https://github.com/grpc/grpc/tree/master/src/python/grpcio
https://github.com/grpc/grpc/issues/5738
https://grpc.io/docs/quickstart/python.html
https://halfrost.com/protobuf_encode/

### build

macos下用cmake编译grpc，plugin在grpc/cmake/build/
* grpc_python_plugin
* grpc_cpp_plugin

### proto example

在 proto 中，所有结构化的数据都被称为 message。
servive只能最多有一个输入、一个输出。

```proto
syntax = "proto3";

option java_package = "io.grpc.examples";

package helloworld;

// The greeter service definition.
service Greeter {
  // Sends a greeting
  rpc SayHello (HelloRequest) returns (HelloReply) {}
}

// The request message containing the user's name.
message HelloRequest {
  string name = 1;
}

// The response message containing the greetings
message HelloReply {
  string message = 1;
}
```

### Protobuf3 语法指南

https://colobu.com/2017/03/16/Protobuf3-language-guide/
https://developers.google.com/protocol-buffers/docs/proto3

###　compile

```shell
protoc --java_out=src helloworld.proto
python -m grpc_tools.protoc -I../java/src/main/proto --python_out=. --grpc_python_out=. helloworld.proto
protoc  --cpp_out=. test.proto
```
例子中的`GreeterGrpc`不是protoc生成，而是[maven/gradle](https://github.com/caio/grpc-java-gradle-hello-world/issues/1)插件生成的。

https://grpc.io/docs/tutorials/basic/cpp/

### python server and client

https://grpc.io/docs/tutorials/basic/python/

protobuf的map类型，在go server中返回nil，在python client中仍可以通过iteritems()访问。其实python client中不是dict，而是一个实现了dict协议的对象。

### protoc C++

https://developers.google.com/protocol-buffers/docs/cpptutorial
message的父类
https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message.html#Message
protoc编译生成的代码说明
https://developers.google.com/protocol-buffers/docs/reference/cpp-generated
c++代码示例
https://grpc.io/docs/tutorials/basic/cpp/

protoc C++生成4个文件: 2个header文件，两个cc实现文件。
其中一对是protobuf的数据结构；另一对文件名含grpc的是service stub。
grpc的生成文件中，Stub是client用的。

proto中的package对应C++中的namespace。如果package被`.`分为多节，namespace也是嵌套的多层。
https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#package

序列化和解析 Parsing and Serialization。相关方法在Message的基类MessageLite中。
https://developers.google.com/protocol-buffers/docs/cpptutorial#parsing-and-serialization

如果想给proto对象添加更多行为，最好在此基础上再封装一个类。但最好不要从生成的类继承。
If you want to add richer behaviour to a generated class, the best way to do this is to wrap the generated protocol buffer class in an application-specific class.
You should never add behaviour to the generated classes by inheriting from them. 
另参考 [You should not create your own Foo subclasses.](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated)

mutable:
* 有两种方式修改field，一种是set方法；一种是根据mutable返回的指针修改。如果没有field没有set，mutable返回的是empty。
* mutable既可以修改已经设置的field，比如rpc传入的field；也可以new(如果之前没有set)。它返回的指针不会是nullptr。repeated的mutable不负责new，应该用add方法。
* repeated的field，如果要添加一个元素，可以提前准备好调用add方法，也可以先调用无参数的add方法返回一个指针。

repeated field的iterator，需要调用field同名方法，返回`const RepeatedPtrField<string>&`，或者带mutable前缀的方法，返回没有const修饰的引用。

Clear操作只清除数据，但并不释放占用的内存。
https://my.oschina.net/u/2554847/blog/600682

线程安全：protobuf对象的const操作都是线程安全的。
https://developers.google.com/protocol-buffers/docs/reference/cpp/index.html

protobuf对象可与json string相互转换
https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.util.json_util

如何更新一个protobuf文件/协议：
https://developers.google.com/protocol-buffers/docs/cpptutorial#extending-a-protocol-buffer

C++版本使用[Arena](https://developers.google.com/protocol-buffers/docs/reference/arenas)管理内存分配，以提高效率。在grpc/protoc中，这是C++独有的。

**疑问**：
* CopyFrom: overwrites the message with the given message's values
* proto对象的拷贝构造是用CopyFrom实现的。这两个方法有重载版本，可以接受任意proto类型，所以不够安全。
* MergeFrom的作用是repeated的append方法？有没有其它方法可以append？
* https://blog.csdn.net/wzzfeitian/article/details/69803166

### C++ service code

以Greeter为例。Greeter类有几个嵌套类:
* Service: 继承自`::grpc::Service`，实现具体的api接口。业务需要继承Service并override相关方法。
* StubInterface: client用的接口，纯虚基类。
* Stub: client用的，实现StubInterface。
* AsyncService
* Stream相关的类和方法

对grpc channel进行定制，比如负载均衡load balance LB，可以用ChannelArguments
https://github.com/grpc/grpc/blob/master/examples/cpp/load_balancing/greeter_client.cc

**疑问**:
* 如果继承同步接口Service，但内部实现使用folly::future，如果有多个并发请求，每个请求占用一个线程吗？

### C++ service thread pool

gprc之前用的是DynamicThreadPool。从v1.1开始使用ThreadManager。
https://github.com/grpc/grpc/pull/10919/files
https://stackoverflow.com/questions/52298811/c-grpc-thread-number-configuration
貌似不是每个req一个线程？sync server也是这样吗？
https://github.com/grpc/grpc/blob/master/doc/core/epoll-polling-engine.md
sync server的线程个数，默认是不限制的：DEFAULT_MAX_SYNC_SERVER_THREADS server_cc.cc
SyncRequestThreadManager: sync server也涉及CompletionQueue
但是，CompletionQueue.Next是阻塞操作，所以async也是阻塞吗？或者grpc内部的线程机制保证不会阻塞，只是挂起任务，线程给其它任务使用？
async一定意味着线程复用吗？

https://stackoverflow.com/questions/51764721/grpc-cpp-async-server-vs-sync-server

线程图示
https://www.jianshu.com/p/551e6578f5df

## 健康检查 health check

https://segmentfault.com/a/1190000018362469

## keepalive

https://github.com/grpc/grpc/blob/master/doc/keepalive.md

HTTP 1.0 需要使用 keep-alive 参数来告知服务器端要建立一个长连接，而 HTTP1.1 默认支持长连接。
http://jartto.wang/2018/03/30/grasp-http2-0/

## 调试工具debug

https://github.com/ktr0731/evans
https://github.com/fullstorydev/grpcui
64位整数截断
https://github.com/uw-labs/bloomrpc
https://github.com/uw-labs/bloomrpc/issues/87

## stress test压力测试

https://ghz.sh/
https://github.com/bojand/ghz

```shell
ghz \
	--proto protofile \
	--call package.Service.Method \
	--insecure \
	--skipTLS \
	-config configfile \
	--concurrency 1 \
	--qps 5 \
	--timeout 1s \
	--duration 10s \
	--connections 1 \
	--data "json string request data" \
	--data-file "filename" \
	--output output \
	--format csv \
	--cpus 1 \
	host:port
```

## sync vs. async 同步 异步

C++/Java都提供了异步接口:
https://grpc.io/docs/tutorials/async/helloasync-cpp/
https://grpc.io/docs/tutorials/basic/java/

C++异步Client:
* Finish传入的tag指针，被Next填到我们指定的tag地址(参考头文件中Next的说明)。可以通过这种方式传递数据，或者tag标志。

Go没有异步接口:
https://stackoverflow.com/questions/49244039/
https://github.com/grpc/grpc-go/issues/246

## load balance

https://grpc.io/blog/loadbalancing
https://github.com/grpc/grpc/blob/master/doc/load-balancing.md
通过开放负载均衡接口可以无缝的与第三方组件进行集成对接(Zookeeper、域名解析服务、SLB服务等):
* 负载均衡server，如SLB，需要配置ip。缺点是，暴露给client的只有一个vip，不能保证下游负载均衡质量，尤其是上游数量相对较少时。
* client自己做负载均衡，无法感知服务器的负载，扩容、缩容、健康检查等也需要自己做。
* nginx/envoy。灵活。
* dns/swarm service name。量少时负载不均衡。
* grpc的LB是on a per-call basis

https://juejin.im/entry/59bb30f76fb9a00a616f1b73
客户端负载均衡策略由客户端内置负载均衡能力，通过静态配置、域名解析服务（例如DNS服务）、订阅发布（例如Zookeeper服务注册中心）等方式获取RPC服务端地址列表，并将地址列表缓存到客户端内存中。
gRPC默认采用客户端负载均衡策略，同时提供了扩展机制，使用者通过自定义实现NameResolver和LoadBalancer，即可覆盖gRPC默认的负载均衡策略，实现自定义路由策略的扩展。
gRPC系统默认提供的是DnsNameResolver。
对于扩展者而言，可以继承NameResolver实现自定义的地址解析服务，例如使用Zookeeper替换DnsNameResolver，把Zookeeper作为动态的服务地址配置中心。
gRPC提供的负载均衡策略实现类如下所示:
* PickFirstBalancer：无负载均衡能力，即使有多个服务端地址可用，也只选择第一个地址。
* RoundRobinLoadBalancer：“RoundRobin”负载均衡策略。

**底层连接的重要资料：**
即使只有1个实例，也可以创建多个连接？
a user who wants multiple connections but knows that the host name will only resolve to one address might have their balancer create connections against each address 10 times to ensure that multiple connections are used.
Once configured, gRPC will keep the pool of connections - as defined by the resolver and balancer - healthy, alive, and utilized.
When a connection fails, the load balancer will begin to reconnect using the last known list of addresses [3]. Meanwhile, the resolver will begin attempting to re-resolve the list of host names. 
https://grpc.io/blog/grpc_on_http2/
如果没有conn fail，dns多久更新一次cache？

### k8s环境下的负载均衡

如果client与server是同一个集群：
* server使用headless，一次返回所有pod ip
* 配置coredns使得查询结果在30秒内失效
* 需要配置resolve/dns的dot，优化外网域名解析

如果client与server在不同集群：
* 在server集群部署ingress网络，有多种负载均衡策略，可以在proxy层实现统计监控、重试、熔断等
* 如果集群创建时合理设置cluster.local等dns配置，跨集群貌似也可以用类似headless方案
* etcd：需要改server和client代码。server启动时注册etcd、退出时注销。client通过etcd获取服务地址。
* sidecar/service mesh

ingress + envoy跨集群:
* client只有一个proxy的连接。
* 如果有两个client，server有两个连接，应该是和client数量一致。
* envoy跟每个client和server都有连接。连接数是`n*m`，相当于用空间换效率。
* 如果在服务的envoy实例挂掉，会自动切到其它实例，有少量错误。估计envoy默认没有grpc那样的graceful exit机制。

cps: 每秒新建连接速率
rps: 每秒交易速率

### etcd

https://github.com/etcd-io/etcd/pull/9860
https://github.com/DavadDi/grpclb
https://github.com/liyue201/grpc-lb
https://colobu.com/2017/03/25/grpc-naming-and-load-balance/

### grpc dns resolve

C++
* dnnrr: docker需要在一个集群内
* server: max age
* client: custom channel, round robin load balance

https://gist.github.com/lyuxuan/515fa6da7e0924b030e29b8be56fd90a
Dial接受一系列的DialOption。还可以设置默认的CallOption列表。
```go
conn, err := grpc.Dial("server", grpc.WithInsecure(), grpc.WithBalancerName("round_robin"))
```

LB相关的DialOption:
* WithBalancer: Deprecated
* WithBalancerName: Deprecated
* withResolverBuilder: only for grpclb
* WithServiceConfig: Deprecated
* WithDefaultServiceConfig
* 默认的健康检查是defaultDialOptions.healthCheckFunc
* WithDisableRetry: 默认是true，即disableRetry

C++直接通过域名指定：
https://github.com/grpc/grpc/blob/master/examples/cpp/load_balancing/greeter_client.cc
https://grpc.github.io/grpc/cpp/md_doc_naming.html
https://grpc.github.io/grpc/cpp/md_doc_environment_variables.html
环境变量GRPC_DNS_RESOLVER指定dns方式，默认是ares。只是还不清楚dns更新周期。

### grpc-lb

* [zookeeper](https://github.com/makdharma/grpc-zookeeper-lb)
* etcd

### local subchannel pool

如下代码可以设置Channel使用local channel pool，而不是global。这样创建多个channel，就可以创建多个物理连接。
貌似目前只有Core支持(c/c++/python?)，而且版本要求1.19+ ？
```cpp
grpc::ChannelArguments channel_args;
channel_args.SetInt(GRPC_ARG_USE_LOCAL_SUBCHANNEL_POOL, 1);
channel = grpc::CreateCustomChannel(address, grpc::InsecureChannelCredentials(), channel_args);
```
https://github.com/grpc/grpc/blob/master/src/core/ext/filters/client_channel/local_subchannel_pool.h
https://stackoverflow.com/questions/53564748/how-do-i-force-my-grpc-client-to-open-multiple-connections-to-the-server
https://grpc.github.io/grpc/core/group__grpc__arg__keys.html
src/core/ext/filters/client_channel/global_subchannel_pool.cc

### Proxy

* [Envoy](https://blog.bugsnag.com/envoy/)
* [nginx](https://www.nginx.com/blog/nginx-1-13-10-grpc/)需要配置每个server的地址
https://github.com/ryanolson/grpc-envoy-loadbalancer

### Envoy

Envoy+dnsrr
* 第一次请求过后，Envoy只创建一个grpc server连接。后续会创建多个连接。
* grpc连接中断后会自动重连，但中间可能会有个别请求报错。
* 新增节点后，Envoy可自动创建新连接。
* 如果有节点下线，Envoy中的tcp连接会自动断开。

sidecar
http://senlinzhan.github.io/2017/12/25/envoy/
使用 Envoy 作为前端代理 是否可复制为grpc？
https://jimmysong.io/posts/envoy-as-front-proxy/
ServiceMesh 数据面板 Envoy 简介
https://www.opsdev.cn/post/envoy.html
示例配置文件的注释
https://github.com/imjoey/blog/issues/26
Envoy设计分析
http://jm.taobao.org/2018/07/05/Mesh%E4%BD%93%E7%B3%BB%E4%B8%AD%E7%9A%84Envoy/
Envoy简介、术语介绍
https://juejin.im/post/5ad6fb06518825364001f619
中文文档
http://www.servicemesher.com/envoy/
英文文档
https://www.envoyproxy.io/docs/envoy/latest/intro/arch_overview/grpc

## vertx

https://vertx.io/docs/vertx-grpc/java/

## 错误处理

这里有错误处理的示例代码，server可以设置、client可以检查StatusCode等信息。
在某些场景下返回标准的grpc错误代码，应该是比较好的做法。比如参数数据错误，就不必重试了。
但是似乎应该有一个单独字段表示成功失败。
https://github.com/avinassh/grpc-errors
https://www.grpc.io/docs/guides/error/
HTTP to gRPC Status Code Mapping
https://github.com/grpc/grpc/blob/master/doc/http-grpc-status-mapping.md
Status codes and their use in gRPC
https://github.com/grpc/grpc/blob/master/doc/statuscodes.md

https://grpc.github.io/grpc/python/grpc.html#grpc-status-code
https://github.com/grpc/grpc/tree/master/examples/python/errors
https://discuss.dgraph.io/t/rendezvous-of-rpc-that-terminated-with-statuscode-unavailable-error-received-from-peer/3855
https://github.com/grpc/grpc/issues/16515

## 注意事项

[grpc-java](https://github.com/grpc/grpc-java/blob/master/examples/pom.xml)中的jar包版本号要去掉`SNAPSHOT`。

## 其它

各个语言的stack
https://grpc.io/blog/grpc-stacks/

流行的rpc框架benchmark
https://colobu.com/2018/01/31/benchmark-2018-spring-of-popular-rpc-frameworks/

## 错误信息

### Connection reset by peer

channel包含两种状态：connected and idle

这种情况应该是connection idle超时关闭，增加重试应该就可以。
grpc.max_connection_age_ms (python)
https://stackoverflow.com/questions/46570634/can-a-grpc-client-connect-timeout-be-set-independent-of-reconnect-backoff-settin
https://github.com/grpc/grpc-node/issues/154
https://github.com/grpc/grpc-go/issues/2130
```
_Rendezvous: <_Rendezvous of RPC that terminated with:
	status = StatusCode.UNAVAILABLE
	details = "Connection reset by peer"
	debug_error_string = "{"created":"@1567479737.421906245","description":"Error received from peer ipv4:172.17.9.125:8787","file":"src/core/lib/surface/call.cc","file_line":1046,"grpc_message":"Connection reset by peer","grpc_status":14}"
```

### grpc server重启时，client可能会报如下错误

```
_Rendezvous: <_Rendezvous of RPC that terminated with:
	status = StatusCode.UNAVAILABLE
	details = "failed to connect to all addresses"
	debug_error_string = "{"created":"@1567679373.492083686","description":"Failed to pick subchannel","file":"src/core/ext/filters/client_channel/client_channel.cc","file_line":3381,"referenced_errors":[{"created":"@1567679373.492034611","description":"failed to connect to all addresses","file":"src/core/ext/filters/client_channel/lb_policy/pick_first/pick_first.cc","file_line":453,"grpc_status":14}]}"
```

## 疑问

如果很大的流量，只用一条grpc连接，是否合适？
channel和conn的关系是什么？为什么再封装一层channel？

需要通过channel->GetState(true)主动检查状态吗？如果物理连接断开，是否会自动重试连接？

运维触发的网络闪断，比如LB切换，有没有什么好的解决办法，减轻grpc长连接的影响。整个系统很多服务都会有影响，可能会有链式反应。
重试一次是否可以解决问题？结合grpc的LB策略。如果SLB切换，多长时间能获取下一个有效地址？

c++ server中，每个请求一个thread？可以测试一下，每个请求都sleep，然后用ps thread查看。
如果请求失败，client重试的时候，还会请求失败的server实例吗？
如果用了folly，是否也每个请求一个线程？

C++线程:
* 如果不指定folly executor，那么是使用grpc的线程池？这有什么影响？即使用folly的executor，最后fut.get时仍会阻塞？其它请求仍无法处理？
* 如果用同步client，每个请求阻塞1小时，验证是否每个请求占用一个线程
* 如果用异步客户端，每个请求阻塞1小时，是否每个请求占用一个线程？
* server也可以进行同样的测试。
* server shutdown后5秒，client继续请求，得到的是什么状态、信息？
* grpc server收到一个请求q1后，应该会占用一个线程t1；在这请求q1处理完之前，线程t1还能继续处理其它请求吗？如果不能，vertx的意义何在？如果可以，如何验证？
  * 如果只是测folly线程，可以这样：从stdin读取一行，然后创建一个future，看是否在另一个线程、或者看在返回前能否接受第二个行输入。
  * !!!!!!!1 打印main线程id，executor线程id，看是否一致。wangle可以查看队列长度。
  * 一种测试手段：打印grpc server线程id，同时打印folly future线程id、wangle线程id。vertx也可以用这种方法测试。
  * C++中能否得到线程编号，grpc server线程中打印这个线程编号，sleep一段时间
  * 限制docker的内存，多少个请求可以把服务打爆？线程一只sleep，线程太多导致内存满
  * C++ server的worker线程个数是不是有限的？如果是，那么后续的client请求会超时。
  * 能否限制worker线程个数。比如限制到1个，就可以验证异步任务下，t1线程是否会继续处理其它线程。如果是grpc server c++同步方式，估计不会。
  * 如果是同步c++ server，在server内使用异步操作，是否还有意义？作用何在？如果需要处理多个子任务，用folly future是有意义的。否则每个子任务需要单独的线程。
  * 如果server在q1结束前就处理其它请求了，那么q1的栈及其对象还存在吗，这是如何处理的？这个应该是completion queue实现的吧？java是如何处理的？
  * 以上也要看异步completion queue是如何处理请求的。
  * 如果folly的Future一开始就via了一个线程池，是否会出让grpc server线程？如何出让？如果future阻塞，新请求会因为线程被占用而阻塞吗，可以在service里面收到请求时检查。
  * 本方法的一个常见的用例是将 CPU 从 I/O 线程中解放出来，以避免队列中其他请求的排队时间。   代码哪里能看到队列呢？ https://yq.aliyun.com/articles/115416
  * 当前版本的folly future，如何捕获中间某个io操作的异常并进行统计？
  * itoi/utoi是否用了异步server？
  * 以上思路，也可以用go或者vertx试验。
https://stackoverflow.com/questions/52298811/c-grpc-thread-number-configuration
  auto stats = common::getGlobalCPUExecutor()->getPoolStats();
  common::Stats::get()->AddMetric(
    "api_executor_pool_length", stats.pendingTaskCount)


graceful shutdown (c++/go/python/java)
最简单的方式：可以结合k8s preStop ，在stop命令中sleep一段时间以便处理完现有请求。这段时间因为pod处于Terminating不会接受新请求。
mixer因为是http server、短连接，通过sleep延迟退出效果较好（容器收到信号后不会有新连接过来）
grpc是长连接，即使从sld摘除，物理连接仍在。server需要主动shutdown、以通知client连接已经断开，这样可以减少client调用错误。但实际观察，仍会有少量错误，具体原因待核实。
https://stackoverflow.com/questions/35708348/grpc-what-is-the-recommended-way-to-shut-down-an-asynchronous-server-in-c
https://github.com/grpc/grpc-go/issues/147
https://pracucci.com/graceful-shutdown-of-kubernetes-pods.html

TODO:
* 在swarm集群内，dnsrr网络，用c++验证lb



