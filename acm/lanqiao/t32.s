	.file	"t32.cpp"
	.text
	.p2align 4,,15
	.def	___tcf_1;	.scl	3;	.type	32;	.endef
___tcf_1:
	pushl	%ebx
	movl	$_w+98292, %ebx
	subl	$24, %esp
	.p2align 4,,10
L3:
	movl	(%ebx), %eax
	testl	%eax, %eax
	je	L2
	movl	%eax, (%esp)
	call	__ZdlPv
L2:
	subl	$12, %ebx
	cmpl	$_w-12, %ebx
	jne	L3
	addl	$24, %esp
	popl	%ebx
	ret
	.p2align 4,,15
	.def	___tcf_0;	.scl	3;	.type	32;	.endef
___tcf_0:
	pushl	%ebx
	movl	$_v+98292, %ebx
	subl	$24, %esp
	.p2align 4,,10
L12:
	movl	(%ebx), %eax
	testl	%eax, %eax
	je	L11
	movl	%eax, (%esp)
	call	__ZdlPv
L11:
	subl	$12, %ebx
	cmpl	$_v-12, %ebx
	jne	L12
	addl	$24, %esp
	popl	%ebx
	ret
	.section	.text$_Z5scanfPKcz,"x"
	.linkonce discard
	.p2align 4,,15
	.globl	__Z5scanfPKcz
	.def	__Z5scanfPKcz;	.scl	2;	.type	32;	.endef
__Z5scanfPKcz:
	subl	$28, %esp
	leal	36(%esp), %eax
	movl	%eax, 4(%esp)
	movl	32(%esp), %eax
	movl	%eax, (%esp)
	call	___mingw_vscanf
	addl	$28, %esp
	ret
	.section	.text$_Z6printfPKcz,"x"
	.linkonce discard
	.p2align 4,,15
	.globl	__Z6printfPKcz
	.def	__Z6printfPKcz;	.scl	2;	.type	32;	.endef
__Z6printfPKcz:
	subl	$28, %esp
	leal	36(%esp), %eax
	movl	%eax, 4(%esp)
	movl	32(%esp), %eax
	movl	%eax, (%esp)
	call	___mingw_vprintf
	addl	$28, %esp
	ret
	.text
	.p2align 4,,15
	.globl	__Z3dfsi
	.def	__Z3dfsi;	.scl	2;	.type	32;	.endef
__Z3dfsi:
	pushl	%ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	xorl	%ebx, %ebx
	subl	$28, %esp
	movl	48(%esp), %eax
	leal	(%eax,%eax,2), %edx
	movb	$1, _visited(%eax)
	leal	0(,%edx,4), %eax
	movl	%edx, %esi
	leal	_v(%eax), %ebp
	movl	_v(%eax), %eax
	movl	4(%ebp), %edi
	subl	%eax, %edi
	cmpl	$3, %edi
	jg	L27
	jmp	L22
	.p2align 4,,10
L24:
	addl	$4, %ebx
	cmpl	%edi, %ebx
	je	L22
L29:
	movl	0(%ebp), %eax
L27:
	movl	(%eax,%ebx), %eax
	cmpb	$0, _visited(%eax)
	jne	L24
	movl	_w(,%esi,4), %ecx
	movl	48(%esp), %edx
	movl	(%ecx,%ebx), %ecx
	addl	_d(,%edx,4), %ecx
	addl	$4, %ebx
	movl	%eax, (%esp)
	movl	%ecx, _d(,%eax,4)
	call	__Z3dfsi
	cmpl	%edi, %ebx
	jne	L29
L22:
	addl	$28, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
	.p2align 4,,15
	.globl	__Z4initv
	.def	__Z4initv;	.scl	2;	.type	32;	.endef
__Z4initv:
	movl	$_d, %eax
	.p2align 4,,10
L31:
	movl	$0, (%eax)
	addl	$4, %eax
	cmpl	$_d+32768, %eax
	jne	L31
	movl	$_visited, %eax
	.p2align 4,,10
L32:
	movb	$0, (%eax)
	addl	$1, %eax
	cmpl	$_visited+8192, %eax
	jne	L32
	rep ret
	.section	.text$_ZNSt6vectorIiSaIiEE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi,"x"
	.linkonce discard
	.align 2
	.p2align 4,,15
	.globl	__ZNSt6vectorIiSaIiEE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi
	.def	__ZNSt6vectorIiSaIiEE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi;	.scl	2;	.type	32;	.endef
