#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <xmmintrin.h> // SSE

#define BLOCK_SIZE 4 // Define block size for SSE operations

// Function to get current time
double getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
}

// Function to perform convolution using IM2COL/IM2ROW + GEMM without optimization
double convolution_no_optimization(int M) {
    printf("========================================\n");
    printf("Convolution without optimization\n");
    printf("========================================\n");
    int N = M;
    int K = N;

    // Declaration and memory allocation for input and filter matrices
    float *A = (float*)malloc(M * K * sizeof(float)); // Input matrix
    float *B = (float*)malloc(K * N * sizeof(float)); // Filter matrix
    float *C = (float*)malloc(M * N * sizeof(float)); // Output matrix

    // Initialization of input and filter matrices with random values
    srand(time(NULL));
    for (int i = 0; i < M * K; ++i) {
        A[i] = (float)rand() / RAND_MAX;
    }
    for (int i = 0; i < K * N; ++i) {
        B[i] = (float)rand() / RAND_MAX;
    }

    // Start Timer
    double start = getCurrentTime();

    // Convolution operation using IM2COL/IM2ROW + GEMM
    for (int n = 0; n < N; n++) {
        for (int m = 0; m < M; m++) {
            for (int k = 0; k < K; k++) {
                C[m * N + n] += A[m * K + k] * B[k * N + n];
            }
        }
    }

    // Stop Timer
    double end = getCurrentTime();

    // Calculate execution time
    double time = end - start;

    // Number of operations - Arithmetic = 2
    double flops = (M * N * K) * 2;
    double gflops = flops / 1.0e9 * time;

    printf("\nC value: %lf\n", C[0]);

    // Memory deallocation
    free(A);
    free(B);

    // Print execution time
    printf("\nConvolution time: %lf seconds\n", time);
    printf("FLOPS: %f\n", flops);
    printf("GFLOPS: %f\n\n", gflops);

    // Return flops and gflops
    return gflops;
}

// Function to perform convolution using IM2COL/IM2ROW + GEMM with SSE optimization
double convolution_with_sse_optimization(int M) {
    printf("========================================\n");
    printf("Convolution with SSE optimization\n");
    printf("========================================\n");

    int N = M;
    int K = N;

    // Declaration and memory allocation for input and filter matrices
    float *A = (float*)malloc(M * K * sizeof(float)); // Input matrix
    float *B = (float*)malloc(K * N * sizeof(float)); // Filter matrix
    float *C = (float*)malloc(M * N * sizeof(float)); // Output matrix

    // Initialization of input and filter matrices with random values
    srand(time(NULL));
    for (int i = 0; i < M * K; ++i) {
        A[i] = (float)rand() / RAND_MAX;
    }
    for (int i = 0; i < K * N; ++i) {
        B[i] = (float)rand() / RAND_MAX;
    }

    // Start Timer
    double start = getCurrentTime();

    // Convolution operation using IM2COL/IM2ROW + GEMM with SSE optimization
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n += BLOCK_SIZE) {
            __m128 sum = _mm_setzero_ps(); // Initialize sum register to zeros
            for (int k = 0; k < K; k++) {
                __m128 a = _mm_set1_ps(A[m * K + k]); // Load A[m * K + k] into a SIMD register
                __m128 b = _mm_loadu_ps(&B[k * N + n]); // Load 4 elements of B[k * N + n] into a SIMD register
                sum = _mm_add_ps(sum, _mm_mul_ps(a, b)); // Perform SIMD multiplication and addition
            }
            _mm_storeu_ps(&C[m * N + n], sum); // Store the result back to memory
        }
    }

    // Stop Timer
    double end = getCurrentTime();

    // Calculate execution time
    double time = end - start;

    // Number of operations - Arithmetic = 2
    double flops = (M * N * K) * 2;
    double gflops = flops / 1.0e9 * time;

    // Memory deallocation
    free(A);
    free(B);

    printf("\nC value: %lf\n", C[0]);

    // Print execution time
    printf("\nConvolution time: %lf seconds\n", time);
    printf("FLOPS: %f\n", flops);
    printf("GFLOPS: %f\n\n", gflops);

    // Return flops and gflops
    return gflops;
}

// Main function to compare execution times and calculate speedup
int main() {
    int N_values[] = {500};

    // Loop through different N values
    for (int i = 0; i < sizeof(N_values) / sizeof(N_values[0]); ++i) {
        int N = N_values[i];
        printf("N = %d\n", N);

        double gflops_no_optimization = convolution_no_optimization(N);
        double gflops_sse_optimization = convolution_with_sse_optimization(N);

        printf("\n----------------------------------------\n");

        // Calculate speedup
        double speedup = gflops_no_optimization / gflops_sse_optimization;

        // Print speedup
        printf("\nSpeedup: %lf\n", speedup);
        printf("\n");
    }
    
    return 0;
}
