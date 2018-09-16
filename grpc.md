# grpc笔记

https://github.com/grpc/grpc
https://grpc.io/

https://developers.google.com/protocol-buffers/docs/overview

## gRPC Concepts

https://grpc.io/docs/guides/concepts.html
https://doc.oschina.net/grpc?t=56831

## protoc

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

### proto example

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

## keepalive

https://github.com/grpc/grpc/blob/master/doc/keepalive.md

## load balance

https://grpc.io/blog/loadbalancing
https://github.com/grpc/grpc/blob/master/doc/load-balancing.md

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

## 注意事项

[grpc-java](https://github.com/grpc/grpc-java/blob/master/examples/pom.xml)中的jar包版本号要去掉`SNAPSHOT`。





docker-compose build proxy
docker stack rm grpc
docker stack deploy -c docker-compose.yml grpc --resolve-image never
docker stack ps grpc | grep -v Shutdown
curl -v -X POST http://localhost:8000/api/data/mdm/m123
curl -X POST http://localhost:8000/api/data/mdm/m123










