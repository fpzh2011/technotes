# C++的引用类型

试验环境：rhel7，gcc4.8.2

## 基本知识

引用的地址与被引用对象的地址相同，sizeof也相同。

	#include <iostream>
	int 
	main () {
		int i = 1024;
		int &refVal = i;
		std::cout << &i << " " << &refVal << " " << i << " " << refVal << " " << 
			sizeof(i) << " " << sizeof(refVal) << std::endl;
		return 0;
	}

## 指针与引用的异同

从下面的汇编代码可以看到，指针是实现引用语义的一种方式。
但是，引用天然是“常量指针”，引用关系一经初始化就不能更改；而且使用引用对象，不需要用 `*p` 来解除指针引用。

从语言的角度来看，引用不是指针。只是在编译器层面，可以用类似指针的方式实现引用语义。

***函数返回引用，与返回指针的区别是什么？**

	http://stackoverflow.com/questions/57483/what-are-the-differences-between-a-pointer-variable-and-a-reference-variable-in
	http://stackoverflow.com/questions/752658/is-the-practice-of-returning-a-c-reference-variable-evil
	http://stackoverflow.com/questions/114180/pointer-vs-reference

## 汇编分析

操作引用对象，与操作被引用对象，生成的指令不同。

访问refVal，是类似于指针的间接访问。而访问i，是直接访问。

示例代码 test.cc ：

	int 
	main () {
		int i = 1024;
		int &refVal = i;
		refVal = 3124;
		i = 1111;
		refVal = 2124;
		int *p = &i;
		int *q = &refVal;
		return 0;
	}

运行 `gcc -masm=intel -S test.cc` 后得到如下汇编代码：

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
		mov	rbp, rsp							; 将栈的地址放到rbp
		.cfi_def_cfa_register 6
		
		mov	DWORD PTR [rbp-28], 1024		; i在栈中的地址是 rbp-28
		
		; 初始化refVal，其实就是保存i的地址到栈。
		lea	rax, [rbp-28]					; 取引用的i的地址到rax，也就是将i的地址（指针）放到rax
		mov	QWORD PTR [rbp-8], rax		; 将i的地址从rax放到栈。rbp-8 位置保存的其实就是i的指针。
	
		mov	rax, QWORD PTR [rbp-8]		; 将i的地址放到rax。通过本行和下一行可以看到，访问refVal，是间接访问。而访问i，是对[rbp-28]直接访问。
		mov	DWORD PTR [rax], 3124			; 通过间接访问，设置引用i的值
		
		mov	DWORD PTR [rbp-28], 1111		; 直接访问，设置i的值
		
		mov	rax, QWORD PTR [rbp-8]
		mov	DWORD PTR [rax], 2124			; 通过间接访问，设置引用i的值
		
		lea	rax, [rbp-28]					; 取i的地址
		mov	QWORD PTR [rbp-16], rax		; p在栈的位置为 rbp-16
		
		mov	rax, QWORD PTR [rbp-8]		; 通过refVal取i的地址，从栈取
		mov	QWORD PTR [rbp-24], rax		; q在栈的位置为 rbp-24
		
		mov	eax, 0								; 设置返回值
		pop	rbp
		.cfi_def_cfa 7, 8
		ret
		.cfi_endproc
	.LFE0:
		.size	main, .-main
		.ident	"GCC: (GNU) 4.8.2 20140120 (Red Hat 4.8.2-16)"
		.section	.note.GNU-stack,"",@progbits

