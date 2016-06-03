# C基础说明

C语言最重要的概念是什么？指针与**内存管理**。

## include

### 默认搜索路径
`#include <stdio.h>` 只在系统目录查找头文件。

`#include "zjh.h"` 会先到包含源代码的目录查找头文件，如果没找到，再到系统目录查找头文件。一般用于用户定义的头文件。

系统路径如：/usr/include:/usr/local/include:/usr/lib/gcc-lib/i386-linux/2.95.2/include。不同版本的系统也不一样。

### 命令行指定参数

`gcc -I /home/my/include test.c`

### 环境变量

gcc也会尝试搜索以下两个变量指定的目录：C_INCLUDE_PATH 和 CPLUS_INCLUDE_PATH

## 预处理指令

`#pragma once` https://en.wikipedia.org/wiki/Pragma_once

## 算法与逻辑

先处理简单直接的逻辑，比如空白、非空白的字符；
充分利用else的潜台词，比如if(空白)，那么else肯定就是非空白了；
对状态、操作等进行拆分，一共要处理几个状态、操作，不同情况下都如何处理。

## C语言抽象数据类型与泛型库

一般认为Gnome提供的glib是一个不错的泛型库。

	stackoverflow.com/questions/668501/are-there-any-open-source-c-libraries-with-common-data-structures
	stackoverflow.com/questions/3039513/type-safe-generic-data-structures-in-plain-old-c
	sglib.sourceforge.net
	stackoverflow.com/questions/2956387/is-there-a-good-collection-library-for-c-language
	programmers.stackexchange.com/questions/116650/is-there-any-boost-equivalent-library-for-c
	libcello.org
	home.gna.org/gdsl/
	andreinc.net/2010/09/30/generic-data-structures-in-c/
	onlinelibrary.wiley.com/doi/10.1002/spe.4380200303/abstract

## C语言最佳实践

	http://programmers.stackexchange.com/questions/118253/oo-best-practices-for-c-programs
	stackoverflow.com/questions/2492446/design-principles-best-practices-and-design-patterns-for-c-or-procedural-progr
	http://embeddedgurus.com/barr-code/2010/11/what-belongs-in-a-c-h-header-file/
	https://en.wikibooks.org/wiki/C_Programming/Common_practices
	http://en.wikipedia.org/wiki/MISRA_C
	http://www.planetpdf.com/codecuts/pdfs/ooc.pdf  http://book.douban.com/subject/2237446/
	http://sunxiunan.com/?p=1661
	https://github.com/mcinglis/c-style

## C语言标准

C标准文本资料

	www.open-std.org/jtc1/sc22/wg14/www/standards
	C11 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
	C99 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
	http://www.open-std.org/jtc1/sc22/wg14/www/docs/C99RationaleV5.10.pdf

C标准简介

	https://zh.wikipedia.org/zh-cn/ANSI_C
	stackoverflow.com/questions/17206568/what-is-the-difference-between-c-c99-ansi-c-and-gnu-c-a-general-confusion-reg
	https://gcc.gnu.org/onlinedocs/gcc/Standards.html

## 日常习惯

编写完一个源文件后，检查不需要暴露接口的函数，是否都是static。


##参考资料

	[1] C程序设计语言（K&R第二版）,P75， ISNB：978-7-111-12806-9
	[2] https://gcc.gnu.org/onlinedocs/cpp/Search-Path.html
	[3] http://blog.csdn.net/chosen0ne/article/details/7210946

	C程序设计语言  http://book.douban.com/subject/1139336/

	C专家编程  http://book.douban.com/subject/2377310/

	C和指针  http://book.douban.com/subject/3012360/

	C陷阱与缺陷  http://book.douban.com/subject/2778632/

	C程序设计的抽象思维  http://book.douban.com/subject/10754300/
	C语言程序设计现代方法  http://book.douban.com/subject/2280547/


## 问题

C不支持泛型，对于栈、队列等常用数据类型，是否每次都需要自己重新写？

C一般如何解决log问题？log需要解决哪些问题：分级，debug？log4c/zlog似乎都太大？
如果是debug，自己写个简单函数是否足够？
程序中包含了stdio.h，调用了printf，链接的时候，gcc怎么知道该和哪个库文件链接呢？.o文件中可能已经不知道包含了哪个头文件了。gcc记录了每个库函数声明与库文件的映射关系？
在用Check单元测试时，需要将头文件提供给Check。如果某个函数作为实现细节不需要在最终的头文件中提供，但是又需要进行单元测试，如何处理？制作两个头文件，一个用于测试，一个用于交付？zabbix等开源软件是如何测试的？
一个.c源文件中定义的struct，能否被其它源文件引用（没有include）？

能否将多个c文件编译为一个o文件？
