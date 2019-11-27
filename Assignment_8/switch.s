	.file	"switch.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"hello "
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB39:
	.cfi_startproc
	endbr64
	subq	$1960, %rsp
	.cfi_def_cfa_offset 1968
	movq	%fs:40, %rax
	movq	%rax, 1944(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rdi
	call	getcontext@PLT
	endbr64
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movq	%rsp, %rsi
	leaq	976(%rsp), %rdi
	call	swapcontext@PLT
	endbr64
	movq	1944(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L8
	xorl	%eax, %eax
	addq	$1960, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L8:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE39:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
