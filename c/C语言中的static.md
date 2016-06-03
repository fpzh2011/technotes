# C语言中的static

## static简介

根据参考资料[1]（Ch4.6，P79-71），C语言中的static可以修饰3类对象：外部变量、函数和自动变量。

static不仅可以影响自动变量的存储，更重要的是，**可以限定这些对象的作用域，从而便于程序隐藏实现细节**。

### 外部变量

变量声明中的extern是指在函数外部，而不是在该文件外部。在源文件的开头，也可以用extern声明在文件后面才定义的外部变量。

没有static限定的外部变量，如果要在其它源文件中被引用，只能在其它源文件中显式声明（通过include头文件，或单独显式声明）。

如果用static限定外部变量，该变量的作用域就被限定为被编译源文件的剩余部分。在其它源文件中将不能引用该外部变量。

### 函数

如果把函数`f`声明为static类型，则`f`只能在本文件中使用，其它文件无法访问`f`函数。

### 自动变量

static类型的自动变量（局部变量）是一种只能在某个特定函数中使用但一直占据存储空间的变量。

## 代码示例

运行 `gcc -S -masm=intel -m32 test.c` 得到汇编代码。观察汇编代码，发现下面C代码中的全局变量、静态变量的初始化语句，在汇编代码中其实并没有对应的汇编指令，只是在相应数据段中有设置数据的“伪指令”。

所以，静态自动变量只能被初始化一次。

	//test.c
	int g = 'a';

	void 
	f (int v) {
		g = 'b';
		static int s1 = 'c';
		static int s2 = 'd';
		//static int s2 = v; //编译提示“错误：初始值设定元素不是常量”
		s2 = v;
	}

附：汇编代码

		.file	"globalstatic.c"
		.intel_syntax noprefix
		.globl	g
		.data
		.align 4
		.type	g, @object
		.size	g, 4
	g:
		.long	97								; g = 'a'
		.text
		.globl	f
		.type	f, @function
	f:
	.LFB0:
		.cfi_startproc
		push	ebp
		.cfi_def_cfa_offset 8
		.cfi_offset 5, -8
		mov	ebp, esp
		.cfi_def_cfa_register 5
		mov	DWORD PTR g, 98
		mov	eax, DWORD PTR [ebp+8]
		mov	DWORD PTR s2.1566, eax	; s2 = v
		pop	ebp
		.cfi_restore 5
		.cfi_def_cfa 4, 4
		ret
		.cfi_endproc
	.LFE0:
		.size	f, .-f
		.data
		.align 4
		.type	s2.1566, @object
		.size	s2.1566, 4
	s2.1566:
		.long	100							; s2 = 'd'
		.align 4
		.type	s1.1565, @object
		.size	s1.1565, 4
	s1.1565:
		.long	99							;  s1 = 'c'
		.ident	"GCC: (GNU) 4.8.2 20140120 (Red Hat 4.8.2-16)"
		.section	.note.GNU-stack,"",@progbits

## 参考资料

[1] C程序设计语言（K&R第二版） ISNB：978-7-111-12806-9

