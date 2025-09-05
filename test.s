	.file	"test.cpp"
	.intel_syntax noprefix
	.text
	.p2align 4
	.def	__tcf_1;	.scl	3;	.type	32;	.endef
	.seh_proc	__tcf_1
__tcf_1:
.LFB3606:
	.seh_endprologue
	mov	rcx, QWORD PTR double_test[rip]
	test	rcx, rcx
	je	.L1
	mov	rdx, QWORD PTR double_test[rip+16]
	sub	rdx, rcx
	jmp	_ZdlPvy
	.p2align 4,,10
	.p2align 3
.L1:
	ret
	.seh_endproc
	.p2align 4
	.def	__tcf_0;	.scl	3;	.type	32;	.endef
	.seh_proc	__tcf_0
__tcf_0:
.LFB3605:
	.seh_endprologue
	mov	rcx, QWORD PTR int_test[rip]
	test	rcx, rcx
	je	.L4
	mov	rdx, QWORD PTR int_test[rip+16]
	sub	rdx, rcx
	jmp	_ZdlPvy
	.p2align 4,,10
	.p2align 3
.L4:
	ret
	.seh_endproc
	.p2align 4
	.def	__tcf_2;	.scl	3;	.type	32;	.endef
	.seh_proc	__tcf_2
__tcf_2:
.LFB3607:
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 40
	.seh_stackalloc	40
	.seh_endprologue
	mov	rsi, QWORD PTR struct_test[rip+8]
	mov	rbx, QWORD PTR struct_test[rip]
	cmp	rsi, rbx
	je	.L7
	.p2align 4
	.p2align 3
.L9:
	mov	rcx, QWORD PTR 24[rbx]
	lea	rax, 40[rbx]
	cmp	rcx, rax
	je	.L8
	mov	rax, QWORD PTR 40[rbx]
	lea	rdx, 1[rax]
	call	_ZdlPvy
.L8:
	add	rbx, 56
	cmp	rsi, rbx
	jne	.L9
	mov	rbx, QWORD PTR struct_test[rip]
.L7:
	test	rbx, rbx
	je	.L6
	mov	rdx, QWORD PTR struct_test[rip+16]
	mov	rcx, rbx
	sub	rdx, rbx
	add	rsp, 40
	pop	rbx
	pop	rsi
	jmp	_ZdlPvy
	.p2align 4,,10
	.p2align 3
.L6:
	add	rsp, 40
	pop	rbx
	pop	rsi
	ret
	.seh_endproc
	.section	.text$_Z15struct_copyTestI4ABCDEvRKSt6vectorIT_SaIS2_EE,"x"
	.linkonce discard
	.p2align 4
	.globl	_Z15struct_copyTestI4ABCDEvRKSt6vectorIT_SaIS2_EE
	.def	_Z15struct_copyTestI4ABCDEvRKSt6vectorIT_SaIS2_EE;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z15struct_copyTestI4ABCDEvRKSt6vectorIT_SaIS2_EE
_Z15struct_copyTestI4ABCDEvRKSt6vectorIT_SaIS2_EE:
.LFB3146:
	push	r14
	.seh_pushreg	r14
	push	r13
	.seh_pushreg	r13
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 112
	.seh_stackalloc	112
	.seh_endprologue
	mov	rbp, QWORD PTR 8[rcx]
	mov	rbx, QWORD PTR [rcx]
	lea	rdi, 88[rsp]
	lea	r13, 40[rsp]
	lea	r12, 72[rsp]
	cmp	rbp, rbx
	jne	.L20
	jmp	.L12
	.p2align 4,,10
	.p2align 3
.L28:
	movzx	eax, BYTE PTR [r14]
	mov	BYTE PTR 88[rsp], al
.L18:
	mov	rax, rdi
.L17:
	mov	QWORD PTR 80[rsp], rsi
	mov	BYTE PTR [rax+rsi], 0
	mov	rcx, QWORD PTR 72[rsp]
	cmp	rcx, rdi
	je	.L19
	mov	rax, QWORD PTR 88[rsp]
	lea	rdx, 1[rax]
	call	_ZdlPvy
.L19:
	add	rbx, 56
	cmp	rbp, rbx
	je	.L12
