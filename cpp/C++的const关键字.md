# C++的const关键字

试验环境：rhel7，gcc4.8.2

## const全局变量的作用域

默认情况下，const对象被设定为仅在文件内有效。（参考[1]，P54）
类似于C中的static全局变量。
**什么情况下作用域会超过文件范围呢？**添加`extern`(P54)？

test.cc

	#include <iostream>
	void f();
	const int i = 1;
	int 
	main () {
		std::cout << "const in main: " << i << std::endl;
		f();
		std::cout << "const in main: " << i << std::endl;
		return 0;
	}

test1.cc

	#include <iostream>
	const int i = 2;
	void 
	f () {
		std::cout << "const in f: " << i << std::endl;
	}

运行 `g++ test.cc test1.cc; ./a.out` 输出结果为：

	const in main: 1
	const in f: 2

### const全局变量的汇编分析

是否可以这样看，编译器并没有把全局const对象放到可以全局访问的符号表中，所以外部其实无法访问。

示例代码：

	const int i = 1234;
	const int i2 = 1234;
	int j = 5678;
	int j2 = 5678;

	int
	f () { return 7777; }

运行 `gcc -masm=intel -S test.cc` 后得到如下汇编代码。
从中可以看到，const全局变量的符号前面带有下划线，而且与普通全局变量位于不同的内存区域。

		.file	"test.cc"
		.intel_syntax noprefix
		.globl	j
		.data
		.align 4
		.type	j, @object
		.size	j, 4
	j:
		.long	5678						; 普通全局变量，有.globl标记
		.globl	j2
		.align 4
		.type	j2, @object
		.size	j2, 4
	j2:
		.long	5678
		.text
		.globl	_Z1fv
		.type	_Z1fv, @function
	_Z1fv:
	.LFB0:									; 函数f
		.cfi_startproc
		push	rbp
		.cfi_def_cfa_offset 16
		.cfi_offset 6, -16
		mov	rbp, rsp
		.cfi_def_cfa_register 6
		mov	eax, 7777
		pop	rbp
		.cfi_def_cfa 7, 8
		ret
		.cfi_endproc
	.LFE0:
		.size	_Z1fv, .-_Z1fv
		.section	.rodata
		.align 4
		.type	_ZL1i, @object
		.size	_ZL1i, 4
	_ZL1i:									; const全局变量
		.long	1234
		.align 4
		.type	_ZL2i2, @object
		.size	_ZL2i2, 4
	_ZL2i2:
		.long	1234
		.ident	"GCC: (GNU) 4.8.2 20140120 (Red Hat 4.8.2-16)"
		.section	.note.GNU-stack,"",@progbits

## const常量编译时替换

当以编译时初始化的方式定义一个const变量时，编译过程中会替换该变量为对应的值。（参考[1]，P54）

如下示例代码 test.cc ：

	int 
	main () {
		const int i = 1024;
		return i + 1;
	}

运行 `gcc -masm=intel -S test.cc` 后得到如下汇编代码。

从中可以看到两点：一是确实存在编译时常量替换；二是和define不同，const常量也是分配了存储空间的对象。

		.file	"test1.cc"
		.intel_syntax noprefix
		.text
		.globl	main
		.type	main, @function
	main:
	.LFB0:
		.cfi_startproc
		push	rbp
		.cfi_def_cfa_offset 16
		.cfi_offset 6, -16
		mov	rbp, rsp
		.cfi_def_cfa_register 6
		mov	DWORD PTR [rbp-4], 1024	; 初始化i的值。i是分配了存储空间的对象。
		mov	eax, 1025						; 编译时替换常量i的值
		pop	rbp
		.cfi_def_cfa 7, 8
		ret
		.cfi_endproc
	.LFE0:
		.size	main, .-main
		.ident	"GCC: (GNU) 4.8.2 20140120 (Red Hat 4.8.2-16)"
		.section	.note.GNU-stack,"",@progbits

##参考资料

	[1] C++ Primer第5版（中文版）

