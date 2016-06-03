# C语言中的字符串

本文档仅参考ANSI C，以及后续的C标准。

## 字符串字面值

在ANSI标准C中，字符串字面值是常量，字符串的存储类型是静态的（[1],P170），对字符串字面值的修改，行为是未定义的。在rhel7中，会产生段错误（如下面的示例代码）。

	//test1.c
	#include <stdio.h>
	void 
	main () {
		char *p = "this is a static const string\n";
		printf("%s", p);
		p[1] = '2';	//系统发生段错误
		printf("%s", p);	//系统在上个语句异常退出，本行语句不会被执行
	}

但是，这种情况下，可以对指针p赋值，比如 `p = NULL;`。

## 字符数组初始化

但是，字符数组可以用字符串字面值初始化。K&R（[1],P239）中说初始化不会带末尾的'\0'，但P73中的例子表明是带末尾的'\0'的，rhel7的GCC实现也是带末尾的'\0'的。如下面的代码可以正常执行：

	//test2.c
	#include <stdio.h>
	void 
	main () {
		char a[] = "1234567890";
		a[1] = '2';
		return (int)(a[1]);
	}

与字符指针相反，这里可以对字符数组中的元素赋值，但不能对a赋值，如`a = NULL;`。因为a是一个编译时的偏移常量，不是左值。

另外可以参考K&R（[1],P90）中的例子。

## 字符数组初始化的机制

执行 `gcc -S -masm=intel test2.c` 得到汇编代码如下：

		.file	"teststringsimple.c"
		.intel_syntax noprefix
		.text
		.globl	main
		.type	main, @function
	main:
	.LFB0:
		.cfi_startproc
		push	rbp									; 保存rbp，这里就占了栈的4个字节
		.cfi_def_cfa_offset 16
		.cfi_offset 6, -16
		mov	rbp, rsp								; rbp现在指向栈顶。从 [rbp-16], rax 可以看到，字符串从rbp-16开始
		.cfi_def_cfa_register 6
		movabs	rax, 4050765991979987505		; 4050765991979987505 的16进制表示是 0x3837363534333231 ，即字符串中的87654321。先放到rax，再放到栈中。
		mov	QWORD PTR [rbp-16], rax
		mov	WORD PTR [rbp-8], 12345			; 12345 的16进制表示是 0x3039 ，即字符串中的90。
		mov	BYTE PTR [rbp-6], 0				; 初始化字符串末尾的'\0'
		mov	BYTE PTR [rbp-15], 50				; 50的16进制是0x32
		movzx	eax, BYTE PTR [rbp-15]
		movsx	eax, al
		pop	rbp
		.cfi_def_cfa 7, 8
		ret
		.cfi_endproc
	.LFE0:
		.size	main, .-main
		.ident	"GCC: (GNU) 4.8.2 20140120 (Red Hat 4.8.2-16)"
		.section	.note.GNU-stack,"",@progbits

从上面可以看到，系统会在栈中为字符数组分配空间，并根据代码中的字符串字面值初始化字符数组。

## 参考资料

[1] C程序设计语言（K&R第二版） ISNB：978-7-111-12806-9

