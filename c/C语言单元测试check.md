# 单元测试Check

头脑中时刻想着测试，才能写出便于测试的代码。

## Check概况

Check是一个C语言单元测试框架，通常需要与autoconf/automake配合使用。

默认情况下，对每个单元测试，Check调用fork创建一个进程，在独立的地址空间中进行测试，以免被测试的代码出现异常时影响其它单元测试的运行。

## 如何测试static函数

有三种方法可以考虑：

1 `#include static.c` 即将定义static函数的源文件include到测试代码中。

2 如果测试代码比较少，可以在源文件中写测试代码。但这样感觉不好。

3 使用条件编译。这样感觉也不好，是否会影响代码可读性？最好是生产代码与测试代码分离。

http://stackoverflow.com/questions/593414/

## 问题

如何测试 putchar、getchar这类IO函数？都通过重定向，再读取文件？


## 参考资料


