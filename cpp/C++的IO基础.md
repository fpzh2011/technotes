# C++的IO基础

cin、cout等的字母c的意思，是character。 http://www.stroustrup.com/bs_faq2.html#cout

## 基本知识

参考[1]，P23。

fstream对象被销毁时，close会被自动调用。（参考[1]，P285。）

## 输入对象

cin的类型是istream，用来从标准输入读取数据。

`std::cin >> i` 的返回值仍是cin，所以可以连续输入如 `std::cin >> i >> j`。

用if或while直接检测流对象，如 `while(std::cin >> i)` ，如果正常输入就是true，否则就是false。（参考[1]，P13）

那么，如何判断是出现错误，还是文件结束，还是遇到错误字符呢？可以参考：

	C++ Primer第5版（中文版），ch8.1.2，P280-281
	http://www.cplusplus.com/reference/istream/istream/
	http://www.cplusplus.com/reference/ios/ios/fail/

### 实际考察输入状态

下面的代码在rhel7中试验。输入 `abc 123` ，输出一直是如下内容：

	i rdstate: 4	fail: 1	good: 0	i=0
	j rdstate: 4	fail: 1	good: 0	j=8888

从上面输出可以看到：

1 cin读入数据时，即使失败，也会设置输入目标的值。`i` 的初始值为9999，输入失败后，值变为0。

2 cin如果是fail状态，不会处理后续的 `>>` 调用。因为 `j` 的值没有像 `i` 那样发生变化。

3 **cin如果进入fail状态，需要人工复位**。否则会一直持续下去，后续的输入操作都无法进行。复位代码：`cin.clear(cin.rdstate() & ~cin.failbit)` 。代码参考Primer P281。

4 eof为1，如果没有读入操作，fail就不为1。比如，运行`echo -n 123 456 | ./a.out` 和 `echo 123 456 | ./a.out`的结果，fail的值就不一样。原因是如果没有换行，读完第2个整数后进行while判断时就已经知道到达文件尾部，不再进行读操作，所以fail的值是0。

	#include <iostream>
	using namespace std;
	int 
	main () {
		int i = 9999, j = 8888, count = 0;
		cout << "input 2 int. i=" << i << " j=" << j << endl << endl;
		while(!cin.eof()) {
			cin >> i;
			cout
				<< "i rdstate: " << cin.rdstate() << '\t'
				<< "fail: " << cin.fail() << '\t'
				<< "good: " << cin.good() << '\t'
				<< "i=" << i << endl;
			cin >> j;
			cout
				<< "j rdstate: " << cin.rdstate() << '\t'
				<< "fail: " << cin.fail() << '\t'
				<< "good: " << cin.good() << '\t'
				<< "j=" << j << endl << endl;
		if(count++ > 3) {
				break;
			}
		}
		cout
			<< "rdstate: " << cin.rdstate() << '\t'
			<< "fail: " << cin.fail() << '\t'
			<< "eof: " << cin.eof() << '\t'
			<< "good: " << cin.good() << '\t'
			<< endl;
		return 0;
	}

## 输出对象

cout、cerr、clog都是ostream类型。

其中clog默认是被缓冲的，cerr默认是不缓冲的；二者默认都关联到标准错误。

默认情况下，cin与cerr都关联了cout，所以读cin或写cerr，会导致刷新cout。（参考[1],P282）

### 程序异常退出时的缓冲区处理

错误信息尽量使用cerr。因为程序崩溃或异常退出时，缓冲区是否被刷新是由多个因素决定的。程序崩溃时如果错误信息没有输出，可能会影响问题诊断。
在rhel7下的试验情况如下：

	exit(1) 或 abort()
		std::cout << "abc";		会输出

	_Exit(1) 或 kill -9 或 null地址访问
		std::cout << "abc";		不会输出。即使重定向到管道、文件都不会输出。
		std::cout << "abc\n";		会输出

示例代码：

	#include <iostream>
	#include <chrono>
	#include <thread>
	int 
	main () {
		std::cout << "abc";
		//std::this_thread::sleep_for(std::chrono::seconds(30));
		_Exit(1);
	}

### 指定整数输入输出的进制

如果输入/输出16进制数字，需要指定操作子std::hex，其它操作子包括dec、oct。
使用 `std::uppercase` 将16进制转换为大写字母。
（参考[2]，P1001，27.5.6.3 basefield manipulators）

这种修改是永久性的，如果输出hex后想再用dec输出，需要显式设置。

	#include <iostream>
	int 
	main () {
		int x = 0x123ABC;
		std::cout << std::hex << x << " " << std::uppercase << x << std::endl;
		std::cout << "input hex num: ";
		std::cin >> std::hex >> x;
		std::cout << "input is " << std::dec << x << std::endl;
		return 0;
	}

