	.file	"GEMM.c"
 # GNU C17 (Rev3, Built by MSYS2 project) version 13.2.0 (x86_64-w64-mingw32)
 #	compiled by GNU C version 13.2.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed: -mtune=generic -march=nocona -O0
	.text
	.def	time;	.scl	3;	.type	32;	.endef
	.seh_proc	time
time:
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	movq	%rsp, %rbp	 #,
	.seh_setframe	%rbp, 0
	subq	$32, %rsp	 #,
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)	 # _Time, _Time
 # C:/msys64/ucrt64/include/time.h:258: static __inline time_t __CRTDECL time(time_t *_Time) { return _time64(_Time); }
	movq	16(%rbp), %rax	 # _Time, tmp84
	movq	%rax, %rcx	 # tmp84,
	movq	__imp__time64(%rip), %rax	 #, tmp85
	call	*%rax	 # tmp85
 # C:/msys64/ucrt64/include/time.h:258: static __inline time_t __CRTDECL time(time_t *_Time) { return _time64(_Time); }
	addq	$32, %rsp	 #,
	popq	%rbp	 #
	ret	
	.seh_endproc
	.globl	getCurrentTime
	.def	getCurrentTime;	.scl	2;	.type	32;	.endef
	.seh_proc	getCurrentTime
getCurrentTime:
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	movq	%rsp, %rbp	 #,
	.seh_setframe	%rbp, 0
	subq	$48, %rsp	 #,
	.seh_stackalloc	48
	.seh_endprologue
 # GEMM.c:32:     gettimeofday(&tv, NULL);
	leaq	-8(%rbp), %rax	 #, tmp89
	movl	$0, %edx	 #,
	movq	%rax, %rcx	 # tmp89,
	call	gettimeofday	 #
 # GEMM.c:33:     return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
	movl	-8(%rbp), %eax	 # tv.tv_sec, _1
 # GEMM.c:33:     return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
	pxor	%xmm1, %xmm1	 # _2
	cvtsi2sdl	%eax, %xmm1	 # _1, _2
 # GEMM.c:33:     return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
	movl	-4(%rbp), %eax	 # tv.tv_usec, _3
 # GEMM.c:33:     return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
	pxor	%xmm2, %xmm2	 # _4
	cvtsi2sdl	%eax, %xmm2	 # _3, _4
 # GEMM.c:33:     return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
	movsd	.LC0(%rip), %xmm0	 #, tmp90
	mulsd	%xmm2, %xmm0	 # _4, _5
 # GEMM.c:33:     return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
	addsd	%xmm1, %xmm0	 # _2, _8
	movq	%xmm0, %rax	 # _8, <retval>
 # GEMM.c:34: }
	movq	%rax, %xmm0	 # <retval>,
	addq	$48, %rsp	 #,
	popq	%rbp	 #
	ret	
	.seh_endproc
	.section .rdata,"dr"
.LC2:
	.ascii "Output matrix:\0"
.LC3:
	.ascii "%f \0"
	.align 8
.LC4:
	.ascii "\12Convolution time: %lf seconds\12\0"
	.text
	.globl	convolution
	.def	convolution;	.scl	2;	.type	32;	.endef
	.seh_proc	convolution
