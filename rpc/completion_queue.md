# grpc C++ server threads

grpc版本：1.19.1
如果没有特别说明，都是针对sync server。

## CompletionQueue

`CompletionQueue`是线程安全的。sync server在grpc内部有一个线程池处理请求。
https://github.com/grpc/grpc/issues/4486
https://github.com/grpc/grpc/issues/9728

sync server的 CompletionQueue ，是在 BuildAndStart 中通过new ServerCompletionQueue 添加到 sync_server_cqs 。
ServerCompletionQueue 继承自 CompletionQueue ，代码在：
* include/grpcpp/impl/codegen/completion_queue.h
* src/cpp/common/completion_queue_cc.cc
BuildAndStart 中调用的是 ServerCompletionQueue 的私有构造函数（friend class）。各个字段取值如下：
* completion_type: GRPC_CQ_NEXT
* polling_type: 如果是sync server， GRPC_CQ_DEFAULT_POLLING ；否则 GRPC_CQ_NON_POLLING

从`sync_server_settings_`的初始化看，默认只创建一个ServerCompletionQueue。

ServerCompletionQueue 构造时，调用的是 CompletionQueue(grpc_completion_queue_attributes) 的构造函数，在这个构造函数中，通过 g_core_codegen_interface 给`CompletionQueue* cq_`赋值。
( grpc_completion_queue_attributes 在grpc_types.h中定义，是一个简单的struct。)

g_core_codegen_interface 是一个全局变量，类型是 CoreCodegenInterface 的子类 CoreCodegen ：
* 在src/cpp/codegen/codegen_init.cc中定义
* 在include/grpcpp/impl/grpc_library.h的 GrpcLibraryInitializer 的构造函数中初始化，在这里可以看到其具体类型是 CoreCodegen
而 GrpcLibraryInitializer 在多处有静态实例初始化，如：
* src/cpp/client/channel_cc.cc
* src/cpp/server/server_cc.cc
* src/cpp/common/completion_queue_cc.cc

CoreCodegenInterface 的代码在 include/grpcpp/impl/codegen/core_codegen_interface.h
都是纯虚函数声明。

CoreCodegen 的代码：
* include/grpcpp/impl/codegen/core_codegen.h
* src/cpp/common/core_codegen.cc
* grpc_completion_queue_factory_lookup: 返回一个工厂指针

### CompletionQueue factory

上述factory指针的实例是 src/core/lib/surface/completion_queue_factory.cc 中的 g_default_cq_factory 。
其最重要的内容在同一文件的变量 default_vtable 中，其类型是 grpc_completion_queue_factory_vtable 。这个struct只有一个名为 create 的函数指针变量，该函数返回 grpc_completion_queue 指针，这是真正的 CompletionQueue （这样说不准确，其实内容更多）。
上述两个类型的声明都在 src/core/lib/surface/completion_queue_factory.h

g_default_cq_factory 主要设置两个字段：
* completion_type: GRPC_CQ_NEXT
* polling_type: 如果是sync server， GRPC_CQ_DEFAULT_POLLING ；否则 GRPC_CQ_NON_POLLING

### 从构造函数到工厂

还是回到 CompletionQueue 的private构造函数：
* 先拿到一个factory指针，实际就是 g_default_cq_factory 的地址
* 将 g_default_cq_factory 的地址传给函数 grpc_completion_queue_create (src/cpp/common/core_codegen.cc)
* 再将 g_default_cq_factory 的地址传给函数 grpc_completion_queue_create (src/core/lib/surface/completion_queue_factory.cc)
* 在这里调用 `factory->vtable->create(factory, attr)`
  * factory就是 g_default_cq_factory 的地址
  * vtable就是 default_vtable ，它唯一的成员变量 create 实际是 default_create ，是实际创建 grpc_completion_queue 的函数指针
* 调用 grpc_completion_queue_create_internal (src/core/lib/surface/completion_queue.cc)
  * 返回值就赋值给 `CompletionQueue._cq`

### grpc_completion_queue_create_internal

