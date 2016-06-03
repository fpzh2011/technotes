# C语言中的数组初始化

## 数组初始化

数组初始化例子：

	int days[] =   {31,...,31};
	int days2[12] = {31,...,31};
	char p[] = "test";

K&R（[1],P239）中说，“如果（数组）初始化表达式的个数比数组元素数少，则对外部变量、静态变量和**自动变量**来说，”没有初始化表达式的元素将被初始化为0。

这里关键是“自动变量”。

下面C代码的汇编代码表明，形如 `int a[8];` 这样的代码，只声明、不初始化，汇编代码中也没有初始化指令。

形如 `int a[8] = {};` 这样的初始化代码，汇编后会自动生成数据初始化为0的代码。但是，**C99标准禁止这种初始化方式**，`gcc -std=c99 -Wpedantic`会有编译警告。

但是，`int a[8] = {0};` 或 `int a[8] = {0,};` 是可以无警告编译的。（参考资料[1]，P196-197）

C代码：

	// arrayinit.c
	int 
	f (void) {
		int a[8];
		return a[3];
	}

	int 
	g (void) {
		int a[8] = {};
		return a[7];
	}

运行 `gcc -S -masm=intel -m32 arrayinit.c` 后生成的汇编代码：

		.file	"arrayinit.c"
		.intel_syntax noprefix
		.text
		.globl	f
		.type	f, @function
	f:											; 函数f
	.LFB0:
		.cfi_startproc
		push	ebp
		.cfi_def_cfa_offset 8
		.cfi_offset 5, -8
		mov	ebp, esp
		.cfi_def_cfa_register 5
		sub	esp, 32
		mov	eax, DWORD PTR [ebp-20]
		leave
		.cfi_restore 5
		.cfi_def_cfa 4, 4
		ret
		.cfi_endproc
	.LFE0:
		.size	f, .-f
		.globl	g
		.type	g, @function
	g:											; 函数g
	.LFB1:
		.cfi_startproc
		push	ebp
		.cfi_def_cfa_offset 8
		.cfi_offset 5, -8
		mov	ebp, esp
		.cfi_def_cfa_register 5
		push	ebx
		sub	esp, 32
		.cfi_offset 3, -12
		lea	edx, [ebp-36]
		mov	ecx, 0								; 将初始化的值0保存到ecx
		mov	eax, 32							; 计算数组地址
		and	eax, -4
		mov	ebx, eax
		mov	eax, 0
	.L4:
		mov	DWORD PTR [edx+eax], ecx		; 循环初始化数组值
		add	eax, 4
		cmp	eax, ebx
		jb	.L4									; 循环结束
		add	edx, eax
		mov	eax, DWORD PTR [ebp-8]		; 设置返回值
		add	esp, 32
		pop	ebx
		.cfi_restore 3
		pop	ebp
		.cfi_restore 5
		.cfi_def_cfa 4, 4
		ret
		.cfi_endproc
	.LFE1:
		.size	g, .-g
		.ident	"GCC: (GNU) 4.8.2 20140120 (Red Hat 4.8.2-16)"
		.section	.note.GNU-stack,"",@progbits

## 参考资料

[1] C程序设计语言（K&R第二版） ISNB：978-7-111-12806-9

