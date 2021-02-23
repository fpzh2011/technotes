# grpc C++ library init

grpc版本：1.19.1

https://zhuanlan.zhihu.com/p/63815417
https://zhuanlan.zhihu.com/p/63815505
https://jiangew.me/grpc-03/
https://cloud.tencent.com/developer/article/1416302
https://www.iteye.com/blog/shift-alt-ctrl-2292862
https://youyou-tech.com/2019/12/27/GRPC-C%2B%2B%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%EF%BC%88%E4%B8%80%EF%BC%89-%E7%BD%91%E7%BB%9C%E6%A8%A1%E5%9E%8B/

## 代码分析

https://zhuanlan.zhihu.com/p/63815292
java
https://jiangew.me/grpc-03/

## grpc_init的来源

函数声明：
include/grpc/grpc.h

函数定义：
src/core/lib/surface/init.cc 

`grpc_init`对grpc库进行初始化。它是怎么被调用的呢？
* `GrpcLibraryCodegen`的构造函数中会调用`g_glip->init()`: include/grpcpp/impl/codegen/grpc_library.h
  * Classes that require gRPC to be initialized should inherit from this class. 子类包括（创建这些类的实例就会init）：
    * include/grpcpp/alarm_impl.h
    * include/grpcpp/channel.h
    * include/grpcpp/resource_quota.h
    * include/grpcpp/server.h
    * include/grpcpp/impl/codegen/completion_queue.h
    * include/grpcpp/security/credentials.h
    * src/cpp/client/secure_credentials.h (MetadataCredentialsPluginWrapper)
  * 一些函数内也会创建`GrpcLibraryCodegen`实例，从而执行init：
    * `CreateCustomChannel`: src/cpp/client/create_channel.cc
    * `GoogleDefaultCredentials`等: src/cpp/client/secure_credentials.cc
* `extern GrpcLibraryInterface* g_glip`: include/grpcpp/impl/codegen/grpc_library.h
* `g_glip`在`GrpcLibraryInitializer`的构造函数中被初始化: include/grpcpp/impl/grpc_library.h
  * `g_core_codegen_interface`的作用是什么？??????????????????????????????????????????????????????
  * 以下文件中有`static internal::GrpcLibraryInitializer`静态变量定义：
    * src/cpp/client/channel_cc.cc
    * src/cpp/client/client_context.cc
    * src/cpp/client/credentials_cc.cc
    * src/cpp/client/secure_credentials.cc
    * src/cpp/common/alarm.cc
    * src/cpp/common/completion_queue_cc.cc
    * src/cpp/server/server_cc.cc
    * src/cpp/util/byte_buffer_cc.cc
    * src/cpp/codegen/codegen_init.cc 只定义指针，包括`CoreCodegenInterface`
* `g_glip`被初始化为`GrpcLibrary`实例
  * `init`函数调用`grpc_init`

相关代码：
include/grpc/grpc.h
src/core/lib/surface/init.cc
src/cpp/common/core_codegen.cc
include/grpcpp/impl/grpc_library.h
include/grpcpp/impl/codegen/grpc_library.h

## 同步相关

出于跨平台的原因，grpc自己定义了一些同步相关的类型（以posix为例）：
* gpr_mu: pthread_mutex_t
* gpr_cv: pthread_cond_t
* gpr_once: pthread_once_t
posix的定义在 include/grpc/impl/codegen/sync_posix.h
https://www.jianshu.com/p/a69745fc0a44

## grpc_init的具体作用

* `do_basic_init`会调用`grpc_register_built_in_plugins`，这里会执行一些插件的初始化，包括：
  * deadline
  * load balance
  * ares resolver
  * max age
* `grpc_iomgr_init`(src/core/lib/iomgr/iomgr.cc): 主要是平台相关设置，如全局变量、vtable。调用栈如下
  * `grpc_determine_iomgr_platform`: src/core/lib/iomgr/iomgr_internal.cc
    * `grpc_set_default_iomgr_platform`: src/core/lib/iomgr/iomgr_posix.cc
      * `grpc_set_iomgr_platform_vtable`设置全局变量`iomgr_platform_vtable`: src/core/lib/iomgr/iomgr_internal.cc
  * `grpc_core::Executor::InitAll()`: src/core/lib/iomgr/executor.cc 启动以下两个线程：
    * resolver-executor: dns 解析。如 src/core/lib/iomgr/resolve_address_posix.cc
    * default-executor: 其它io任务貌似都是由default-executor执行，包括tcp/udp读写等。
  * `grpc_iomgr_platform_init`: src/core/lib/iomgr/iomgr_internal.cc
* grpc_iomgr_start: src/core/lib/iomgr/iomgr.cc 调用栈如下：
  * grpc_timer_manager_init: src/core/lib/iomgr/timer_manager.cc
    * start_threads
      * start_timer_thread_and_unlock: 这里会创建并启动线程`grpc_global_timer`。相关对象在heap中，没有全局或静态变量持有这些对象。这个线程的具体作用还不清楚。

## vtable 跨平台实现

以completion queue为例，其操作依赖大量vtable设置，具体是怎么初始化的呢？

vtable的实例是 `grpc::CoreCodegenInterface* grpc::g_core_codegen_interface`
* 变量定义: src/cpp/codegen/codegen_init.cc
* 类型声明: include/grpcpp/impl/codegen/core_codegen_interface.h
* 全局变量初始化: include/grpcpp/impl/grpc_library.h
* `CoreCodegen`类型定义: include/grpcpp/impl/codegen/core_codegen.h
* `CoreCodegen`类型实现: src/cpp/common/core_codegen.cc