convolution:
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	movq	%rsp, %rbp	 #,
	.seh_setframe	%rbp, 0
	subq	$112, %rsp	 #,
	.seh_stackalloc	112
	.seh_endprologue
	movl	%ecx, 16(%rbp)	 # M, M
 # GEMM.c:38:     int N = M;
	movl	16(%rbp), %eax	 # M, tmp138
	movl	%eax, -28(%rbp)	 # tmp138, N
 # GEMM.c:39:     int K = N;
	movl	-28(%rbp), %eax	 # N, tmp139
	movl	%eax, -32(%rbp)	 # tmp139, K
 # GEMM.c:42:     float *A = (float*)malloc(M * K * sizeof(float)); // Input matrix
	movl	16(%rbp), %eax	 # M, tmp140
	imull	-32(%rbp), %eax	 # K, _1
	cltq
 # GEMM.c:42:     float *A = (float*)malloc(M * K * sizeof(float)); // Input matrix
	salq	$2, %rax	 #, _3
	movq	%rax, %rcx	 # _3,
	call	malloc	 #
	movq	%rax, -40(%rbp)	 # tmp141, A
 # GEMM.c:43:     float *B = (float*)malloc(K * N * sizeof(float)); // Filter matrix
	movl	-32(%rbp), %eax	 # K, tmp142
	imull	-28(%rbp), %eax	 # N, _4
	cltq
 # GEMM.c:43:     float *B = (float*)malloc(K * N * sizeof(float)); // Filter matrix
	salq	$2, %rax	 #, _6
	movq	%rax, %rcx	 # _6,
	call	malloc	 #
	movq	%rax, -48(%rbp)	 # tmp143, B
 # GEMM.c:44:     float *C = (float*)malloc(M * N * sizeof(float)); // Output matrix
	movl	16(%rbp), %eax	 # M, tmp144
	imull	-28(%rbp), %eax	 # N, _7
	cltq
 # GEMM.c:44:     float *C = (float*)malloc(M * N * sizeof(float)); // Output matrix
	salq	$2, %rax	 #, _9
	movq	%rax, %rcx	 # _9,
	call	malloc	 #
	movq	%rax, -56(%rbp)	 # tmp145, C
 # GEMM.c:47:     srand(time(NULL));
	movl	$0, %ecx	 #,
	call	time	 #
 # GEMM.c:47:     srand(time(NULL));
	movl	%eax, %ecx	 # _11,
	call	srand	 #
 # GEMM.c:48:     for (int i = 0; i < M * K; ++i) {
	movl	$0, -4(%rbp)	 #, i
 # GEMM.c:48:     for (int i = 0; i < M * K; ++i) {
	jmp	.L6	 #
.L7:
 # GEMM.c:49:         A[i] = (float)rand() / RAND_MAX;
	call	rand	 #
 # GEMM.c:49:         A[i] = (float)rand() / RAND_MAX;
	pxor	%xmm0, %xmm0	 # _13
	cvtsi2ssl	%eax, %xmm0	 # _12, _13
 # GEMM.c:49:         A[i] = (float)rand() / RAND_MAX;
	movl	-4(%rbp), %eax	 # i, tmp146
	cltq
	leaq	0(,%rax,4), %rdx	 #, _15
	movq	-40(%rbp), %rax	 # A, tmp147
	addq	%rdx, %rax	 # _15, _16
 # GEMM.c:49:         A[i] = (float)rand() / RAND_MAX;
	movss	.LC1(%rip), %xmm1	 #, tmp148
	divss	%xmm1, %xmm0	 # tmp148, _17
 # GEMM.c:49:         A[i] = (float)rand() / RAND_MAX;
	movss	%xmm0, (%rax)	 # _17, *_16
 # GEMM.c:48:     for (int i = 0; i < M * K; ++i) {
	addl	$1, -4(%rbp)	 #, i
.L6:
 # GEMM.c:48:     for (int i = 0; i < M * K; ++i) {
	movl	16(%rbp), %eax	 # M, tmp149
	imull	-32(%rbp), %eax	 # K, _18
 # GEMM.c:48:     for (int i = 0; i < M * K; ++i) {
	cmpl	%eax, -4(%rbp)	 # _18, i
	jl	.L7	 #,
 # GEMM.c:51:     for (int i = 0; i < K * N; ++i) {
	movl	$0, -8(%rbp)	 #, i
 # GEMM.c:51:     for (int i = 0; i < K * N; ++i) {
	jmp	.L8	 #
