	.file	"unique_ptr_zero_overhead_test.cpp"
	.intel_syntax noprefix
	.text
	.p2align 4
	.globl	_Z15Test_new_deletev
	.def	_Z15Test_new_deletev;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z15Test_new_deletev
_Z15Test_new_deletev:
.LFB2853:
	sub	rsp, 40
	.seh_stackalloc	40
	.seh_endprologue
	mov	ecx, 4
	call	_Znwy
	mov	edx, 4
	mov	rcx, rax
	mov	QWORD PTR escape[rip], rax
	add	rsp, 40
	jmp	_ZdlPvy
	.seh_endproc
	.p2align 4
	.globl	_Z15Test_unique_ptrv
	.def	_Z15Test_unique_ptrv;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z15Test_unique_ptrv
_Z15Test_unique_ptrv:
.LFB2854:
	sub	rsp, 40
	.seh_stackalloc	40
	.seh_endprologue
	mov	ecx, 4
	call	_Znwy
	mov	edx, 4
	mov	rcx, rax
	mov	QWORD PTR escape[rip], rax
	add	rsp, 40
	jmp	_ZdlPvy
	.seh_endproc
	.section	.text.startup,"x"
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB2863:
	sub	rsp, 40
	.seh_stackalloc	40
	.seh_endprologue
	call	__main
	call	_Z15Test_unique_ptrv
	call	_Z15Test_new_deletev
	xor	eax, eax
	add	rsp, 40
	ret
	.seh_endproc
	.globl	escape
	.bss
	.align 8
escape:
	.space 8
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-Builds project) 15.2.0"
	.def	_Znwy;	.scl	2;	.type	32;	.endef
	.def	_ZdlPvy;	.scl	2;	.type	32;	.endef
