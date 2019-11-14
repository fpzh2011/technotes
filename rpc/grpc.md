# grpc笔记

https://github.com/grpc/grpc/tree/master/doc
https://grpc.io/

https://developers.google.com/protocol-buffers/docs/overview

## gRPC Concepts

https://grpc.io/docs/guides/concepts.html
https://doc.oschina.net/grpc?t=56831

Deadlines/Timeouts 单次调用超时
https://www.grpc.io/docs/guides/concepts/#deadlines
https://grpc.io/blog/deadlines/

### channel

Channel Connectivity 连通性
这里讲述了channel几个状态的详细定义。
https://github.com/grpc/grpc/blob/master/doc/connectivity-semantics-and-api.md
https://grpc.github.io/grpc/python/grpc.html#channel-connectivity

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

###　compile

```shell
protoc --java_out=src helloworld.proto
python -m grpc_tools.protoc -I../java/src/main/proto --python_out=. --grpc_python_out=. helloworld.proto
```
例子中的`GreeterGrpc`不是protoc生成，而是[maven/gradle](https://github.com/caio/grpc-java-gradle-hello-world/issues/1)插件生成的。

### python server and client

https://grpc.io/docs/tutorials/basic/python/

protobuf的map类型，在go server中返回nil，在python client中仍可以通过iteritems()访问。其实python client中不是dict，而是一个实现了dict协议的对象。

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
	-skipTLS \
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

## load balance

https://grpc.io/blog/loadbalancing
https://github.com/grpc/grpc/blob/master/doc/load-balancing.md
几种选择：
* 负载均衡server，如SLB，需要配置ip
* client自己做负载均衡，无法感知服务器的负载
* nginx/envoy。灵活。
* docker dns。量少时负载不均衡。

k8s就不需要proxy吗？为什么？

### grpc-lb

* [zookeeper](https://github.com/makdharma/grpc-zookeeper-lb)
* etcd

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

## 疑问

如果很大的流量，只用一条grpc连接，是否合适？

### Connection reset by peer

channel包含两种状态：connected and idle

grpc.max_connection_age_ms
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


docker-compose build proxy
docker stack rm grpc
docker stack deploy -c docker-compose.yml grpc --resolve-image never
docker stack ps grpc | grep -v Shutdown
curl -v -X POST http://localhost:8000/api/data/mdm/m123
curl -X POST http://localhost:8000/api/data/mdm/m123










