# grpc C++ server threads

grpc版本：1.19.1
如果没有特别说明，都是针对sync server。

`Server`继承自：
* ServerInterface
* GrpcLibraryCodegen: 主要是grpc lib初始化

## ServerInterface: Models a gRPC server

include/grpcpp/impl/codegen/server_interface.h

### Shutdown

调用Shutdown后，效果如下：
* 连接不会立即断开，但是不会再接受新请求，直接返回失败状态码
* 已经放到completion queue、但还没进入handler的请求，貌似也会返回失败状态码
* 在deadline到达之前，已经进入handler处理的请求，会继续处理、返回结果
* 在deadline到达之后，所有请求都返回失败状态码
* stream的长期处理可能会阻止shutdown？


## Server

在Server构造函数中可以看到，如果没有定制`ResourceQuota`，那么sync server最大线程数`DEFAULT_MAX_SYNC_SERVER_THREADS`是`INT_MAX`，实际就是没有限制。

对于sync server的ServerCompletionQueue，每个queue对应一个`SyncRequestThreadManager`。

https://stackoverflow.com/questions/52298811/c-grpc-thread-number-configuration
ResourceQuota::SetMaxThreads

相关代码：
* include/grpcpp/server.h
* src/cpp/server/server_cc.cc

## sync server

sync server在grpc内部有一个线程池处理请求。
https://github.com/grpc/grpc/issues/9728

可以用`ResourceQuota`控制线程池数量：
https://stackoverflow.com/questions/52298811/c-grpc-thread-number-configuration

## async server

如何实现多个线程处理HandleRpcs？
如果没有显示指定多线程，会用grpc内部的线程池吗？
可以和folly的future/wangle线程池结合？

async server + folly，应该可以实现类似vert.x的效果。
