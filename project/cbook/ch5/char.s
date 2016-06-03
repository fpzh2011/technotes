	.file	"char.c"
	.intel_syntax noprefix
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
	sub	esp, 24
	mov	eax, DWORD PTR gs:20
	mov	DWORD PTR [ebp-12], eax
	xor	eax, eax
	movzx	eax, BYTE PTR [ebp-18]
	add	eax, 1
	mov	BYTE PTR [ebp-18], al
	movzx	eax, BYTE PTR [ebp-15]
	mov	BYTE PTR [ebp-17], al
	movzx	eax, BYTE PTR [ebp-16]
	add	eax, 7
	mov	BYTE PTR [ebp-16], al
	movzx	eax, BYTE PTR [ebp-13]
	movsx	eax, al
	mov	DWORD PTR [ebp-24], eax
	mov	BYTE PTR [ebp-14], 97
	mov	eax, 0
	mov	edx, DWORD PTR [ebp-12]
	xor	edx, DWORD PTR gs:20
	je	.L3
	call	__stack_chk_fail
.L3:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	f, .-f
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
