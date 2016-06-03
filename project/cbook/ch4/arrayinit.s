	.file	"arrayinit.c"
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
g:
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
	mov	ecx, 0
	mov	eax, 32
	and	eax, -4
	mov	ebx, eax
	mov	eax, 0
.L4:
	mov	DWORD PTR [edx+eax], ecx
	add	eax, 4
	cmp	eax, ebx
	jb	.L4
	add	edx, eax
	mov	eax, DWORD PTR [ebp-8]
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
