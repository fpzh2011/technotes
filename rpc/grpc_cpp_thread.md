# grpc C++ server threads

grpc版本：1.19.1
如果没有特别说明，都是针对sync server。

grpc sync server启动后(builder.BuildAndStart())，通过gdb，可观察到如下线程：
* main: 主线程，与可执行程序同名
* default-executor: src/core/lib/iomgr/executor.cc
* resolver-executor: src/core/lib/iomgr/executor.cc
* grpc_global_timer: src/core/lib/iomgr/timer_manager.cc
* grpcpp_sync_server: src/cpp/thread_manager/thread_manager.cc

## sync vs async

https://stackoverflow.com/questions/51764721/grpc-cpp-async-server-vs-sync-server
https://groups.google.com/forum/#!topic/grpc-io/DuBDpK96B14
The async model give the application code full control over threading. Sync can handle multiple requests in parallel, yes, but they are serviced from a gRPC thread pool. If you want finer control over threading, then async is the better model to use.

To provide a slight implementation detail - Both the async and sync models of gRPC C++ end up using completion queues. The difference is that, for the sync case, the completion queue handling is hidden from the application, while in the async case, the application has complete control over how and when the completion queues are run.

## logging

设置logging
```shell
GRPC_VERBOSITY=INFO ./program
```
include/grpc/impl/codegen/log.h

## grpc::ServerBuilder.BuildAndStart()

核心代码：
* 声明 include/grpcpp/server_builder.h
* 定义 src/cpp/server/server_builder.cc

主要完成如下工作：
* 设置channel属性
* CompletionQueue。判断如果是sync server，就创建一个vector<ServerCompletionQueue> sync_server_cqs，to listen for incoming requests
  * xxx.grpc.pb.cc中，有grpc::ServiceAddMethod的调用，默认是SYNC
  * 如果是sync server，添加的是ServerCompletionQueue(GRPC_CQ_NEXT, GRPC_CQ_DEFAULT_POLLING, nullptr))
  * ServerBuilder::SyncServerSettings: 构造函数中设置了默认值
* 创建`Server`实例:
  * include/grpcpp/server.h
  * src/cpp/server/server_cc.cc

在Server构造函数中可以看到，如果没有定制`ResourceQuota`，那么sync server最大线程数`DEFAULT_MAX_SYNC_SERVER_THREADS`是`INT_MAX`，实际就是没有限制。




