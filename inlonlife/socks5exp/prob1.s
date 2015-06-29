	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 10
	.globl	_counter_x
	.align	4, 0x90
_counter_x:                             ## @counter_x
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	movl	_counter_x._x(%rip), %eax
	addl	$1, %eax
	movl	%eax, _counter_x._x(%rip)
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_counter_y
	.align	4, 0x90
_counter_y:                             ## @counter_y
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp3:
	.cfi_def_cfa_offset 16
Ltmp4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp5:
	.cfi_def_cfa_register %rbp
	movl	_counter_y._y(%rip), %eax
	addl	$1, %eax
	movl	%eax, _counter_y._y(%rip)
	popq	%rbp
	retq
	.cfi_endproc

	.section	__TEXT,__literal8,8byte_literals
	.align	3
LCPI2_0:
	.quad	4746794007244308480     ## double 2147483647
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_random_in
	.align	4, 0x90
_random_in:                             ## @random_in
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp6:
	.cfi_def_cfa_offset 16
Ltmp7:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp8:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movsd	%xmm0, -8(%rbp)
	movsd	%xmm1, -16(%rbp)
	callq	_rand
	movsd	LCPI2_0(%rip), %xmm0
	cvtsi2sdl	%eax, %xmm1
	divsd	%xmm0, %xmm1
	movsd	-16(%rbp), %xmm0
	subsd	-8(%rbp), %xmm0
	mulsd	%xmm0, %xmm1
	addsd	-8(%rbp), %xmm1
	movaps	%xmm1, %xmm0
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_sum
	.align	4, 0x90
_sum:                                   ## @sum
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp9:
	.cfi_def_cfa_offset 16
Ltmp10:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp11:
	.cfi_def_cfa_register %rbp
	subq	$448, %rsp              ## imm = 0x1C0
	testb	%al, %al
	movaps	%xmm7, -240(%rbp)       ## 16-byte Spill
	movaps	%xmm6, -256(%rbp)       ## 16-byte Spill
	movaps	%xmm5, -272(%rbp)       ## 16-byte Spill
	movaps	%xmm4, -288(%rbp)       ## 16-byte Spill
	movaps	%xmm3, -304(%rbp)       ## 16-byte Spill
	movaps	%xmm2, -320(%rbp)       ## 16-byte Spill
	movaps	%xmm1, -336(%rbp)       ## 16-byte Spill
	movaps	%xmm0, -352(%rbp)       ## 16-byte Spill
	movl	%edi, -356(%rbp)        ## 4-byte Spill
	movq	%r9, -368(%rbp)         ## 8-byte Spill
	movq	%r8, -376(%rbp)         ## 8-byte Spill
	movq	%rcx, -384(%rbp)        ## 8-byte Spill
	movq	%rdx, -392(%rbp)        ## 8-byte Spill
	movq	%rsi, -400(%rbp)        ## 8-byte Spill
	je	LBB3_11
## BB#10:
	movaps	-352(%rbp), %xmm0       ## 16-byte Reload
	movaps	%xmm0, -176(%rbp)
	movaps	-336(%rbp), %xmm1       ## 16-byte Reload
	movaps	%xmm1, -160(%rbp)
	movaps	-320(%rbp), %xmm2       ## 16-byte Reload
	movaps	%xmm2, -144(%rbp)
	movaps	-304(%rbp), %xmm3       ## 16-byte Reload
	movaps	%xmm3, -128(%rbp)
	movaps	-288(%rbp), %xmm4       ## 16-byte Reload
	movaps	%xmm4, -112(%rbp)
	movaps	-272(%rbp), %xmm5       ## 16-byte Reload
	movaps	%xmm5, -96(%rbp)
	movaps	-256(%rbp), %xmm6       ## 16-byte Reload
	movaps	%xmm6, -80(%rbp)
	movaps	-240(%rbp), %xmm7       ## 16-byte Reload
	movaps	%xmm7, -64(%rbp)
LBB3_11:
	movq	-368(%rbp), %rax        ## 8-byte Reload
	movq	%rax, -184(%rbp)
	movq	-376(%rbp), %rcx        ## 8-byte Reload
	movq	%rcx, -192(%rbp)
	movq	-384(%rbp), %rdx        ## 8-byte Reload
	movq	%rdx, -200(%rbp)
	movq	-392(%rbp), %rsi        ## 8-byte Reload
	movq	%rsi, -208(%rbp)
	movq	-400(%rbp), %rdi        ## 8-byte Reload
	movq	%rdi, -216(%rbp)
	movl	-356(%rbp), %r8d        ## 4-byte Reload
	leaq	-32(%rbp), %r9
	movq	___stack_chk_guard@GOTPCREL(%rip), %r10
	movq	(%r10), %r10
	movq	%r10, -8(%rbp)
	movl	%r8d, -36(%rbp)
	movl	$0, -40(%rbp)
	leaq	-224(%rbp), %r10
	movq	%r10, 16(%r9)
	leaq	16(%rbp), %r10
	movq	%r10, 8(%r9)
	movl	$48, 4(%r9)
	movl	$8, (%r9)
	movl	$0, -44(%rbp)