.L9:
 # GEMM.c:52:         B[i] = (float)rand() / RAND_MAX;
	call	rand	 #
 # GEMM.c:52:         B[i] = (float)rand() / RAND_MAX;
	pxor	%xmm0, %xmm0	 # _20
	cvtsi2ssl	%eax, %xmm0	 # _19, _20
 # GEMM.c:52:         B[i] = (float)rand() / RAND_MAX;
	movl	-8(%rbp), %eax	 # i, tmp150
	cltq
	leaq	0(,%rax,4), %rdx	 #, _22
	movq	-48(%rbp), %rax	 # B, tmp151
	addq	%rdx, %rax	 # _22, _23
 # GEMM.c:52:         B[i] = (float)rand() / RAND_MAX;
	movss	.LC1(%rip), %xmm1	 #, tmp152
	divss	%xmm1, %xmm0	 # tmp152, _24
 # GEMM.c:52:         B[i] = (float)rand() / RAND_MAX;
	movss	%xmm0, (%rax)	 # _24, *_23
 # GEMM.c:51:     for (int i = 0; i < K * N; ++i) {
	addl	$1, -8(%rbp)	 #, i
.L8:
 # GEMM.c:51:     for (int i = 0; i < K * N; ++i) {
	movl	-32(%rbp), %eax	 # K, tmp153
	imull	-28(%rbp), %eax	 # N, _25
 # GEMM.c:51:     for (int i = 0; i < K * N; ++i) {
	cmpl	%eax, -8(%rbp)	 # _25, i
	jl	.L9	 #,
 # GEMM.c:56:     double start = getCurrentTime();
	call	getCurrentTime	 #
	movq	%xmm0, %rax	 #, tmp154
	movq	%rax, -64(%rbp)	 # tmp154, start
 # GEMM.c:59:     for (int n = 0; n < N; n++) {
	movl	$0, -12(%rbp)	 #, n
 # GEMM.c:59:     for (int n = 0; n < N; n++) {
	jmp	.L10	 #
.L15:
 # GEMM.c:60:         for (int m = 0; m < M; m++) {
	movl	$0, -16(%rbp)	 #, m
 # GEMM.c:60:         for (int m = 0; m < M; m++) {
	jmp	.L11	 #
.L14:
 # GEMM.c:61:             for (int k = 0; k < K; k++) {
	movl	$0, -20(%rbp)	 #, k
 # GEMM.c:61:             for (int k = 0; k < K; k++) {
	jmp	.L12	 #
.L13:
 # GEMM.c:62:                 C[m * N + n] += A[m * K + k] * B[k * N + n];
	movl	-16(%rbp), %eax	 # m, tmp155
	imull	-28(%rbp), %eax	 # N, tmp155
	movl	%eax, %edx	 # tmp155, _26
	movl	-12(%rbp), %eax	 # n, tmp156
	addl	%edx, %eax	 # _26, _27
	cltq
	leaq	0(,%rax,4), %rdx	 #, _29
	movq	-56(%rbp), %rax	 # C, tmp157
	addq	%rdx, %rax	 # _29, _30
	movss	(%rax), %xmm1	 # *_30, _31
 # GEMM.c:62:                 C[m * N + n] += A[m * K + k] * B[k * N + n];
	movl	-16(%rbp), %eax	 # m, tmp158
	imull	-32(%rbp), %eax	 # K, tmp158
	movl	%eax, %edx	 # tmp158, _32
 # GEMM.c:62:                 C[m * N + n] += A[m * K + k] * B[k * N + n];
	movl	-20(%rbp), %eax	 # k, tmp159
	addl	%edx, %eax	 # _32, _33
	cltq
 # GEMM.c:62:                 C[m * N + n] += A[m * K + k] * B[k * N + n];
	leaq	0(,%rax,4), %rdx	 #, _35
	movq	-40(%rbp), %rax	 # A, tmp160
	addq	%rdx, %rax	 # _35, _36
	movss	(%rax), %xmm2	 # *_36, _37
 # GEMM.c:62:                 C[m * N + n] += A[m * K + k] * B[k * N + n];
	movl	-20(%rbp), %eax	 # k, tmp161
	imull	-28(%rbp), %eax	 # N, tmp161
	movl	%eax, %edx	 # tmp161, _38
 # GEMM.c:62:                 C[m * N + n] += A[m * K + k] * B[k * N + n];
	movl	-12(%rbp), %eax	 # n, tmp162
	addl	%edx, %eax	 # _38, _39
	cltq
 # GEMM.c:62:                 C[m * N + n] += A[m * K + k] * B[k * N + n];
	leaq	0(,%rax,4), %rdx	 #, _41
	movq	-48(%rbp), %rax	 # B, tmp163
	addq	%rdx, %rax	 # _41, _42
	movss	(%rax), %xmm0	 # *_42, _43
 # GEMM.c:62:                 C[m * N + n] += A[m * K + k] * B[k * N + n];
	mulss	%xmm2, %xmm0	 # _37, _44
 # GEMM.c:62:                 C[m * N + n] += A[m * K + k] * B[k * N + n];
	movl	-16(%rbp), %eax	 # m, tmp164
	imull	-28(%rbp), %eax	 # N, tmp164
	movl	%eax, %edx	 # tmp164, _45
	movl	-12(%rbp), %eax	 # n, tmp165
	addl	%edx, %eax	 # _45, _46
	cltq
	leaq	0(,%rax,4), %rdx	 #, _48
	movq	-56(%rbp), %rax	 # C, tmp166
	addq	%rdx, %rax	 # _48, _49
 # GEMM.c:62:                 C[m * N + n] += A[m * K + k] * B[k * N + n];
	addss	%xmm1, %xmm0	 # _31, _50
	movss	%xmm0, (%rax)	 # _50, *_49
 # GEMM.c:61:             for (int k = 0; k < K; k++) {
	addl	$1, -20(%rbp)	 #, k
