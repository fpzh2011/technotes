# GDB学习

## 编译选项

`gcc -g a.c`	# g选项表明增加debug信息

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