LBB3_1:                                 ## =>This Inner Loop Header: Depth=1
	movl	-44(%rbp), %eax
	cmpl	-36(%rbp), %eax
	jge	LBB3_7
## BB#2:                                ##   in Loop: Header=BB3_1 Depth=1
	leaq	-32(%rbp), %rax
	movl	-32(%rbp), %ecx
	cmpl	$40, %ecx
	movq	%rax, -408(%rbp)        ## 8-byte Spill
	movl	%ecx, -412(%rbp)        ## 4-byte Spill
	ja	LBB3_4
## BB#3:                                ##   in Loop: Header=BB3_1 Depth=1
	movq	-408(%rbp), %rax        ## 8-byte Reload
	movq	16(%rax), %rcx
	movl	-412(%rbp), %edx        ## 4-byte Reload
	movslq	%edx, %rsi
	addq	%rsi, %rcx
	addl	$8, %edx
	movl	%edx, (%rax)
	movq	%rcx, -424(%rbp)        ## 8-byte Spill
	jmp	LBB3_5
LBB3_4:                                 ##   in Loop: Header=BB3_1 Depth=1
	movq	-408(%rbp), %rax        ## 8-byte Reload
	movq	8(%rax), %rcx
	movq	%rcx, %rdx
	addq	$8, %rcx
	movq	%rcx, 8(%rax)
	movq	%rdx, -424(%rbp)        ## 8-byte Spill
LBB3_5:                                 ##   in Loop: Header=BB3_1 Depth=1
	movq	-424(%rbp), %rax        ## 8-byte Reload
	movl	(%rax), %ecx
	movl	-40(%rbp), %edx
	addl	%ecx, %edx
	movl	%edx, -40(%rbp)
