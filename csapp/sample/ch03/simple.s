	.file	"simple.c"
	.text
	.globl	_simple
	.def	_simple;	.scl	2;	.type	32;	.endef
_simple:
	movl	4(%esp), %edx
	movl	(%edx), %eax
	addl	8(%esp), %eax
	movl	%eax, (%edx)
	ret
	.ident	"GCC: (tdm64-1) 4.9.2"
