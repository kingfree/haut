	.file	"t121.cpp"
	.section	.text$_Z5scanfPKcz,"x"
	.linkonce discard
	.globl	_Z5scanfPKcz
	.def	_Z5scanfPKcz;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z5scanfPKcz
_Z5scanfPKcz:
.LFB2:
	subq	$56, %rsp
	.seh_stackalloc	56
	.seh_endprologue
	movq	%rdx, 72(%rsp)
	movq	%r8, 80(%rsp)
	movq	%r9, 88(%rsp)
	leaq	72(%rsp), %rdx
	movq	%rdx, 40(%rsp)
	call	__mingw_vscanf
	addq	$56, %rsp
	ret
	.seh_endproc
	.section	.text$_Z6printfPKcz,"x"
	.linkonce discard
	.globl	_Z6printfPKcz
	.def	_Z6printfPKcz;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z6printfPKcz
_Z6printfPKcz:
.LFB8:
	subq	$56, %rsp
	.seh_stackalloc	56
	.seh_endprologue
	movq	%rdx, 72(%rsp)
	movq	%r8, 80(%rsp)
	movq	%r9, 88(%rsp)
	leaq	72(%rsp), %rdx
	movq	%rdx, 40(%rsp)
	call	__mingw_vprintf
	addq	$56, %rsp
	ret
	.seh_endproc
	.section	.text$_Z2fnx,"x"
	.linkonce discard
	.globl	_Z2fnx
	.def	_Z2fnx;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z2fnx
_Z2fnx:
.LFB153:
	subq	$104, %rsp
	.seh_stackalloc	104
	movaps	%xmm6, 80(%rsp)
	.seh_savexmm	%xmm6, 80
	.seh_endprologue
	pxor	%xmm6, %xmm6
	cvtsi2sdq	%rcx, %xmm6
	movapd	%xmm6, %xmm1
	movsd	.LC0(%rip), %xmm0
	call	pow
	movsd	%xmm0, 40(%rsp)
	movapd	%xmm6, %xmm1
	movsd	.LC1(%rip), %xmm0
	call	pow
	movsd	%xmm0, 48(%rsp)
	fldl	48(%rsp)
	fldl	40(%rsp)
	fsubp	%st, %st(1)
	fdivl	.LC2(%rip)
	fnstcw	70(%rsp)
	movzwl	70(%rsp), %eax
	orb	$12, %ah
	movw	%ax, 68(%rsp)
	fldcw	68(%rsp)
	fistpq	56(%rsp)
	fldcw	70(%rsp)
	movq	56(%rsp), %rax
	movaps	80(%rsp), %xmm6
	addq	$104, %rsp
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC4:
	.ascii "%lld %lld %lld\0"
.LC5:
	.ascii "%lld\12\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB154:
	subq	$40, %rsp
	.seh_stackalloc	40
	.seh_endprologue
	call	__main
	leaq	p(%rip), %r9
	leaq	m(%rip), %r8
	leaq	n(%rip), %rdx
	leaq	.LC4(%rip), %rcx
	call	_Z5scanfPKcz
	movq	n(%rip), %rcx
	call	_Z2fnx
	movq	%rax, f1(%rip)
	movq	n(%rip), %rax
	leaq	1(%rax), %rcx
	call	_Z2fnx
	movq	%rax, f2(%rip)
	movq	m(%rip), %rcx
	call	_Z2fnx
	movq	%rax, %rcx
	movq	%rax, f3(%rip)
	movq	p(%rip), %r8
	movq	f1(%rip), %rax
	cqto
	idivq	%rcx
	movq	%rdx, %rax
	cqto
	idivq	%r8
	movq	%rdx, %r9
	movq	f2(%rip), %rax
	cqto
	idivq	%rcx
	movq	%rdx, %rax
	cqto
	idivq	%r8
	addq	%rdx, %r9
	movq	%rcx, %rax
	cqto
	idivq	%r8
	leaq	-1(%r9,%rdx), %rax
	cqto
	idivq	%rcx
	movq	%rdx, %rax
	cqto
	idivq	%r8
	movq	%rdx, s(%rip)
	leaq	.LC5(%rip), %rcx
	call	_Z6printfPKcz
	movl	$0, %eax
	addq	$40, %rsp
	ret
	.seh_endproc
	.globl	s
	.bss
	.align 8
s:
	.space 8
	.globl	p
	.align 8
p:
	.space 8
	.globl	m
	.align 8
m:
	.space 8
	.globl	n
	.align 8
n:
	.space 8
	.globl	f3
	.align 8
f3:
	.space 8
	.globl	f2
	.align 8
f2:
	.space 8
	.globl	f1
	.align 8
f1:
	.space 8
	.section .rdata,"dr"
	.align 8
.LC0:
	.long	-1684540248
	.long	1073341303
	.align 8
.LC1:
	.long	925886800
	.long	-1075591441
	.align 8
.LC2:
	.long	-1684540248
	.long	1073865591
	.ident	"GCC: (tdm64-1) 4.9.2"
	.def	__mingw_vscanf;	.scl	2;	.type	32;	.endef
	.def	__mingw_vprintf;	.scl	2;	.type	32;	.endef
	.def	pow;	.scl	2;	.type	32;	.endef