.L12:
 # GEMM.c:61:             for (int k = 0; k < K; k++) {
	movl	-20(%rbp), %eax	 # k, tmp167
	cmpl	-32(%rbp), %eax	 # K, tmp167
	jl	.L13	 #,
 # GEMM.c:60:         for (int m = 0; m < M; m++) {
	addl	$1, -16(%rbp)	 #, m
.L11:
 # GEMM.c:60:         for (int m = 0; m < M; m++) {
	movl	-16(%rbp), %eax	 # m, tmp168
	cmpl	16(%rbp), %eax	 # M, tmp168
	jl	.L14	 #,
 # GEMM.c:59:     for (int n = 0; n < N; n++) {
	addl	$1, -12(%rbp)	 #, n
.L10:
 # GEMM.c:59:     for (int n = 0; n < N; n++) {
	movl	-12(%rbp), %eax	 # n, tmp169
	cmpl	-28(%rbp), %eax	 # N, tmp169
	jl	.L15	 #,
 # GEMM.c:68:     double end = getCurrentTime();
	call	getCurrentTime	 #
	movq	%xmm0, %rax	 #, tmp170
	movq	%rax, -72(%rbp)	 # tmp170, end
 # GEMM.c:71:     free(A);
	movq	-40(%rbp), %rax	 # A, tmp171
	movq	%rax, %rcx	 # tmp171,
	call	free	 #
 # GEMM.c:72:     free(B);
	movq	-48(%rbp), %rax	 # B, tmp172
	movq	%rax, %rcx	 # tmp172,
	call	free	 #
 # GEMM.c:75:     printf("Output matrix:\n");
	leaq	.LC2(%rip), %rax	 #, tmp173
	movq	%rax, %rcx	 # tmp173,
	call	puts	 #
 # GEMM.c:76:     for (int i = 0; i < 10; ++i) {
	movl	$0, -24(%rbp)	 #, i
 # GEMM.c:76:     for (int i = 0; i < 10; ++i) {
	jmp	.L16	 #
.L17:
 # GEMM.c:77:         printf("%f ", C[i]);
	movl	-24(%rbp), %eax	 # i, tmp174
	cltq
	leaq	0(,%rax,4), %rdx	 #, _52
	movq	-56(%rbp), %rax	 # C, tmp175
	addq	%rdx, %rax	 # _52, _53
	movss	(%rax), %xmm0	 # *_53, _54
 # GEMM.c:77:         printf("%f ", C[i]);
	cvtss2sd	%xmm0, %xmm0	 # _54, _55
	movq	%xmm0, %rax	 # _55, tmp176
	movq	%rax, %rdx	 # tmp176, tmp177
	movq	%rdx, %xmm0	 # tmp177, tmp179
	movapd	%xmm0, %xmm1	 # tmp179,
	movq	%rax, %rdx	 # tmp180,
	leaq	.LC3(%rip), %rax	 #, tmp181
	movq	%rax, %rcx	 # tmp181,
	call	printf	 #
 # GEMM.c:76:     for (int i = 0; i < 10; ++i) {
	addl	$1, -24(%rbp)	 #, i
