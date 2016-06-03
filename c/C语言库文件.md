# C语言库文件

Linux下编写C程序的时候，不可避免的需要链接库文件。库都包含哪些文件呢？

## 动态库与静态库

库分为静态库和动态库。

静态库，扩展名为".a"，即archive。静态库在构建程序时链接，相同的库代码，在不同的进程中独立存在。

动态库，又称共享库，扩展名为".so"，即shared object libraries。动态库在运行时链接，相同的库在系统内存中只有一份拷贝。

## glibc

glibc是C程序库的一种实现。glibc不仅包括C标准库的内容，也包括pthread等POSIX的内容。

库文件名一般以lib开头。在rhel7下，库文件部署在 `/usr/lib /usr/lib64`。如/usr/lib/libc-2.17.so和/usr/lib/libm-2.17.so。

其中`libm-2.17.so`是C标准库中的数据函数库，`libc-2.17.so`是C标准库中的其它函数库。
gcc链接时，默认不会链接libm，所以需要用 `-lm` 选项显式链接数学库函数。

glibc将fork等函数也实现在`libc-2.17.so`中（`nm /lib/libc.so.6 | grep fork`）。其它的库文件如`/usr/lib/libpthread-2.17.so`等。
http://baike.baidu.com/view/1323132.htm 中对一些库文件作了说明。

如果在连接时**禁用标准库**，可以使用选项 `-nostdlib` 或 `-nodefaultlibs`。

参考资料

	man 7 libc
	https://en.wikipedia.org/wiki/GNU_C_Library
	https://en.wikipedia.org/wiki/C_standard_library
	http://baike.baidu.com/view/1323132.htm
	http://stackoverflow.com/questions/1033898/why-do-you-have-to-link-the-math-library-in-c

## libc glibc glib gnulib

glibc是linux下面c标准库的实现，即GNU C Library。
glibc本身是GNU旗下的C标准库，后来逐渐成为了Linux的标准c库，而Linux下原来的标准c库Linux libc慢慢不再被维护。
Linux下面的标准c库不是只有glibc，其它如uclibc、klibc，以及上面提到的Linux libc，但是glibc无疑是用得最多的。

glib是 Gtk+ 库和 Gnome 的基础。glib可以在多个平台下使用，比如Linux、Unix、Windows等。

gnulib是提供从POSIX API到native API转换的适配器库。

参考资料

	http://blog.csdn.net/yasi_xi/article/details/9899599
	http://www.freesoftwaremagazine.com/articles/history_of_glibc_and_linux_libc
	https://developer.gnome.org/glib/
	http://stackoverflow.com/questions/2240120/glibc-glib-and-gnulib

## Linux库相关命令

ldd 列出共享库依赖

nm 列出object文件的符号表

……

参考资料

	http://www.cyberciti.biz/tips/linux-shared-library-management.html
	http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html

