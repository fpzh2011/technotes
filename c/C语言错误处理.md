# C语言错误处理

C语言不支持异常处理机制（抛出、捕获），所以调用者需要时刻注意检查函数的返回值、并处理异常情况。

这个链接给列出一些错误处理的原则：https://dlang.org/spec/errors.html

自己编写程序时，如何告诉用户出错情况呢？总结有以下几种方式。

## 返回值

最简单、直接的方式就是通过函数返回值报告出错情况。

如果函数正常执行也需要返回值，比如返回字符串，可以通过返回NULL来通知出错。
如果返回正整数，可以通过返回 -1 通知出错。

但如果出错的原因不止一种，如何通知用户错误详情？

一种方法是将错误作为返回值，而结果作为指针参数；或者传入一个int指针用于返回错误代码；或者使用struct返回多个数据项；或者使用可全局访问的errno。

## struct 

另一种同时传递结果和错误信息的方法是使用struct，其中用一个int字段记录错误信息，可以复用errno的代码。

使用struct传递错误信息比较简单，但也有几个问题：

1 可能影响效率。

2 语义不那么清晰。到底哪个字段是错误，哪个字段是有用的信息，不那么直接。

3 不同的返回类型要定义不同的返回结构，如int、long、指针等。或者使用联合union。

## errno

`errno`定义在`<errno.h>`中，在线程中可以全局访问。
我们不应该自己声明errno，只需要包含头文件，就可以访问errno了。

rhel7中的错误码文字注释在 /usr/include/asm-generic 的 errno.h 和 errno-base.h 中。
或者使用`man 3 errno`。

关于errno有以下几点需要注意：

1 “如果没有出错，其值不会被清除。”
即，如果系统调用或库函数正确执行的话，errno的值是不会被清零（注意这里是不会被清零，不是不会被改变）。

2 “任何函数都不会将errno设置为0，而且在中定义的所有常量都不为0。”

3 上面两条并不意味着正确执行的库函数不会修改errno的值？

假若执行函数A的时候发生了错误errno被改变，接下来直接执行函数B。
如果函数B正确执行的话，errno还保留函数A发生错误时被设置的值。

所以，在利用errno之前，最好先对函数的返回值进行判断，看是否发生了错误，并利用errno判断发生了什么错误。
如果函数返回值表明没有发生错误（比如指针不为NULL），就不用检查errno。
如果函数返回值表明发生错误，又**需要通过errno来判断出错的具体原因，则在调用函数之前必须手动将errno清零，在调用函数后可能还需要暂存errno的值。**

*如何使用errno传递自定义的错误码？用负值？*

### 应用代码能利用errno传递错误信息吗？

一般认为，应用代码也可以利用errno传递错误信息。但要根据具体场景，在多种错误信息传递方式中综合权衡。

如果应用代码中使用errno传递错误信息，根据惯例，应该只在错误发生时设置errno的值；如果没有发生异常，不改变errno的值。

参考如下资料：http://stackoverflow.com/questions/9856822/should-i-set-errno

### 显示错误信息的库函数

`void perror(char *)` 用于向stderr输出errno表示的错误信息，可以传入一个字符串表明操作背景，如argv[0]。

`char* strerror(int)` 根据传入的错误码返回描述字符串。该函数可能会根据locale作语言转换。
调用者不能修改返回的字符串内容；再调用strerror后，原来的字符串内容可能会被修改。
**该函数并不保证是线程安全的。**strerror可能是这样实现的：自己维护一个`static char*`指针，本次调用的错误号如果与上次调用不同，就free并重新创建一个字符串。

`int strerror_r(int errnum, char *buf, size_t buflen)` 是POSIX/XSI规范定义的函数，与strerror()类似。它接受一个字符串缓冲区，所以是线程安全的。

### 关于errno的深入探讨

在多线程出现以前，errno被实现为全局变量。

多线程出现后，如果仍用全局变量访问errno，很容易产生冲突。一个线程执行出错、写了errno，结果被另一个线程检测到了。
为了兼容传统库函数，errno被规定为线程的私有数据。

多线程环境下的errno通常被实现为由函数返回的指针，该指针指向线程私有的errno变量。Unix下的实现通常是：

	extern int *__errno_location(void);
	#define errno (*__errno_location())

上述代码中，第一行声明了一个无参函数，该函数的返回值是一个int指针。
第二行代码表明，errno被定义为该指针指向的int变量。即，对errno的引用，会先调用函数，再引用该函数返回的指针。

### errno的参考资料

	APUE，CH1.7，CH11.2 P308，CH12.7 P358
	C11标准，CH7.5，Errors <errno.h>
	http://blog.hehehehehe.cn/a/10831.htm
	http://stackoverflow.com/questions/12945486/is-errno-legal-c
	man 3 strerror
	man 3 errno

## 其它错误处理方式

### signal

信号通常用于通知严重错误的发生。信号的处理比较复杂，要考虑信号延迟、屏蔽等问题，不建议大面积用于错误通知。

一些信号的触发可能会导致程序终止，所以要提前做好资源清理。

## setjmp

参考APUE的CH7.10，以及 https://en.wikibooks.org/wiki/C_Programming/Error_handling

## 常用的错误预查

引用NULL指针。

除数为0。

"segmentation violation" or "segmentation fault" 的常见原因：http://web.mit.edu/10.001/Web/Tips/tips_on_segmentation.html

关于内存错误处理的一些建议 
http://eli.thegreenplace.net/2009/10/30/handling-out-of-memory-conditions-in-c

freetype关于可靠的C编程的一篇文章 
http://freetype.sourceforge.net/david/reliable-c.html

## 参考资料

	APUE
	https://en.wikibooks.org/wiki/C_Programming/Error_handling
	http://stackoverflow.com/questions/385975/error-handling-in-c-code
	https://dlang.org/spec/errors.html
	http://www.on-time.com/ddj0011.htm