字符串转换为大些字母通常用 `std::transform` 函数：

	#include <string>
	#include <iostream>
	#include <algorithm>
	using namespace std;
	int 
	main () {
		string s;
		while(getline(std::cin, s)) {
			std::transform(s.begin(), s.end(), s.begin(), ::toupper);
			cout << s << endl;
		}
	}

## istringstream会拷贝字符串

```cpp
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
	std::string s = "123";
	std::istringstream ss(s);
	s[0] = 'a';
	std::cout << s << std::endl << ss.str() << std::endl;
}
```

## 读取一行字符串

标准库函数 `std::getline(istream, string, char)` 读入一行，可以使用行分隔符作为可选的第3个参数。

在 gcc5.3 中，getline 采用的是类似这种实现方式（libstdc++-v3/include/bits/basic_string.tcc）：
```cpp
string s;
while(一行未结束）{
	s += c;
}
```
如果对性能要求高，或者一行比较长，最好提前准备好足够的空间，如`line.reserve(8192);`

getline返回它的流参数（参考[1]，P78）。换行符被读入，但不保存。

getline这个函数貌似在以下头文件定义：string -> bits/basic_string.tcc 。

	#include <iostream>
	using namespace std;
	int 
	main () {
		string s;
		while(getline(cin, s, ' ')) {
			cout << "input: " << s << endl;
		}
	}

参考[3]，P677-678。

http://stackoverflow.com/questions/2910836/how-do-i-read-long-lines-from-a-text-file-in-c

## 忽略输入流的部分内容

`std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\t');`
http://www.cplusplus.com/reference/istream/istream/ignore/

## 关于clog的重定向

	http://stackoverflow.com/questions/2638654/redirect-c-stdclog-to-syslog-on-unix
	http://stackoverflow.com/questions/2638654/redirect-c-stdclog-to-syslog-on-unix
	http://stackoverflow.com/questions/9371238/why-is-reading-lines-from-stdin-much-slower-in-c-than-python

## 目录操作

C++标准库没有专门对目录遍历等操作的支持。简单的操作可以使用C语言`<dirent.h>`中定义的操作。
复杂的可以考虑boost的Filesystem库（[5]）。

http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c

## 定义errno常量的头文件

/usr/include/asm-generic/errno-base.h
/usr/include/asm-generic/errno.h

## 常用I/O流对象是RAII类

比如`ifstream, ofstream`都不必手动`close()`，析构函数会执行关闭操作。
https://stackoverflow.com/questions/748014/do-i-need-to-manually-close-an-ifstream
Effective Modern C++, P253




## 问题

C++能否混用iostream和stdio？特别是，如果要输出errno，比如perror呢？
标准CH27.4.1 第4段。要求对iostream和stdio进行同步。
标准CH27.4.2，预定义的流cin cout cerr都是与stdio tie的。所以，对这些流混合使用应该是没问题的。
http://stackoverflow.com/questions/9653751/cstdio-streams-vs-iostream-streams
http://www.cnblogs.com/Solstice/archive/2011/07/17/2108715.html
http://www.cplusplus.com/reference/cstdio/perror/
http://en.cppreference.com/w/c/string/byte/strerror
http://www.oracle.com/technetwork/articles/servers-storage-dev/mixingcandcpluspluscode-305840.html
https://www.daniweb.com/programming/software-development/threads/145998/can-stdio-h-be-in-one-program-with-iostream-h


std::cin >> i >> j; 
	如何忽略一部分内容，比如 %d %s %d ，忽略%s的输入。
类似sscanf可以参考 istringstream，或者string based stream。
>> 和 << 本质上是操作符重载函数，实际调用的还是函数吧。
连续的 >> ，是一次函数调用，还是两次？从汇编代码看，应该是两次函数调用。如果内容不匹配，会退回缓冲区。
表达式 std::cin >> i 返回值是其左侧运算符，即cin。cout也类似。

如果文件打开失败，如何获取错误信息？


C不支持泛型，对于栈、队列等常用数据类型，是否每次都需要自己重新写？

C一般如何解决log问题？log需要解决哪些问题：分级，debug？log4c/zlog似乎都太大？
如果是debug，自己写个简单函数是否足够？
程序中包含了stdio.h，调用了printf，链接的时候，gcc怎么知道该和哪个库文件链接呢？.o文件中可能已经不知道包含了哪个头文件了。gcc记录了每个库函数声明与库文件的映射关系？
在用Check单元测试时，需要将头文件提供给Check。如果某个函数作为实现细节不需要在最终的头文件中提供，但是又需要进行单元测试，如何处理？制作两个头文件，一个用于测试，一个用于交付？zabbix等开源软件是如何测试的？
一个.c源文件中定义的struct，能否被其它源文件引用（没有include）？

能否将多个c文件编译为一个o文件？


##参考资料

	[1] C++ Primer第5版（中文版）
	[2] C++ 标准2011
	[3] The C++ Standard Library, A Tutorial and Reference. Second Edition. Nicolai M. Josuttis. 
	[4] http://www.cplusplus.com/
	[5] http://www.boost.org/