.L20:
	mov	eax, DWORD PTR [rbx]
	movsd	xmm0, QWORD PTR 8[rbx]
	mov	DWORD PTR 48[rsp], eax
	movzx	eax, BYTE PTR 16[rbx]
	movsd	QWORD PTR 56[rsp], xmm0
	mov	BYTE PTR 64[rsp], al
	movzx	eax, BYTE PTR 17[rbx]
	mov	rsi, QWORD PTR 32[rbx]
	mov	QWORD PTR 72[rsp], rdi
	mov	BYTE PTR 65[rsp], al
	mov	r14, QWORD PTR 24[rbx]
	mov	QWORD PTR 40[rsp], rsi
	cmp	rsi, 15
	ja	.L27
	cmp	rsi, 1
	je	.L28
	test	rsi, rsi
	je	.L18
	mov	rcx, rdi
	jmp	.L15
	.p2align 4,,10
	.p2align 3
.L27:
	mov	rcx, r12
	xor	r8d, r8d
	mov	rdx, r13
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERyy
	mov	QWORD PTR 72[rsp], rax
	mov	rcx, rax
	mov	rax, QWORD PTR 40[rsp]
	mov	QWORD PTR 88[rsp], rax
.L15:
	mov	r8, rsi
	mov	rdx, r14
	call	memcpy
	mov	rsi, QWORD PTR 40[rsp]
	mov	rax, QWORD PTR 72[rsp]
	jmp	.L17
	.p2align 4,,10
	.p2align 3
.L12:
	add	rsp, 112
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	pop	r13
	pop	r14
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC0:
	.ascii "\354\213\244\355\226\211 \354\213\234\352\260\204 \355\205\214\354\212\244\355\212\270\12\12\0"
	.align 8
.LC2:
	.ascii "\354\235\274\353\260\230 \355\203\200\354\236\205 \353\263\265\354\202\254 \353\260\251\354\213\235 \352\262\275\352\263\274 \354\213\234\352\260\204 : \0"
.LC3:
	.ascii " seconds\12\0"
	.align 8
.LC4:
	.ascii "\354\235\274\353\260\230 \355\203\200\354\236\205 const \354\260\270\354\241\260 \353\260\251\354\213\235 \352\262\275\352\263\274 \354\213\234\352\260\204 : \0"
	.align 8
.LC5:
	.ascii "----------------------------------------------------------------------------------------------\12\0"
	.align 8
.LC6:
	.ascii "\352\265\254\354\241\260\354\262\264 \355\203\200\354\236\205 \353\263\265\354\202\254 \353\260\251\354\213\235 \352\262\275\352\263\274 \354\213\234\352\260\204 : \0"
	.align 8
