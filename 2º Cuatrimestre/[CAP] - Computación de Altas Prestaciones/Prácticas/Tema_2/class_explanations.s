	.file	"class_explanations.c"
 # GNU C17 (Rev3, Built by MSYS2 project) version 13.2.0 (x86_64-w64-mingw32)
 #	compiled by GNU C version 13.2.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed: -mtune=generic -march=nocona -O3
	.text
	.def	__main;	.scl	2;	.type	32;	.endef
	.section	.text.startup,"x"
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	subq	$40, %rsp	 #,
	.seh_stackalloc	40
	.seh_endprologue
 # class_explanations.c:15: int main(){
	call	__main	 #
 # class_explanations.c:29: }
	xorl	%eax, %eax	 #
	addq	$40, %rsp	 #,
	ret	
	.seh_endproc
	.ident	"GCC: (Rev3, Built by MSYS2 project) 13.2.0"
