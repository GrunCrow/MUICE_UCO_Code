// gcc temp_op.c -o temp_op
// temp_op

/*
>wmic cpu get caption, name, maxclockspeed, currentclockspeed, l2cachesize, l3cachesize, numberofcores, numberoflogicalprocessors
Caption                              CurrentClockSpeed  L2CacheSize  L3CacheSize  MaxClockSpeed  Name                                    NumberOfCores  NumberOfLogicalProcessors
AMD64 Family 23 Model 96 Stepping 1  1700               4096         8192         2900           AMD Ryzen 7 4800H with Radeon Graphics  8              16
*/

/*
                                                Convolutions
    
    Implementation of a convolution operation using IM2COL/IM2ROW + GEMM
    
    The input matrix is of size N x K and the filter matrix is of size 3 x 3. The output matrix is of size (N - 2) x (K - 2).

    The input and filter matrices are initialized with random values. 
    
    The convolution operation is performed using IM2COL/IM2ROW + GEMM. 
    
    The execution time of the convolution operation is printed for different values of N.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

// Function to get current time
double getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
}

// Function to perform convolution using IM2COL/IM2ROW + GEMM
void convolution(int M) {
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

    double time = end - start;

    // Number of operations - Arithmetic = 2
    double flops = (M * N * K) * 2;
    double gflops = flops / 1.0e9 * time;

    // Memory deallocation
    free(A);
    free(B);

    // Print first element
    printf("Output matrix:\n");
    for (int i = 0; i < 1; ++i) {
        printf("%f ", C[i]);
    }

    // Print execution time
    printf("\nConvolution time: %lf seconds\n", time);
    printf("FLOPS: %f\n", flops);
    printf("GFLOPS: %f\n", gflops);
}


// Main function
int main() {
    int N_values[] = {500};

    // Loop through different N values
    for (int i = 0; i < sizeof(N_values) / sizeof(N_values[0]); ++i) {
        int N = N_values[i];
        printf("N = %d\n", N);
        convolution(N);
        printf("\n");
    }
    
    return 0;
}