.LC7:
	.ascii "\352\265\254\354\241\260\354\262\264 \355\203\200\354\236\205 const \354\260\270\354\241\260 \353\260\251\354\213\235 \352\262\275\352\263\274 \354\213\234\352\260\204 : \0"
	.section	.text.startup,"x"
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB2806:
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 56
	.seh_stackalloc	56
	movups	XMMWORD PTR 32[rsp], xmm6
	.seh_savexmm	xmm6, 32
	.seh_endprologue
	lea	rsi, .LC3[rip]
	call	__main
	mov	rbx, QWORD PTR .refptr._ZSt4cout[rip]
	lea	rdx, .LC0[rip]
	mov	rcx, rbx
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	call	_ZNSt6chrono3_V212system_clock3nowEv
	mov	QWORD PTR t_start[rip], rax
	call	_ZNSt6chrono3_V212system_clock3nowEv
	pxor	xmm0, xmm0
	movsd	xmm6, QWORD PTR .LC1[rip]
	mov	rcx, rbx
	mov	QWORD PTR t_end[rip], rax
	sub	rax, QWORD PTR t_start[rip]
	lea	rdx, .LC2[rip]
	cvtsi2sd	xmm0, rax
	divsd	xmm0, xmm6
	movsd	QWORD PTR elapsed_seconds[rip], xmm0
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movsd	xmm1, QWORD PTR elapsed_seconds[rip]
	mov	rcx, rax
	call	_ZNSo9_M_insertIdEERSoT_
	mov	rdx, rsi
	mov	rcx, rax
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	call	_ZNSt6chrono3_V212system_clock3nowEv
	mov	QWORD PTR t_start[rip], rax
	call	_ZNSt6chrono3_V212system_clock3nowEv
	pxor	xmm0, xmm0
	lea	rdx, .LC4[rip]
	mov	rcx, rbx
	mov	QWORD PTR t_end[rip], rax
	sub	rax, QWORD PTR t_start[rip]
	cvtsi2sd	xmm0, rax
	divsd	xmm0, xmm6
	movsd	QWORD PTR elapsed_seconds[rip], xmm0
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movsd	xmm1, QWORD PTR elapsed_seconds[rip]
	mov	rcx, rax
	call	_ZNSo9_M_insertIdEERSoT_
	mov	rdx, rsi
	mov	rcx, rax
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	lea	rdx, .LC5[rip]
	mov	rcx, rbx
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	call	_ZNSt6chrono3_V212system_clock3nowEv
	lea	rcx, struct_test[rip]
	mov	QWORD PTR t_start[rip], rax
	call	_Z15struct_copyTestI4ABCDEvRKSt6vectorIT_SaIS2_EE
	call	_ZNSt6chrono3_V212system_clock3nowEv
	pxor	xmm0, xmm0
	lea	rdx, .LC6[rip]
	mov	rcx, rbx
	mov	QWORD PTR t_end[rip], rax
	sub	rax, QWORD PTR t_start[rip]
	cvtsi2sd	xmm0, rax
	divsd	xmm0, xmm6
	movsd	QWORD PTR elapsed_seconds[rip], xmm0
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movsd	xmm1, QWORD PTR elapsed_seconds[rip]
	mov	rcx, rax
	call	_ZNSo9_M_insertIdEERSoT_
	mov	rdx, rsi
	mov	rcx, rax
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	call	_ZNSt6chrono3_V212system_clock3nowEv
	mov	QWORD PTR t_start[rip], rax
	call	_ZNSt6chrono3_V212system_clock3nowEv
	pxor	xmm0, xmm0
	lea	rdx, .LC7[rip]
	mov	rcx, rbx
	mov	QWORD PTR t_end[rip], rax
	sub	rax, QWORD PTR t_start[rip]
	cvtsi2sd	xmm0, rax
	divsd	xmm0, xmm6
	movsd	QWORD PTR elapsed_seconds[rip], xmm0
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movsd	xmm1, QWORD PTR elapsed_seconds[rip]
	mov	rcx, rax
	call	_ZNSo9_M_insertIdEERSoT_
	mov	rdx, rsi
	mov	rcx, rax
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	nop
	movups	xmm6, XMMWORD PTR 32[rsp]
	xor	eax, eax
	add	rsp, 56
	pop	rbx
	pop	rsi
	ret
	.seh_endproc
	.section	.text$_ZNSt27__uninitialized_default_n_1ILb0EE18__uninit_default_nIP4ABCDyEET_S4_T0_,"x"
	.linkonce discard
	.p2align 4
	.globl	_ZNSt27__uninitialized_default_n_1ILb0EE18__uninit_default_nIP4ABCDyEET_S4_T0_
	.def	_ZNSt27__uninitialized_default_n_1ILb0EE18__uninit_default_nIP4ABCDyEET_S4_T0_;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZNSt27__uninitialized_default_n_1ILb0EE18__uninit_default_nIP4ABCDyEET_S4_T0_
_ZNSt27__uninitialized_default_n_1ILb0EE18__uninit_default_nIP4ABCDyEET_S4_T0_:
.LFB3569:
	push	r12
	.seh_pushreg	r12
	push	rbp
	.seh_pushreg	rbp
	push	rdi
	.seh_pushreg	rdi
	push	rsi
	.seh_pushreg	rsi
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 80
	.seh_stackalloc	80
	movups	XMMWORD PTR 48[rsp], xmm6
	.seh_savexmm	xmm6, 48
	movups	XMMWORD PTR 64[rsp], xmm7
	.seh_savexmm	xmm7, 64
	.seh_endprologue
	mov	r12, rcx
	mov	rsi, rdx
	mov	rbx, rcx
	test	rdx, rdx
	je	.L30
	movsd	xmm7, QWORD PTR .LC8[rip]
	lea	rbp, 40[rsp]
	pxor	xmm6, xmm6
	movabs	rdi, -6589914658031028854
	.p2align 4
	.p2align 3
.L32:
	lea	rax, 40[rbx]
	movups	XMMWORD PTR [rbx], xmm6
	lea	rcx, 24[rbx]
	xor	r8d, r8d
	movups	XMMWORD PTR 16[rbx], xmm6
	mov	rdx, rbp
	mov	QWORD PTR 48[rbx], 0
	mov	DWORD PTR [rbx], 5
	mov	BYTE PTR 16[rbx], 66
	mov	QWORD PTR 24[rbx], rax
	movups	XMMWORD PTR 32[rbx], xmm6
	movsd	QWORD PTR 8[rbx], xmm7
	mov	QWORD PTR 40[rsp], 71
