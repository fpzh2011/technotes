# C++多线程编程基础

开源线程池
http://stackoverflow.com/questions/3988128/c-thread-pool
http://threadpool.sourceforge.net/
http://threadpool.sourceforge.net/tutorial/intro.html

## 线程安全

### new 是线程安全的

这也是 new 和 malloc 比较慢的原因之一。
`man malloc`
http://stackoverflow.com/questions/796099/c-new-operator-thread-safety-in-linux-and-gcc-4


### 标准输出的线程安全

主要关注多线程环境下的标准（错误）日志输出。

#### C++11标准

C++11标准 CH27.2.3 Thread safety：除非特殊注明的，iostream 和 C io 库函数不是线程安全的。

还可以参考 CH 27.4.1 的第4条，以及`bool sync_with_stdio(bool sync = true);`的说明。

https://msdn.microsoft.com/en-us/library/c9ceah3b.aspx

printf、maloc/free等函数是线程安全的。参考[陈硕，Linux多线程服务端编程，P86，P103]。

#### 其它资料

而根据 [cppreference](http://en.cppreference.com/w/cpp/io/cerr)，cerr和wcerr是线程安全的。
但是要注意，内容要一次性输出，如`std::cerr << "this is first line." << std::endl << "this is second line.\n`，多次函数调用的结果不一定保证一起输出。

另外可以参考：http://stackoverflow.com/questions/15033827/multiple-threads-writing-to-stdcout-or-stdcerr/15034451

但是标准中sync_with_stdio一节没有相关说明。而TR18015中说默认情况下cout与stdin是同步的，可能会有性能损失。

所以，这个问题其实还没有完全搞明白。但暂时还可以认为cerr是线程安全的，可以这样在程序中使用。

#### apue

P370，P62，pwrite系统调用。

P355 标准库中关于文件锁的函数。

### 建议

http://stackoverflow.com/questions/5125241/how-to-make-an-application-thread-safe

### 其它资料

apue P355 线程安全和不安全的函数。

可重入、可取消的函数。

pthread很多函数的参数为指针，应该是为兼容进程共享。



问题：
启动多个线程后，如果查询一个线程的状态而又不阻塞？
mysql的stmt初始化是线程安全的吗？mysql多线程编程，需要注意什么？
vector等容器是线程安全的吗？
线程共享内容，可以放在主线程的栈中吗，还是只能放在全局存储、静态存储或者堆中？



pthread vs. OpenMP
http://stackoverflow.com/questions/3949901/pthreads-vs-openmp

深入应用C++11：代码优化与工程级应用
http://www.th7.cn/Program/cp/201306/140256.shtml
http://blog.csdn.net/saga1979/article/details/44057633


std::this_thread::sleep_for(std::chrono::seconds(1));

## 线程调用成员函数

http://stackoverflow.com/questions/12160175/c-creating-new-thread-using-pthread-create-to-run-a-class-member-function
http://stackoverflow.com/questions/10673585/start-thread-with-member-function
http://stackoverflow.com/questions/266168/simple-example-of-threading-in-c
http://stackoverflow.com/questions/1151582/pthread-function-from-a-class
http://stackoverflow.com/questions/15734598/how-to-execute-a-class-member-function-in-a-separate-thread-using-c11-thread-c

##并行计算

https://www.zhihu.com/question/20188244
http://book.douban.com/subject/10742110/

## lock free

http://preshing.com/20130605/the-worlds-simplest-lock-free-hash-table/
http://web.stanford.edu/class/ee380/Abstracts/070221_LockFreeHash.pdf
http://stackoverflow.com/questions/14338732/is-it-possbile-to-implement-lock-free-map-in-c
https://www.ll.mit.edu/HPEC/agendas/proc11/Day1/Posters/A-9_Feldman_A.pdf
http://www.stroustrup.com/lock-free-vector.pdf
可以试试boost  http://stackoverflow.com/questions/1164023/is-there-a-production-ready-lock-free-queue-or-hash-implementation-in-c
http://www.liblfds.org/
http://libcds.sourceforge.net/
http://www.memoryhole.net/kyle/2011/06/a_c_lockfree_hash_table_implem.html
http://excess-project.eu/publications/published/CuckooHashing_ICDCS.pdf
https://github.com/kumpera/Lock-free-hastable
http://blog.jobbole.com/39186/
http://www.sunrisetel.net/software/devtools/sunrise-data-dictionary.shtml
http://www.wseas.org/multimedia/journals/computers/2013/5705-125.pdf
http://www.drdobbs.com/lock-free-data-structures/184401865
http://www.research.ibm.com/people/m/michael/spaa-2002.pdf
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3425.html
http://www.rossbencina.com/code/lockfree
http://www.cs.tau.ac.il/~afek/SplitOrderListHashSS03.pdf
http://www.linuxtag.org/2012/fileadmin/www.linuxtag.org/slides/Ulrich%20Drepper%20-%20Fast_%20less-complicated_%20lock-free%20data%20structures.p3.pdf
http://www.xuebuyuan.com/2162073.html
http://www.yebangyu.org/blog/2016/02/16/materials-in-concurrency-programming/
http://arxiv.org/pdf/1302.2757.pdf
https://www.ll.mit.edu/HPEC/agendas/proc11/Day1/Posters/A-9_Feldman_A.pdf
http://dl.acm.org/citation.cfm?id=564870.564881

https://github.com/boostorg/lockfree
https://github.com/memsql/lockfree-bench
https://github.com/yamasa/lockfree
https://github.com/kevinlynx/lockfree-list
https://github.com/eile/boost_lockfree
https://github.com/brycelelbach/boost.lockfree