__ZNSt6vectorIiSaIiEE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi:
	pushl	%ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$44, %esp
	movl	4(%ecx), %ebx
	cmpl	8(%ecx), %ebx
	movl	64(%esp), %esi
	je	L36
	testl	%ebx, %ebx
	je	L37
	movl	-4(%ebx), %eax
	movl	%eax, (%ebx)
L37:
	leal	4(%ebx), %eax
	movl	%eax, 4(%ecx)
	movl	68(%esp), %eax
	movl	(%eax), %edi
	leal	-4(%ebx), %eax
	subl	%esi, %eax
	movl	%eax, %edx
	sarl	$2, %edx
	testl	%edx, %edx
	jne	L64
L38:
	movl	%edi, (%esi)
L35:
	addl	$44, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	ret	$8
	.p2align 4,,10
L64:
	subl	%eax, %ebx
	movl	%eax, 8(%esp)
	movl	%esi, 4(%esp)
	movl	%ebx, (%esp)
	call	_memmove
	jmp	L38
	.p2align 4,,10
L36:
	movl	(%ecx), %eax
	subl	%eax, %ebx
	sarl	$2, %ebx
	testl	%ebx, %ebx
	je	L40
	leal	(%ebx,%ebx), %edx
	cmpl	%edx, %ebx
	jbe	L65
L41:
	movl	%esi, %ebp
	movl	$-4, %ebx
	subl	%eax, %ebp
L49:
	movl	%ebx, (%esp)
	movl	%ecx, 20(%esp)
	call	__Znwj
	movl	20(%esp), %ecx
	movl	%eax, %edi
	movl	%esi, %edx
	movl	(%ecx), %eax
	movl	%eax, 20(%esp)
	subl	%eax, %edx
