# Java Concurrency In Practice

## Chapter 2. Thread Safety

The primary mechanism for synchronization in Java is the
synchronized keyword, which provides exclusive locking, but the term "synchronization" also includes the use of
volatile variables,explicit locks,and atomic variables.

a  program  that consists  entirely  of 
thread safe  classes may not be thread safe, and a  thread safe program may contain  classes  that are not thread safe. 


## 问题

https://docs.oracle.com/javase/tutorial/essential/concurrency/
https://docs.oracle.com/javase/specs/jls/se8/html/jls-17.html

java中关于同步、重排序、内存可见性的注意事项，在其它语言如 C C++ Python 中如何？

notify/wait 与 condition 的区别是什么？为什么必须在同步区域内调用？是同一个锁吗，不会死锁吗？
condition自带锁？各自的适用场景是什么？
如果多个线程调用wait(timeout)、且都超时，是不是只有一个线程能退出wait方法调用？



