# C++基础说明

首先，C++不是“C with classis”。C和C++都有各自的语言哲学。

## 最重要的几件事

### RAII

“资源获得即初始化”（RAII，Resource Acquisition Is Initialization）
 “智能指针”（Smart Pointer）
 http://blog.csdn.net/bonchoix/article/details/8044560

### STL
 
熟悉STL。让STL成为自己思维方式的一部分。

### 经典书籍

C++ Primer 第5版
Effective C++
Effective STL 

## 编译

编译一般用 `g++ test.cpp` 。

也可以用gcc，gcc再调用g++。如 `gcc -c test.cpp; gcc -lstdc++ test.o`，或 `gcc -lstdc++ test.cpp`。但一般很少这样用。

gcc根据文件名后缀决定采用哪种编译、链接动作（man gcc）。

### 支持标准

`-std=`

	c++98 c++03 ansi
	c++11

### autoconf

AC_PROG_CC -> AC_PROG_CXX

如果要指定编译选项，可以通过 `./configure CC="gcc -Wpedantic" CFLAGS=-g LIBS=-lposix` 实现。
或者参考 `AM_CFLAGS CFLAGS`。（autoconf手册，16.2 Compilers and Options）

## C++语言标准

http://www.open-std.org/jtc1/sc22/wg21/

## 用户定义文本（User-defined literals）

比如代码 `json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;`。

参考资料：
C++标准 CH2.14.8
https://www.chenlq.net/books/cpp11-faq/cpp11-faq-chinese-version-series-user-defined-literals.html
http://en.cppreference.com/w/cpp/language/user_literal
https://akrzemi1.wordpress.com/2012/08/12/user-defined-literals-part-i/
https://msdn.microsoft.com/zh-cn/library/dn919277.aspx

## raw string literal 

如 `auto s = "(test)";`

参考资料：
http://blog.csdn.net/ubiter/article/details/35277247
https://solarianprogrammer.com/2011/10/16/cpp-11-raw-strings-literals-tutorial/
https://en.wikipedia.org/wiki/C++11#New_string_literals
https://zh.wikipedia.org/wiki/C++11
https://www.zhihu.com/question/32173783

## 预处理指令

`#pragma once` https://en.wikipedia.org/wiki/Pragma_once

## 定义二进制整数

gcc可以用 `int x = 0b101; ` ，符合C++2014标准，但目前并不是所有编译器都支持。

http://stackoverflow.com/questions/2611764/can-i-use-a-binary-literal-in-c-or-c

## 浮点数

`nan, inf`这些值都是[IEEE 754](https://en.wikipedia.org/wiki/IEEE_floating_point)定义的，和1、2、3一样都是可存储为double的值。
```cpp
#include <iostream>
#include <limits>

int main() {
	//inf的输出显示
	std::cout << std::numeric_limits<double>::infinity() << std::endl;
	std::cout << -std::numeric_limits<double>::infinity() << std::endl;
	std::cout << (1.0 + std::numeric_limits<double>::infinity()) << std::endl;
	std::cout << (1.0 - std::numeric_limits<double>::infinity()) << std::endl;
	//比较大小
	std::cout << (1.0 > std::numeric_limits<double>::infinity()) << std::endl;
	std::cout << (1.0 < std::numeric_limits<double>::infinity()) << std::endl;
	//nan
	std::cout << std::numeric_limits<double>::quiet_NaN() << std::endl;
	std::cout << std::numeric_limits<double>::signaling_NaN() << std::endl;
	std::cout << (1.0 + std::numeric_limits<double>::quiet_NaN()) << std::endl;
	//float
	std::cout << std::numeric_limits<float>::infinity() << std::endl;
	std::cout << std::numeric_limits<float>::quiet_NaN() << std::endl;
	//int
	std::cout << std::numeric_limits<int>::signaling_NaN() << std::endl;
	return 0;
}
```

## 开源项目

leveldb https://github.com/google/leveldb

http://en.cppreference.com/w/cpp/links/libs

## 其它

`std::this_thread::sleep_for(std::chrono::seconds(1));`

## 参考资料与在线资源

	[1] C++ Primer第5版（中文版）
	[2] http://www.cplusplus.com/
	[3] http://en.cppreference.com/w/
	[4] More C++ Idioms https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms



单元测试

google test
cppunit
cxxtest
boost Test
如何测试私有方法？不要测试，或者通过public测试。
http://stackoverflow.com/questions/242926/comparison-of-c-unit-test-frameworks
http://gamesfromwithin.com/exploring-the-c-unit-testing-framework-jungle
http://coolshell.cn/articles/8209.html



 
 








## C++语言最佳实践

**Google C++ Style Guide**

C++中，class的声明在头文件中，包括所有成员变量、成员函数。成员函数的定义一般在单独的cc文件中。
关于在头文件中定义函数等逻辑的利弊分析，相关文章可以参考：

	http://stackoverflow.com/questions/17619255/the-benefit-of-writing-the-definition-of-method-in-implementation-file-cpp-file
	http://stackoverflow.com/questions/1783849/what-are-the-advantages-and-disadvantages-of-implementing-classes-in-header-file
	http://stackoverflow.com/questions/4467038/is-it-considered-bad-form-in-c-to-put-a-class-declaration-containing-method-de
	http://stackoverflow.com/questions/902432/when-to-use-header-files-that-do-not-declare-a-class-but-have-function-definitio




### 需要谨慎使用的语言特性

	动态类型信息、虚拟继承、异常。（参考[1]）




	

## 问题

C++在每次调用完库函数后，是否需要判断异常、错误码？

std::cin >> i >> j; 如何判断输入是否成功，比如需要整数，但实际提供的是字符？
	用if或while直接检测流对象，如 `while(std::cin >> i)` ，如果正常输入就是true，否则就是false。（参考[1]，P13）
	那么，如何判断是出现错误，还是文件结束，还是遇到错误字符呢？
	如何忽略一部分内容，比如 %d %s %d ，忽略%s的输入。
	cin能否输入带空格的字符串，比如"this is a test"
类似sscanf可以参考 istringstream，或者string based stream。
>> 和 << 本质上是操作符重载函数，实际调用的还是函数吧。
连续的 >> ，是一次函数调用，还是两次？从汇编代码看，应该是两次函数调用。如果内容不匹配，会退回缓冲区。
表达式 std::cin >> i 返回值是其左侧运算符，即cin。cout也类似。

make_shared 会返回空指针吗？如果持续创建堆内存知道用尽，程序直接退出“已杀死”，即使尝试捕捉异常，也不会输出任何其它信息。是因为assert退出进程吗？
用strace跟踪，killed by SIGKILL，有可能是操作系统直接杀死进程。
http://stackoverflow.com/questions/11922262/what-happens-if-a-shared-ptrs-constructor-fails


C不支持泛型，对于栈、队列等常用数据类型，是否每次都需要自己重新写？

C一般如何解决log问题？log需要解决哪些问题：分级，debug？log4c/zlog似乎都太大？
如果是debug，自己写个简单函数是否足够？
程序中包含了stdio.h，调用了printf，链接的时候，gcc怎么知道该和哪个库文件链接呢？.o文件中可能已经不知道包含了哪个头文件了。gcc记录了每个库函数声明与库文件的映射关系？
在用Check单元测试时，需要将头文件提供给Check。如果某个函数作为实现细节不需要在最终的头文件中提供，但是又需要进行单元测试，如何处理？制作两个头文件，一个用于测试，一个用于交付？zabbix等开源软件是如何测试的？
一个.c源文件中定义的struct，能否被其它源文件引用（没有include）？

能否将多个c文件编译为一个o文件？