.LEHB0:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERyy
.LEHE0:
	mov	rdx, QWORD PTR 40[rsp]
	mov	QWORD PTR 24[rbx], rax
	movabs	r9, -7643288590163077652
	movabs	r10, 2347671647265549464
	movabs	r8, -7499460267227960085
	add	rbx, 56
	movabs	r11, -8796338743462350671
	mov	QWORD PTR -16[rbx], rdx
	mov	QWORD PTR [rax], r9
	movabs	r9, -1474775034288477004
	mov	QWORD PTR 8[rax], r10
	movabs	r10, -1534427935548726133
	mov	QWORD PTR 16[rax], r8
	movabs	r8, 2357787158200904844
	mov	QWORD PTR 24[rax], r9
	movabs	r9, -8436191311671290387
	mov	QWORD PTR 56[rax], r9
	mov	QWORD PTR 32[rax], r10
	mov	QWORD PTR 40[rax], r11
	mov	QWORD PTR 48[rax], r8
	mov	QWORD PTR 63[rax], rdi
	mov	rax, QWORD PTR -32[rbx]
	mov	QWORD PTR -24[rbx], rdx
	mov	BYTE PTR [rax+rdx], 0
	sub	rsi, 1
	jne	.L32
.L30:
	movups	xmm6, XMMWORD PTR 48[rsp]
	movups	xmm7, XMMWORD PTR 64[rsp]
	mov	rax, rbx
	add	rsp, 80
	pop	rbx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r12
	ret
.L38:
	mov	rcx, rax
	call	__cxa_begin_catch
.L34:
	cmp	r12, rbx
	je	.L41
	lea	rcx, 24[r12]
	add	r12, 56
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	jmp	.L34
.L41:
.LEHB1:
	call	__cxa_rethrow
.LEHE1:
.L39:
	mov	rbx, rax
	call	__cxa_end_catch
	mov	rcx, rbx
.LEHB2:
	call	_Unwind_Resume
	nop
.LEHE2:
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
	.align 4
.LLSDA3569:
	.byte	0xff
	.byte	0x9b
	.uleb128 .LLSDATT3569-.LLSDATTD3569
.LLSDATTD3569:
	.byte	0x1
	.uleb128 .LLSDACSE3569-.LLSDACSB3569
.LLSDACSB3569:
	.uleb128 .LEHB0-.LFB3569
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L38-.LFB3569
	.uleb128 0x1
	.uleb128 .LEHB1-.LFB3569
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L39-.LFB3569
	.uleb128 0
	.uleb128 .LEHB2-.LFB3569
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSE3569:
	.byte	0x1
	.byte	0
	.align 4
	.long	0

.LLSDATT3569:
	.section	.text$_ZNSt27__uninitialized_default_n_1ILb0EE18__uninit_default_nIP4ABCDyEET_S4_T0_,"x"
	.linkonce discard
	.seh_endproc
	.section	.text.startup,"x"
	.p2align 4
	.def	_GLOBAL__sub_I_t_start;	.scl	3;	.type	32;	.endef
	.seh_proc	_GLOBAL__sub_I_t_start
_GLOBAL__sub_I_t_start:
.LFB3624:
	push	rbx
	.seh_pushreg	rbx
	sub	rsp, 32
	.seh_stackalloc	32
	.seh_endprologue
	pxor	xmm0, xmm0
	mov	ecx, 40000000
	mov	QWORD PTR int_test[rip+16], 0
	movups	XMMWORD PTR int_test[rip], xmm0
.LEHB3:
	call	_Znwy
	mov	ecx, 1
	lea	rdx, 40000000[rax]
	mov	QWORD PTR int_test[rip], rax
	movd	xmm0, ecx
	mov	QWORD PTR int_test[rip+16], rdx
	pshufd	xmm0, xmm0, 0
	.p2align 4
	.p2align 4
	.p2align 3
.L43:
	movups	XMMWORD PTR [rax], xmm0
	add	rax, 32
	movups	XMMWORD PTR -16[rax], xmm0
	cmp	rdx, rax
	jne	.L43
	lea	rcx, __tcf_0[rip]
	mov	QWORD PTR int_test[rip+8], rdx
	call	atexit
	pxor	xmm0, xmm0
	mov	ecx, 80000000
	mov	QWORD PTR double_test[rip+16], 0
	movups	XMMWORD PTR double_test[rip], xmm0
	call	_Znwy
	movddup	xmm0, QWORD PTR .LC12[rip]
	lea	rdx, 80000000[rax]
	mov	QWORD PTR double_test[rip], rax
	mov	QWORD PTR double_test[rip+16], rdx
	.p2align 4
	.p2align 4
	.p2align 3
