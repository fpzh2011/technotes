# 内存拷贝函数memcpy、memmove、strcpy

## memcpy

	#include <string.h>
	void *memcpy(void *dest, const void *src, size_t n);

memcpy将src中的n个字节拷贝到dest。**src与dest不能有重叠。**

### 类似函数

strcpy等。

### 实现优化

mem函数可以通过汇编实现，利用高效的内存复制指令、数据预取、缓存等加速执行速度。

在很多编译器中，memcpy是一个intrinsic函数，也就是说，这个函数是由编译器实现的。它比inline函数更容易被编译时优化。
编译器可以根据memcpy的参数是常量还是变量做出多种版本，达到最佳的性能。这一点，用inline或者template的技巧都无法办到。

如gcc利用`cld rep movsb`等汇编指令优化memcpy的实现。

## memmove

	#include <string.h>
	void *memmove(void *dest, const void *src, size_t n);

memcpy将src中的n个字节拷贝到dest。src与dest可以有重叠。

## 比较

对于库函数来说，由于没有办法知道传递给他的内存区域的情况，所以应该使用memmove()函数。通过这个函数，可以保证不会出现任何内存块重叠问题。
而对于应用程序来说，因为代码“知道”两个内存块不会重叠，所以可以安全地使用memcpy()函数。

在rhel7，gcc 4.8.2下，没有发现memcpy的重叠问题。但是为安全期间，如果有重叠时，仍应使用memmove。

可以反汇编ibc的静态库或动态库，分析memcpy的实现。

	#include <stdio.h>
	#include <string.h>
	
	char*
	mymemcpy(char *dest, const char *src, int n) {
		while(n--) {
			*dest++ = *src++;
		}
		return dest;
	}
	
	int 
	f() {
		int i = 4;
		return i;
	}
	
	int 
	main () {
		volatile int n = f();
		char s[] = "1234567890"; 
		char t[] = "1234567890"; 
		char q[] = "1234567890";
		memcpy(s+2, s, n);		//即使加长字符串、n值，也没有出现异常情况。
		memmove(t+2, t, n);
		mymemcpy(q+2, q, n);
		printf("%s\n%s\n%s\n", s+2, t+2, q+2);
		return 0;
	}


## 参考资料

	http://blog.chinaunix.net/uid-22780578-id-3346391.html
	http://www.cnblogs.com/mydomain/archive/2013/04/16/3023278.html
	http://blog.codingnow.com/2005/10/vc_memcpy.html
	http://zhidao.baidu.com/question/130424698.html
	glibc实现