这个函数实际执行 grpc_completion_queue 的创建。
这个函数的定义在 src/core/lib/surface/completion_queue.cc 。几个重要的数据结构也在这个文件中，包括：
* grpc_completion_queue (281行): 数据结构定义放到cc源文件的目的可能是为隐藏数据结构细节。
* 调用`gpr_zalloc`分配内存的时候可以看到，实际创建的对象，比`grpc_completion_queue`多了两部分：
  * cq_next_data
  * grpc_pollset: 定义在 src/core/lib/iomgr/ev_epollex_linux.cc （实现之一）
* g_cq_vtable: 引用的函数定义都在该 completion_queue.cc 文件中。以`GRPC_CQ_NEXT`为例，各个字段对应的函数如下（这些函数都在completion_queue.cc中）：
  * init: cq_init_next
  * shutdown: cq_shutdown_next
  * destroy: cq_destroy_next
  * begin_op: cq_begin_op_for_next
  * end_op: cq_end_op_for_next
  * next: cq_next
  * pluck: nullptr
* g_poller_vtable_by_poller_type (src/core/lib/iomgr/ev_epollex_linux.cc的`vtable`): 
  * 引用的函数声明在 src/core/lib/iomgr/pollset.h 定义在 src/core/lib/iomgr/pollset.cc
  * 这些函数都通过 grpc_pollset_impl 调用，其实例也在 pollset.cc 中定义。有多种平台实现，如 src/core/lib/iomgr/iomgr_posix.cc
  * grpc_pollset_impl 的值在 src/core/lib/iomgr/iomgr_posix.cc 中通过调用`grpc_set_default_iomgr_platform`设置
  * `grpc_pollset_impl`其实就是`grpc_posix_pollset_vtable`，在 src/core/lib/iomgr/ev_posix.cc 中定义，函数也都在同一文件中
  * ev_posix.cc 的函数都依赖全局变量`g_event_engine`
  * `g_event_engine`在`grpc_event_engine_init`中通过调用`try_engine`设置，实际指向在src/core/lib/iomgr/ev_epollex_linux.cc的`vtable`
    * `grpc_event_engine_init`在`iomgr_platform_init`中调用
    * `iomgr_platform_init`在iomgr_posix.cc的全局变量`vtable`中注册
  * 如果不设置环境变量`GRPC_POLL_STRATEGY`，`try_engine`中得到的参数就是`all`，这样实际就是获取`g_factories`第一个非空指针，也就是`grpc_init_epollex_linux`
    * `GRPC_POLL_STRATEGY`就是指定epoll/poll等策略。
  * `grpc_init_epollex_linux`在src/core/lib/iomgr/ev_epollex_linux.cc
自定义vtable可能是为与C兼容。


typedef struct {
  bool can_get_pollset;
  bool can_listen;
  size_t (*size)(void);
  void (*init)(grpc_pollset* pollset, gpr_mu** mu);
  grpc_error* (*kick)(grpc_pollset* pollset,
                      grpc_pollset_worker* specific_worker);
  grpc_error* (*work)(grpc_pollset* pollset, grpc_pollset_worker** worker,
                      grpc_millis deadline);
  void (*shutdown)(grpc_pollset* pollset, grpc_closure* closure);
  void (*destroy)(grpc_pollset* pollset);
} cq_poller_vtable;



iomgr_posix.cc 中的几个全局变量设置：
* grpc_posix_pollset_vtable: src/core/lib/iomgr/ev_posix.cc
* grpc_posix_tcp_client_vtable: src/core/lib/iomgr/tcp_client_posix.cc
* grpc_posix_tcp_server_vtable: src/core/lib/iomgr/tcp_server_posix.cc
* grpc_generic_timer_vtable: src/core/lib/iomgr/timer_generic.cc
* grpc_posix_pollset_set_vtable: src/core/lib/iomgr/ev_posix.cc
* grpc_posix_resolver_vtable: src/core/lib/iomgr/resolve_address_posix.cc

grpc_completion_queue_create_internal 的主要功能：
* 设置 vtable 和 poller_vtable
* 调用 grpc_pollset_init ，也就是 src/core/lib/iomgr/ev_epollex_linux.cc 中的`pollset_init` （实现之一）


```c++
struct grpc_completion_queue {
  /** Once owning_refs drops to zero, we will destroy the cq */
  gpr_refcount owning_refs;

  gpr_mu* mu;

  const cq_vtable* vtable;
  const cq_poller_vtable* poller_vtable;
  grpc_closure pollset_shutdown_done;
  int num_polls;
};
```

