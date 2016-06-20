# C++多线程编程基础

本文件基于C++11标准。

## 只读容器的线程安全性

一个 STL Container 对象，如果在一个时间区间T内，其值不改变，那么在时间T内，多线程并发**只读**访问这个对象，是线程安全的。

参考：
http://en.cppreference.com/w/cpp/container
http://stackoverflow.com/questions/7455982/is-stl-vector-concurrent-read-thread-safe
https://msdn.microsoft.com/en-us/library/c9ceah3b.aspx
http://www.sgi.com/tech/stl/thread_safety.html
https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_concurrency.html
http://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm


使用C++11的thread接口：
`g++ -c -pthread test.cc`
`g++ -o test -thread`
或
`g++ -pthread test.cc`

使用C的pthread接口：
`g++ -c test.cc`
`g++ -o test -lpthread test.o`



## 原子类型（C++11）

http://www.cnblogs.com/haippy/p/3301408.html
http://www.cnblogs.com/haippy/p/3252056.html
http://en.cppreference.com/w/cpp/atomic/atomic
http://www.cplusplus.com/reference/atomic/


## unique_lock vs lock_guard

lock_guard 只能一次性的加解锁：在构造函数中加锁，在析构函数中解锁。lock_guard主要为“以RAII方式对mutex进行管理”提供便利。
但是lock_guard不能用于条件变量。

unique_lock可以和条件变量一起使用。unique_lock的构造函数中，可以选择是加锁还是不加锁（std::adopt_lock, std::defer_lock等）。
在后续使用中，可以对一个unique_lock对象多次加锁、解锁，对临界区进行更细粒度的控制。
另外，可以和std::lock()配合使用，对多个锁以RAII方式进行管理。

std::lock() 可对多个锁同时加锁，可以规避死锁（？）。

http://en.cppreference.com/w/cpp/thread/unique_lock
http://www.cplusplus.com/reference/mutex/unique_lock/
http://www.cplusplus.com/reference/mutex/unique_lock/unique_lock/
http://en.cppreference.com/w/cpp/thread/lock
http://stackoverflow.com/questions/20516773/stdunique-lockstdmutex-or-stdlock-guardstdmutex
http://stackoverflow.com/questions/17113619/whats-the-best-way-to-lock-multiple-stdmutexes/17113678#17113678

## thread 使用注意事项

如果创建的joinable线程没有调用join()，进程退出时会报错并异常转储：`terminate called without an active exception`

编译链接选项：
`g++ -Wl,--no-as-needed -std=c++11 -pthread test.cc -o test`
否则运行会报错：`Enable multithreading to use std::thread: Operation not permitted`

http://stackoverflow.com/questions/19463602/compiling-multithread-code-with-g

### 参数传递

C++的thread实现，即使参数声明为引用，也会进行拷贝。所以，对于不能、或不适合拷贝的对象，尽量通过智能指针传递，尽量避免通过普通指针传递。

对于可能进行多线程处理的数据，尽量通过智能指针管理。


## 线程参数拷贝

by default the thread constructor will copy all arguments passed to it
http://stackoverflow.com/questions/10673585/start-thread-with-member-function
http://www.cplusplus.com/reference/thread/thread/thread/

## pthread静态连接问题

C++11的thread如果静态链接，需要指定`-std=c++11 -static -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive`。
否则，可能报错：
```
terminate called after throwing an instance of 'std::system_error'
  what():  Enable multithreading to use std::thread: Operation not permitted
已放弃 (核心已转储)
```
或`段错误 (核心已转储)`。gdb调试报错`Thread 2 "test" received signal SIGSEGV, Segmentation fault.`。

http://stackoverflow.com/questions/8649828/what-is-the-correct-link-options-to-use-stdthread-in-gcc-under-linux
https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52590#c4

## 用类成员函数构造/启动线程

示例代码：
```cpp
std::thread t1(&SomeClass::threadFunction, *this, arg1, arg2);
//或用lamda表达式
std::thread( [this] { this->test(); } );
```

http://stackoverflow.com/questions/10673585/start-thread-with-member-function
http://stackoverflow.com/questions/21059115/c11-thread-class-how-to-use-a-class-member-function
http://stackoverflow.com/questions/15734598/how-to-execute-a-class-member-function-in-a-separate-thread-using-c11-thread-c

## 任务调度

https://github.com/dougbinks/enkiTS









