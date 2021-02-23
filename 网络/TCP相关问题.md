# TCP相关问题

## 基础

TCP网络编程中connect()、listen()和accept()三者之间的关系
https://blog.csdn.net/tennysonsky/article/details/45621341

## SO_REUSEPORT 多个进程、线程绑定同一个ip:port

https://zhuanlan.zhihu.com/p/35367402
https://zhuanlan.zhihu.com/p/63815100

## 粘包

http://www.cnblogs.com/zhaox583132460/p/3372381.html
TCP的数据是字节流，没有结构。

## 惊群

http://blog.csdn.net/russell_tao/article/details/7204260
简单说来，多线程/多进程（linux下线程进程也没多大区别）等待同一个socket事件，当这个事件发生时，这些线程/进程被同时唤醒，就是惊群。可以想见，效率很低下，许多进程被内核重新调度唤醒，同时去响应这一个事件，当然只有一个进程能处理事件成功，其他的进程在处理该事件失败后重新休眠（也有其他选择）。这种性能浪费现象就是惊群。

https://www.zhihu.com/question/24169490
