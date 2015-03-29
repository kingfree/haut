	.file	"exchange.c"
	.text
	.globl	_exchange
	.def	_exchange;	.scl	2;	.type	32;	.endef
_exchange:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, (%eax)
	movl	-4(%ebp), %eax
	leave
	ret
	.ident	"GCC: (tdm64-1) 4.9.2"