## BB#6:                                ##   in Loop: Header=BB3_1 Depth=1
	movl	-44(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -44(%rbp)
	jmp	LBB3_1
LBB3_7:
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	leaq	-32(%rbp), %rcx
	movl	-40(%rbp), %edx
	movq	(%rax), %rax
	cmpq	-8(%rbp), %rax
	movl	%edx, -428(%rbp)        ## 4-byte Spill
	movq	%rcx, -440(%rbp)        ## 8-byte Spill
	jne	LBB3_9
## BB#8:                                ## %SP_return
	movl	-428(%rbp), %eax        ## 4-byte Reload
	addq	$448, %rsp              ## imm = 0x1C0
	popq	%rbp
	retq
LBB3_9:                                 ## %CallStackCheckFailBlk
	callq	___stack_chk_fail
	.cfi_endproc

	.globl	_test2
	.align	4, 0x90
_test2:                                 ## @test2
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp12:
	.cfi_def_cfa_offset 16
Ltmp13:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp14:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	leaq	L_.str(%rip), %rdi
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	l_test2.arr(%rip), %rax
	movq	%rax, -20(%rbp)
	movl	l_test2.arr+8(%rip), %ecx
	movl	%ecx, -12(%rbp)
	movl	$0, -24(%rbp)
	movl	-24(%rbp), %ecx
	addl	$1, %ecx
	movl	%ecx, -24(%rbp)
	movslq	%ecx, %rax
	movl	-20(%rbp,%rax,4), %ecx
	addl	$1, %ecx
	movl	%ecx, -20(%rbp,%rax,4)
	movl	-24(%rbp), %edx
	addl	$1, %edx
	movl	%edx, -24(%rbp)
	addl	%edx, %ecx
	movl	-24(%rbp), %edx
	addl	$4294967295, %edx       ## imm = 0xFFFFFFFF
	movl	%edx, -24(%rbp)
	movslq	%edx, %rax
	addl	-20(%rbp,%rax,4), %ecx
	movl	%ecx, -24(%rbp)
	movl	-24(%rbp), %esi
	movb	$0, %al
	callq	_printf
	movq	___stack_chk_guard@GOTPCREL(%rip), %rdi
	movq	(%rdi), %rdi
	cmpq	-8(%rbp), %rdi
	movl	%eax, -28(%rbp)         ## 4-byte Spill
	jne	LBB4_2
## BB#1:                                ## %SP_return
	addq	$32, %rsp
	popq	%rbp
	retq
LBB4_2:                                 ## %CallStackCheckFailBlk
	callq	___stack_chk_fail
	.cfi_endproc

	.section	__TEXT,__literal8,8byte_literals
	.align	3
LCPI5_0:
	.quad	4611686018427387904     ## double 2
LCPI5_1:
	.quad	4621819117588971520     ## double 10
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp15:
	.cfi_def_cfa_offset 16
Ltmp16:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp17:
	.cfi_def_cfa_register %rbp
	subq	$96, %rsp
	xorl	%eax, %eax
	movl	%eax, %ecx
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rcx, %rdi
	callq	_time
	movl	%eax, %edx
	movl	%edx, %edi
	callq	_srand
	callq	_counter_x
	leaq	L_.str(%rip), %rdi
	movl	%eax, %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -52(%rbp)         ## 4-byte Spill
	callq	_counter_x
	leaq	L_.str(%rip), %rdi
	movl	%eax, %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -56(%rbp)         ## 4-byte Spill
	callq	_counter_x
	leaq	L_.str(%rip), %rdi
	movl	%eax, %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -60(%rbp)         ## 4-byte Spill
	callq	_counter_y
	leaq	L_.str(%rip), %rdi
	movl	%eax, %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -64(%rbp)         ## 4-byte Spill
	callq	_counter_y
	leaq	L_.str(%rip), %rdi
	movl	%eax, %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -68(%rbp)         ## 4-byte Spill
	callq	_counter_x
	leaq	L_.str(%rip), %rdi
	movl	%eax, %esi
	movb	$0, %al
	callq	_printf
	leaq	L_.str1(%rip), %rdi
	movl	$4, -20(%rbp)
	movl	$5, -24(%rbp)
	movl	-20(%rbp), %edx
	movl	%edx, -28(%rbp)
	movl	-24(%rbp), %edx
	movl	%edx, -20(%rbp)
	movl	-28(%rbp), %edx
	movl	%edx, -24(%rbp)
	movl	-20(%rbp), %esi
	movl	-24(%rbp), %edx
	movl	%eax, -72(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_printf
	leaq	L_.str2(%rip), %rsi
	leaq	L_.str3(%rip), %rdx
	leaq	-20(%rbp), %rcx
	movq	___stderrp@GOTPCREL(%rip), %rdi
	movq	(%rdi), %rdi
	movl	-20(%rbp), %r8d
	movl	%eax, -76(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_fprintf
	movl	$800, %r8d              ## imm = 0x320
	movl	%r8d, %edi
	movl	%eax, -80(%rbp)         ## 4-byte Spill
	callq	_malloc
	leaq	L_.str4(%rip), %rdi
	movl	$8, %r8d
	movl	%r8d, %esi
	movq	%rax, -40(%rbp)
	movb	$0, %al
	callq	_printf
	cmpq	$0, -40(%rbp)
	movl	%eax, -84(%rbp)         ## 4-byte Spill
	jne	LBB5_2
## BB#1:
	movl	$1, %edi
	callq	_exit
LBB5_2:
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	callq	_free
	movl	$100, %ecx
	movl	%ecx, %edi
	movl	$8, %ecx
	movl	%ecx, %esi
	callq	_calloc
	movq	%rax, -48(%rbp)
	cmpq	$0, -48(%rbp)
	jne	LBB5_4
## BB#3:
	movl	$1, %edi
	callq	_exit
LBB5_4:
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	callq	_free
	movsd	LCPI5_0(%rip), %xmm0
	movsd	LCPI5_1(%rip), %xmm1
	callq	_random_in
	leaq	L_.str5(%rip), %rdi
	movb	$1, %al
	callq	_printf
	movl	$4, %ecx
	movl	$1, %esi
	movl	$2, %edx
	movl	$3, %r8d
	movl	%ecx, %edi
	movl	%ecx, -88(%rbp)         ## 4-byte Spill
	movl	%r8d, %ecx
	movl	-88(%rbp), %r8d         ## 4-byte Reload
	movl	%eax, -92(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_sum
	leaq	L_.str(%rip), %rdi
	movl	%eax, %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -96(%rbp)         ## 4-byte Spill
	callq	_test2
	xorl	%eax, %eax
	addq	$96, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_execv
	.align	4, 0x90
_execv:                                 ## @execv
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp18:
	.cfi_def_cfa_offset 16
Ltmp19:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp20:
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	leaq	L_.str6(%rip), %rcx
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rcx, -8(%rbp)
	movq	-16(%rbp), %rcx
	movq	(%rcx), %rcx
	movb	$97, (%rcx)
	popq	%rbp
	retq
	.cfi_endproc

.zerofill __DATA,__bss,_counter_x._x,4,2 ## @counter_x._x
.zerofill __DATA,__bss,_counter_y._y,4,2 ## @counter_y._y
	.section	__TEXT,__const
	.align	2                       ## @test2.arr
l_test2.arr:
	.long	10                      ## 0xa
	.long	20                      ## 0x14
	.long	30                      ## 0x1e

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%d\n"

L_.str1:                                ## @.str1
	.asciz	"%d %d\n"

L_.str2:                                ## @.str2
	.asciz	"%s[%p] = %d\n"

L_.str3:                                ## @.str3
	.asciz	"u"

L_.str4:                                ## @.str4
	.asciz	"%lu\n"

L_.str5:                                ## @.str5
	.asciz	"%lf\n"

L_.str6:                                ## @.str6
	.asciz	"/bin/sh"


.subsections_via_symbols
