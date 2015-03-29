	.file	"code.c"
	.text
	.globl	_sum
	.def	_sum;	.scl	2;	.type	32;	.endef
_sum:
	movl	8(%esp), %eax
	addl	4(%esp), %eax
	addl	%eax, _accum
	ret
	.globl	_accum
	.bss
	.align 4
_accum:
	.space 4
	.ident	"GCC: (tdm64-1) 4.9.2"