.L16:
 # GEMM.c:76:     for (int i = 0; i < 10; ++i) {
	cmpl	$9, -24(%rbp)	 #, i
	jle	.L17	 #,
 # GEMM.c:81:     printf("\nConvolution time: %lf seconds\n", end - start);
	movsd	-72(%rbp), %xmm0	 # end, tmp182
	subsd	-64(%rbp), %xmm0	 # start, _56
	movq	%xmm0, %rax	 # _56, tmp183
	movq	%rax, %rdx	 # tmp183, tmp184
	movq	%rdx, %xmm0	 # tmp184, tmp186
	movapd	%xmm0, %xmm1	 # tmp186,
	movq	%rax, %rdx	 # tmp187,
	leaq	.LC4(%rip), %rax	 #, tmp188
	movq	%rax, %rcx	 # tmp188,
	call	printf	 #
 # GEMM.c:82: }
	nop	
	addq	$112, %rsp	 #,
	popq	%rbp	 #
	ret	
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC5:
	.ascii "N = %d\12\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	movq	%rsp, %rbp	 #,
	.seh_setframe	%rbp, 0
	subq	$48, %rsp	 #,
	.seh_stackalloc	48
	.seh_endprologue
 # GEMM.c:86: int main() {
	call	__main	 #
 # GEMM.c:87:     int N_values[] = {500};
	movl	$500, -12(%rbp)	 #, N_values[0]
 # GEMM.c:90:     for (int i = 0; i < sizeof(N_values) / sizeof(N_values[0]); ++i) {
	movl	$0, -4(%rbp)	 #, i
 # GEMM.c:90:     for (int i = 0; i < sizeof(N_values) / sizeof(N_values[0]); ++i) {
	jmp	.L19	 #
.L20:
 # GEMM.c:91:         int N = N_values[i];
	movl	-4(%rbp), %eax	 # i, tmp85
	cltq
	movl	-12(%rbp,%rax,4), %eax	 # N_values[i_1], tmp86
	movl	%eax, -8(%rbp)	 # tmp86, N
 # GEMM.c:92:         printf("N = %d\n", N);
	movl	-8(%rbp), %eax	 # N, tmp87
	movl	%eax, %edx	 # tmp87,
	leaq	.LC5(%rip), %rax	 #, tmp88
	movq	%rax, %rcx	 # tmp88,
	call	printf	 #
 # GEMM.c:93:         convolution(N);
	movl	-8(%rbp), %eax	 # N, tmp89
	movl	%eax, %ecx	 # tmp89,
	call	convolution	 #
 # GEMM.c:94:         printf("\n");
	movl	$10, %ecx	 #,
	call	putchar	 #
 # GEMM.c:90:     for (int i = 0; i < sizeof(N_values) / sizeof(N_values[0]); ++i) {
	addl	$1, -4(%rbp)	 #, i
.L19:
 # GEMM.c:90:     for (int i = 0; i < sizeof(N_values) / sizeof(N_values[0]); ++i) {
	cmpl	$0, -4(%rbp)	 #, i
	je	.L20	 #,
 # GEMM.c:97:     return 0;
	movl	$0, %eax	 #, _6
 # GEMM.c:98: }
	addq	$48, %rsp	 #,
	popq	%rbp	 #
	ret	
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC0:
	.long	-1598689907
	.long	1051772663
	.align 4
.LC1:
	.long	1191181824
	.ident	"GCC: (Rev3, Built by MSYS2 project) 13.2.0"
	.def	gettimeofday;	.scl	2;	.type	32;	.endef
	.def	malloc;	.scl	2;	.type	32;	.endef
	.def	srand;	.scl	2;	.type	32;	.endef
	.def	rand;	.scl	2;	.type	32;	.endef
	.def	free;	.scl	2;	.type	32;	.endef
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	putchar;	.scl	2;	.type	32;	.endef