L42:
	addl	%edi, %ebp
	je	L43
	movl	68(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, 0(%ebp)
L43:
	movl	%edx, %ebp
	sarl	$2, %ebp
	testl	%ebp, %ebp
	jne	L66
L45:
	movl	4(%ecx), %ebp
	leal	4(%edi,%edx), %edx
	subl	%esi, %ebp
	movl	%ebp, %eax
	sarl	$2, %eax
	testl	%eax, %eax
	jne	L67
L47:
	movl	(%ecx), %eax
	addl	%edx, %ebp
	testl	%eax, %eax
	je	L48
	movl	%eax, (%esp)
	movl	%ecx, 20(%esp)
	call	__ZdlPv
	movl	20(%esp), %ecx
L48:
	addl	%edi, %ebx
	movl	%edi, (%ecx)
	movl	%ebp, 4(%ecx)
	movl	%ebx, 8(%ecx)
	jmp	L35
	.p2align 4,,10
L40:
	movl	%esi, %ebp
	movl	$4, %ebx
	subl	%eax, %ebp
	jmp	L49
	.p2align 4,,10
L67:
	movl	%edx, (%esp)
	movl	%ebp, 8(%esp)
	movl	%esi, 4(%esp)
	movl	%ecx, 24(%esp)
	movl	%edx, 20(%esp)
	call	_memmove
	movl	24(%esp), %ecx
	movl	20(%esp), %edx
	jmp	L47
	.p2align 4,,10
L66:
	movl	20(%esp), %eax
	movl	%edx, 8(%esp)
	movl	%edi, (%esp)
	movl	%ecx, 28(%esp)
	movl	%edx, 24(%esp)
	movl	%eax, 4(%esp)
	call	_memmove
	movl	28(%esp), %ecx
	movl	24(%esp), %edx
	jmp	L45
L65:
	cmpl	$1073741823, %edx
	ja	L41
	movl	%esi, %ebp
	sall	$3, %ebx
	subl	%eax, %ebp
	testl	%edx, %edx
	jne	L49
	movl	%ebp, %edx
	movl	%eax, 20(%esp)
	xorl	%edi, %edi
	jmp	L42
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
LC0:
	.ascii "%d\0"
LC1:
	.ascii "%d %d %d\0"
LC2:
	.ascii "%d\12\0"
	.section	.text.startup,"x"
	.p2align 4,,15
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	pushl	%ecx
	subl	$40, %esp
	call	___main
	movl	$_n, 4(%esp)
	movl	$LC0, (%esp)
	call	__Z5scanfPKcz
	movl	$_d, %eax
	.p2align 4,,10
L69:
	movl	$0, (%eax)
	addl	$4, %eax
	cmpl	$_d+32768, %eax
	jne	L69
	movl	$_visited, %eax
	.p2align 4,,10
L70:
	movb	$0, (%eax)
	addl	$1, %eax
	cmpl	$_visited+8192, %eax
	jne	L70
	cmpl	$1, _n
	jle	L71
	movl	$1, %edi
	leal	-28(%ebp), %ebx
	leal	-32(%ebp), %esi
	jmp	L84
	.p2align 4,,10
L115:
	movl	-32(%ebp), %edx
	movl	%edx, (%eax)
	movl	-36(%ebp), %edx
L73:
	leal	(%ecx,%ecx,2), %ecx
	addl	$4, %eax
	movl	%eax, _v+4(,%ecx,4)
L74:
	leal	(%edx,%edx,2), %eax
	leal	_w(,%eax,4), %ecx
	movl	4(%ecx), %eax
	cmpl	8(%ecx), %eax
	je	L75
	testl	%eax, %eax
	je	L76
	movl	-28(%ebp), %ecx
	movl	%ecx, (%eax)
L76:
	leal	(%edx,%edx,2), %edx
	addl	$4, %eax
	movl	%eax, _w+4(,%edx,4)
L77:
	movl	-32(%ebp), %ecx
	leal	(%ecx,%ecx,2), %eax
	leal	_v(,%eax,4), %edx
	movl	4(%edx), %eax
	cmpl	8(%edx), %eax
	je	L78
	testl	%eax, %eax
	je	L98
	movl	-36(%ebp), %edx
	movl	%edx, (%eax)
	movl	-32(%ebp), %edx
L79:
	leal	(%ecx,%ecx,2), %ecx
	addl	$4, %eax
	movl	%eax, _v+4(,%ecx,4)
L80:
	leal	(%edx,%edx,2), %eax
	leal	_w(,%eax,4), %ecx
	movl	4(%ecx), %eax
	cmpl	8(%ecx), %eax
	je	L81
	testl	%eax, %eax
	je	L82
	movl	-28(%ebp), %ecx
	movl	%ecx, (%eax)
L82:
	leal	(%edx,%edx,2), %edx
	addl	$4, %eax
	movl	%eax, _w+4(,%edx,4)
L83:
	addl	$1, %edi
	cmpl	%edi, _n
	jle	L71
L84:
	leal	-36(%ebp), %eax
	movl	%ebx, 12(%esp)
	movl	%esi, 8(%esp)
	movl	$LC1, (%esp)
	movl	%eax, 4(%esp)
	call	__Z5scanfPKcz
	movl	-36(%ebp), %ecx
	leal	(%ecx,%ecx,2), %eax
	leal	_v(,%eax,4), %edx
	movl	4(%edx), %eax
	cmpl	8(%edx), %eax
	je	L72
	testl	%eax, %eax
	jne	L115
	movl	%ecx, %edx
	jmp	L73
	.p2align 4,,10
L71:
	movl	$_d, %eax
	.p2align 4,,10
L85:
	movl	$0, (%eax)
	addl	$4, %eax
	cmpl	$_d+32768, %eax
	jne	L85
	movl	$_visited, %eax
	.p2align 4,,10
L86:
	movb	$0, (%eax)
	addl	$1, %eax
	cmpl	$_visited+8192, %eax
	jne	L86
	movl	$1, (%esp)
	call	__Z3dfsi
	movl	_n, %eax
	movl	$_d+4, %edx
	leal	_d+4(,%eax,4), %ecx
	cmpl	$_d+4, %ecx
	je	L87
	movl	%edx, %eax
	jmp	L88
	.p2align 4,,10
L90:
	movl	(%eax), %esi
	cmpl	%esi, (%edx)
	cmovl	%eax, %edx
L88:
	addl	$4, %eax
	cmpl	%eax, %ecx
	jne	L90
L87:
	subl	$_d, %edx
	movl	$_d, %eax
	sarl	$2, %edx
	.p2align 4,,10
L91:
	movl	$0, (%eax)
	addl	$4, %eax
	cmpl	$_d+32768, %eax
	jne	L91
	movl	$_visited, %eax
	.p2align 4,,10
L92:
	movb	$0, (%eax)
	addl	$1, %eax
	cmpl	$_visited+8192, %eax
	jne	L92
	movl	%edx, (%esp)
	call	__Z3dfsi
	movl	_n, %eax
	movl	_d+4, %edx
	leal	_d+4(,%eax,4), %ebx
	cmpl	$_d+4, %ebx
	je	L95
	movl	$_d+4, %eax
	jmp	L94
	.p2align 4,,10
L96:
	movl	(%eax), %ecx
	cmpl	%ecx, %edx
	cmovl	%ecx, %edx
L94:
	addl	$4, %eax
	cmpl	%eax, %ebx
	jne	L96
L95:
	leal	1(%edx), %eax
	leal	(%edx,%edx,4), %ecx
	movl	$LC2, (%esp)
	imull	%eax, %edx
	movl	%edx, %eax
	shrl	$31, %eax
	addl	%eax, %edx
	sarl	%edx
	leal	(%edx,%ecx,2), %eax
	movl	%eax, 4(%esp)
	call	__Z6printfPKcz
	leal	-16(%ebp), %esp
	xorl	%eax, %eax
	popl	%ecx
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
L98:
	movl	%ecx, %edx
	jmp	L79
L72:
	movl	%edx, %ecx
	movl	%esi, 4(%esp)
	movl	%eax, (%esp)
	call	__ZNSt6vectorIiSaIiEE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi
	movl	-36(%ebp), %edx
	subl	$8, %esp
	jmp	L74
L78:
	leal	-36(%ebp), %ecx
	movl	%eax, (%esp)
	movl	%ecx, 4(%esp)
	movl	%edx, %ecx
	call	__ZNSt6vectorIiSaIiEE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi
	movl	-32(%ebp), %edx
	subl	$8, %esp
	jmp	L80
L81:
	movl	%ebx, 4(%esp)
	movl	%eax, (%esp)
	call	__ZNSt6vectorIiSaIiEE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi
	subl	$8, %esp
	jmp	L83
L75:
	movl	%ebx, 4(%esp)
	movl	%eax, (%esp)
	call	__ZNSt6vectorIiSaIiEE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi
	subl	$8, %esp
	jmp	L77
	.p2align 4,,15
	.def	__GLOBAL__sub_I_n;	.scl	3;	.type	32;	.endef
__GLOBAL__sub_I_n:
	movl	$_v, %eax
	.p2align 4,,10
L117:
	movl	$0, (%eax)
	movl	$0, 4(%eax)
	addl	$12, %eax
	movl	$0, -4(%eax)
	cmpl	$_v+98304, %eax
	jne	L117
	subl	$28, %esp
	movl	$___tcf_0, (%esp)
	call	_atexit
	movl	$_w, %eax
	.p2align 4,,10
L118:
	movl	$0, (%eax)
	movl	$0, 4(%eax)
	addl	$12, %eax
	movl	$0, -4(%eax)
	cmpl	$_w+98304, %eax
	jne	L118
	movl	$___tcf_1, (%esp)
	call	_atexit
	addl	$28, %esp
	ret
	.section	.ctors,"w"
	.align 4
	.long	__GLOBAL__sub_I_n
	.globl	_visited
	.bss
	.align 64
_visited:
	.space 8192
	.globl	_d
	.align 64
_d:
	.space 32768
	.globl	_w
	.align 64
_w:
	.space 98304
	.globl	_v
	.align 64
_v:
	.space 98304
	.globl	_n
	.align 4
_n:
	.space 4
	.ident	"GCC: (tdm64-1) 4.9.2"
	.def	__ZdlPv;	.scl	2;	.type	32;	.endef
	.def	___mingw_vscanf;	.scl	2;	.type	32;	.endef
	.def	___mingw_vprintf;	.scl	2;	.type	32;	.endef
	.def	_memmove;	.scl	2;	.type	32;	.endef
	.def	__Znwj;	.scl	2;	.type	32;	.endef
	.def	_atexit;	.scl	2;	.type	32;	.endef
