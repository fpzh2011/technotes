# Linux线程基础

## 线程分类

线程分为两种，joinable or detached。man pthread_create
默认。区别。也可以在创建线程是指定。

线程创建
运行顺序，P309，
man pthread_create
Unless real-time scheduling policies are being employed, after a call to pthread_create(), it is indeterminate which thread—the caller or the new thread—will next execute.

线程标识，不同实现的结构不一样，不一定是整数。pthread_t gettid proc/xxx/task
编码方法。唯一性。id比较方法
self equal
可以被复用。`man pthreads` 的 Thread IDs 节。A thread ID may be reused after a terminated thread has been joined,  or a detached thread has terminated. 


获取线程返回值。join一方面是获取线程退出码，一方面是释放线程资源。
多个线程同时join一个线程的行为，是未定义的。join一个之前已经join过的线程，也是未定义的。
join一个joinable线程失败，会产生僵尸线程，占用系统资源。


编译选项 -pthread
连接选项 -lpthread

线程终止。返回值。
man pthread_exit()
除了主线程外，从线程的启动函数返回，将导致隐式调用pthread_exit()。
Performing a return from the start function of any thread other than the main thread results in an implicit call to pthread_exit(), using the function's return value as the thread's exit status.
如果想让主线程退出，而其它线程继续运行，主线程需要调用pthread_exit()而不是exit()。
To allow other threads to continue execution, the main thread should terminate by calling pthread_exit() rather than exit(3).
但是，主线程退出之后，/proc/xxxx/task中，仍然有主线程对应的目录。




数据共享
P308 第2段
P358


查询线程实现
getconf GNU_LIBPTHREAD_VERSION




