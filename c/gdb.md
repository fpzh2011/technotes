# GDB学习

## 安装

除了安装gdb，还需要安装glib等的debug包。否则，会提示`Missing separate debuginfos, use: debuginfo-install glibc-2.12-1.192.el6.x86_64 zlib-1.2.3-29.el6.x86_64`。

debuginfo-install glibc-2.17-292.el7.x86_64 gperftools-libs-2.6.1-1.el7.x86_64 keyutils-libs-1.5.8-3.el7.x86_64 krb5-libs-1.15.1-37.el7_7.2.x86_64 libcom_err-1.42.9-16.el7.x86_64 libevent-2.0.21-4.el7.x86_64 libgcc-4.8.5-39.el7.x86_64 libselinux-2.5-14.1.el7.x86_64 openssl-libs-1.0.2k-19.el7.x86_64 pcre-8.32-17.el7.x86_64 zlib-1.2.7-18.el7.x86_64

```shell
sudo yum install yum-utils
sudo debuginfo-install glibc zlib
```

http://stackoverflow.com/questions/10389988/missing-separate-debuginfos-use-debuginfo-install-glibc-2-12-1-47-el6-2-9-i686

## coredump

路径配置:
```shell
cat /proc/sys/kernel/core_pattern
```

## 编译选项

```
g++ -gdwarf-4 -fvar-tracking-assignments test.cc
gcc -g a.c
```

## 启动gdb

```
gdb --exec=./vector_recall_server --pid=4272 --directory=../
gdb -p 4272
# core dump
gdb [exec_file] [core_file]
```

## 线程

info threads: 列出所有线程
bt: 显示当前线程的堆栈。bt full 可以查看完整信息，包括函数参数。
thread 2: 切换到编号为2的线程
thread apply all bt: 查看所有线程的堆栈
set scheduler-locking off|on|step 设置只有当前线程执行、所有线程执行
https://www.cnblogs.com/xuxm2007/archive/2011/04/01/2002162.html

## 设置断点

https://ftp.gnu.org/old-gnu/Manuals/gdb/html_node/gdb_28.html
http://www.unknownroad.com/rtfm/gdbtut/gdbbreak.html
* `break thread_test.c:123 thread all`: 在所有线程中相应的行上设置断点。文件名可以是绝对路径。
* `break subrecall::VectorServiceImpl::Recall`: 按函数设置断点。通常会设置在函数最后一行代码。
* `break filename:function`
* `info br[eakpoints]`: 列出所有断点
* `dis[able] 1`: Disable breakpoint 1
* `en[able] 1`: Enable breakpoint 1
* `d[elete] 1`: Delete breakpoint 1。可指定多个断点。如果不指定断点，就删除所有断点。

## 变量

https://sourceware.org/gdb/current/onlinedocs/gdb/Symbols.html
* `info variables`: list "All global and static variable names"
* `info locals`: list "Local variables of current stack frame"
* `info args`: list "Arguments of the current stack frame"

## 有用的命令

https://ccrma.stanford.edu/~jos/stkintro/Useful_commands_gdb.html

## 疑问

一个server启动后，gdb attach pid，需要重新run启动吗？如果不需要，如何查看线程、bt？




## gdb命令行选项

`gdb -x file`	# 从文件file读取gdb命令，如 b 27

## gdb命令

l 或 list 列出源代码

b 16 或 break 16	# 在第16行设置断点

break func	# 在函数func的入口设置断点

info break	# 查看断点信息

r 或 run 运行程序。run后面可以带程序的参数，也可以用 <> 重定向输入、输出。

n 或 next 单条语句执行

c 或 continue 继续执行

p i 或 print i 打印变量i的值

finish 退出函数

q 或 quit 退出gdb会话

shell 运行shell命令，如`shell pwd`

help 帮助

help breakpoints 查看断点类的命令列表

命令可以用tab补全。


## 重要调试命令

gdb -p 4522 调试一个正在运行的进程，参数是pid

bt 查看函数堆栈

thread apply all bt 查看线程的函数调用栈


## 参考资料

	http://coolshell.cn/articles/3643.html
	http://blog.csdn.net/haoel/article/details/2879
	http://blog.csdn.net/haoel/article/details/2880
	http://blog.csdn.net/haoel/article/details/2881
	http://blog.csdn.net/haoel/article/details/2882
	http://blog.csdn.net/haoel/article/details/2883
	http://linuxtools-rst.readthedocs.org/zh_CN/latest/tool/gdb.html
	http://www.programlife.net/gdb-manual.html
	http://wiki.ubuntu.org.cn/用GDB调试程序
	http://www.vimer.cn/2009/11/使用gdb调试程序详解.html




如何将gdb命令保存到文件，在调试时调用？
多个o文件链接的可执行文件，gdb如何设置断点调试？
gdb的条件断点。