.L44:
	movups	XMMWORD PTR [rax], xmm0
	add	rax, 32
	movups	XMMWORD PTR -16[rax], xmm0
	cmp	rdx, rax
	jne	.L44
	lea	rcx, __tcf_1[rip]
	mov	QWORD PTR double_test[rip+8], rdx
	call	atexit
	pxor	xmm0, xmm0
	mov	ecx, 560000000
	mov	QWORD PTR struct_test[rip+16], 0
	movups	XMMWORD PTR struct_test[rip], xmm0
	call	_Znwy
.LEHE3:
	mov	edx, 10000000
	movq	xmm1, rax
	mov	rcx, rax
	lea	rax, 560000000[rax]
	movddup	xmm0, xmm1
	mov	QWORD PTR struct_test[rip+16], rax
	movups	XMMWORD PTR struct_test[rip], xmm0
.LEHB4:
	call	_ZNSt27__uninitialized_default_n_1ILb0EE18__uninit_default_nIP4ABCDyEET_S4_T0_
.LEHE4:
	lea	rcx, __tcf_2[rip]
	mov	QWORD PTR struct_test[rip+8], rax
	add	rsp, 32
	pop	rbx
	jmp	atexit
.L47:
	mov	rcx, QWORD PTR struct_test[rip]
	mov	rdx, QWORD PTR struct_test[rip+16]
	mov	rbx, rax
	sub	rdx, rcx
	test	rcx, rcx
	je	.L46
	call	_ZdlPvy
.L46:
	mov	rcx, rbx
.LEHB5:
	call	_Unwind_Resume
	nop
.LEHE5:
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA3624:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE3624-.LLSDACSB3624
.LLSDACSB3624:
	.uleb128 .LEHB3-.LFB3624
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB4-.LFB3624
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L47-.LFB3624
	.uleb128 0
	.uleb128 .LEHB5-.LFB3624
	.uleb128 .LEHE5-.LEHB5
	.uleb128 0
	.uleb128 0
.LLSDACSE3624:
	.section	.text.startup,"x"
	.seh_endproc
	.section	.ctors,"w"
	.align 8
	.quad	_GLOBAL__sub_I_t_start
	.globl	struct_test
	.bss
	.align 16
struct_test:
	.space 24
	.globl	double_test
	.align 16
double_test:
	.space 24
	.globl	int_test
	.align 16
int_test:
	.space 24
	.globl	elapsed_seconds
	.align 8
elapsed_seconds:
	.space 8
	.globl	t_end
	.align 8
t_end:
	.space 8
	.globl	t_start
	.align 8
t_start:
	.space 8
	.section .rdata,"dr"
	.align 8
.LC1:
	.long	0
	.long	1104006501
	.align 8
.LC8:
	.long	-2034096511
	.long	1079958439
	.align 8
.LC12:
	.long	309237645
	.long	1072939139
	.def	__gxx_personality_seh0;	.scl	2;	.type	32;	.endef
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev2, Built by MSYS2 project) 14.2.0"
	.def	_ZdlPvy;	.scl	2;	.type	32;	.endef
	.def	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERyy;	.scl	2;	.type	32;	.endef
	.def	memcpy;	.scl	2;	.type	32;	.endef
	.def	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc;	.scl	2;	.type	32;	.endef
	.def	_ZNSt6chrono3_V212system_clock3nowEv;	.scl	2;	.type	32;	.endef
	.def	_ZNSo9_M_insertIdEERSoT_;	.scl	2;	.type	32;	.endef
	.def	__cxa_begin_catch;	.scl	2;	.type	32;	.endef
	.def	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv;	.scl	2;	.type	32;	.endef
	.def	__cxa_rethrow;	.scl	2;	.type	32;	.endef
	.def	__cxa_end_catch;	.scl	2;	.type	32;	.endef
	.def	_Unwind_Resume;	.scl	2;	.type	32;	.endef
	.def	_Znwy;	.scl	2;	.type	32;	.endef
	.def	atexit;	.scl	2;	.type	32;	.endef
	.section	.rdata$.refptr._ZSt4cout, "dr"
	.globl	.refptr._ZSt4cout
	.linkonce	discard
.refptr._ZSt4cout:
	.quad	_ZSt4cout
