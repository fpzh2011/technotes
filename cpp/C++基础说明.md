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

## lambda表达式与函数对象

https://www.jianshu.com/p/d686ad9de817

如果捕获变量在lambda外部是引用类型，但是捕获列表中没有指定引用捕获形式，就按value捕获：
```cpp
// c++ -std=c++17
#include <iostream>

int main() {
	int i = 0;
	auto &ref = i;
	ref = 1;
	std::cout << ref << std::endl;

	// copy
	auto copy = [ref] () mutable {ref = 2;};
	copy();
	std::cout << ref << std::endl;

	// ref
	auto modify = [&ref] () mutable {ref = 3;};
	modify();
	std::cout << ref << std::endl;
}
```

## traits

https://blog.csdn.net/lihao21/article/details/55043881
https://www.jianshu.com/p/6f18d17bc161
https://harttle.land/2015/09/15/effective-cpp-47.html
https://www.dongming.io/post/type_traits/
https://zh.wikipedia.org/wiki/Traits_(计算机科学)
https://zhuanlan.zhihu.com/p/85809752

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

## 静态/全局变量初始化顺序 static/global

不同编译单元内，静态/全局变量初始化顺序是未定义的。
函数+static可以保证初始化顺序。
静态初始化可以保持线程安全(从[文档](https://en.cppreference.com/w/cpp/language/initialization)看是这样的)
https://stackoverflow.com/questions/3746238/c-global-initialization-order-ignores-dependencies/3746249#3746249
https://isocpp.org/wiki/faq/ctors#static-init-order
https://en.cppreference.com/w/cpp/language/initialization

## 线程

主线程退出后，`detach`的线程也会被终止、不会抛异常。
如果`thread`的析构函数运行时，线程是`joinable`的，比如正在运行，标准库会调用`terminate()`终止整个程序。相关分析可参考 Effective Modern C++, P252 。
`t.join()`会阻塞调用线程的执行，等待`t`执行完毕、退出后，调用线程才会继续执行。

```cpp
#include <chrono>
#include <iostream>
#include <thread>

void f(){
	int i = 0;
	while(true) {
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::cout << (++i) << std::endl;
	}
}

int main() {
	std::thread t(f);
	//如果没有detach也不join，主线程退出会抛异常
	//参考std::thread::~thread()的说明
	t.detach();
	std::this_thread::sleep_for(std::chrono::seconds(8));
	std::cout << "main exit" << std::endl;
	return 0;
}
```

## future

https://www.cnblogs.com/haippy/p/3239248.html
https://www.cnblogs.com/haippy/p/3279565.html
https://www.cnblogs.com/haippy/p/3280643.html

folly
https://www.cnblogs.com/chenyangyao/p/folly-future.html
https://my.oschina.net/fileoptions/blog/881798
https://yq.aliyun.com/articles/115416
https://zh.wikipedia.org/wiki/Future与promise

## 内存模型

我们必须对编译器和 CPU 作出一定的约束才能合理正确地优化你的程序，那么这个约束是什么呢？答曰：内存模型。
内存模型是程序员、编译器，CPU 之间的契约，遵守契约后大家就各自做优化，从而尽可能提高程序的性能。
https://www.cnblogs.com/haippy/p/3412858.html

https://zh.cppreference.com/w/cpp/atomic/memory_order
https://www.zhihu.com/question/24301047
https://zhuanlan.zhihu.com/p/24983412
https://github.com/forhappy/Cplusplus-Concurrency-In-Practice

### atomic

shared_ptr (C++17, gcc7.4.0):
* 原子操作的一种实现: atomic_store_explicit(memory_order_release) / atomic_load_explicit(memory_order_acquire)
* 实际是根据shared_ptr对象地址hash后、从gcc内置的若干个mutex中获取一个
* [shared_ptr的control block是线程安全的](https://zh.cppreference.com/w/cpp/memory/shared_ptr/atomic)
* C++20会实现`atomic<shared_ptr>`
/App/gcc/include/c++/7.4.0/bits/shared_ptr_atomic.h
https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/src/c++11/shared_ptr.cc

还有一种方式，是使用`atomic<size_t>`作index，结合vector。
问题是：用mutex设置size_t后，vector被更新的元素，能否同步给其它线程？

## 类的几种常用函数

```cpp
#include <iostream>

class Wiget {
public:
	Wiget() {
		std::cout << "Wiget default constructor" << std::endl;
	}

	Wiget(const std::string &s) {
		std::cout << "Wiget string constructor. s=" << s << std::endl;
	}

	Wiget(const Wiget &w) {
		std::cout << "Wiget copy constructor" << std::endl;
	}

	Wiget(Wiget&& w) noexcept {
		std::cout << "Wiget move copy constructor" << std::endl;
	}

	Wiget& operator=(const Wiget &w) {
		std::cout << "Wiget copy assignment" << std::endl;
		return *this;
	}

	Wiget& operator=(Wiget&& w) noexcept {
		std::cout << "Wiget move copy assignment" << std::endl;
		if(this != &w) {
			std::cout << "\tfree this resource, assume parameter w's resource" << std::endl;
		}
		return *this;
	}
};

int main() {
	Wiget w;
	const Wiget w2("abc");
	auto w3 = w2;
	w = w3;
	Wiget w4(std::move(w));
	w3 = std::move(w3);
	w3 = std::move(w4);
	return 0;
}
```

## 自定义类型、函数返回值、左值、右值

如果函数返回的是引用，返回值是左值（C++ Primer, P202）；否则，返回值一般是右值。
如果函数返回的是内置类型如`int`，返回值不能被赋值。但如果函数返回的是类类型，返回值可以被赋值，可以引用返回值数据成员，但是不能对返回值的数据成员赋值。（Effective C++, P19）
```cpp
#include <iostream>

int f() { return 0; }

struct block { int x; };

block f2() { return block(); }

int main() {
	//拷贝赋值成员函数 N3936, §3.10/5
	f2() = block();
	std::cout << f2().x << std::endl;
	//f2().x = 1; //编译错误
	//f() = 1; //编译错误
	return 0;
}
```

## 引用折叠

C++ Primer, P608

```cpp
#include <iostream>

//f的模版参数是右值引用。
//如果调用f的实参是左值，T被解析为左值引用，可以更改实参的值。
template <typename T> void f(T&& t) {
	T v = t;
	v = 3;
}

int main() {
	int i = 1;
	std::cout << i << std::endl;
	f(i);
	std::cout << i << std::endl;
	return 0;
}
```

## 不测试私有方法的几个原因

* 如果没有更改public接口，而只修改了内部实现，不应删除原有测试代码。
* 测试私有方法表明类承担多项职责：公共的，私有的。
https://stackoverflow.com/questions/3676664/unit-testing-of-private-methods
https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#testing-private-code
https://www.artima.com/suiterunner/privateP.html

## 浮点数比较

https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/

### 最小浮点数

目前已知的double最小正数，与python结果一致：
```cpp
#include <iostream>
#include <limits>
#include <cfloat>

int main() {
  std::cout << std::numeric_limits<double>::epsilon() << std::endl;
  std::cout << DBL_EPSILON << std::endl;
  std::cout << DBL_MIN << std::endl;
  std::cout << DBL_MIN*DBL_EPSILON << std::endl;
  std::cout << 1.0/(DBL_MIN*0.27) << std::endl;
  return 0;
}
```
有没有更可靠的方法防止除法出现inf？

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
