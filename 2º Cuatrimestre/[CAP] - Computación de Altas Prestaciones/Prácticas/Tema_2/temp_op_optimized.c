// gcc temp_op.c -o temp_op
// temp_op

/*
>wmic cpu get caption, name, maxclockspeed, currentclockspeed, l2cachesize, l3cachesize, numberofcores, numberoflogicalprocessors
Caption                              CurrentClockSpeed  L2CacheSize  L3CacheSize  MaxClockSpeed  Name                                    NumberOfCores  NumberOfLogicalProcessors
AMD64 Family 23 Model 96 Stepping 1  1700               4096         8192         2900           AMD Ryzen 7 4800H with Radeon Graphics  8              16
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <xmmintrin.h> // Include SSE headers

// Block of code given in the slides - GetCurrentTime
double getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
}

void temp_op(int N) {
    // Declaration and memory allocation for vectors A, B, and C
    float *A = (float*)_mm_malloc(N * sizeof(float), 16); // malloc -> _mm_malloc : SSE optimization
    float *B = (float*)_mm_malloc(N * sizeof(float), 16);
    float *C = (float*)_mm_malloc(N * sizeof(float), 16);

    // Initialization of vectors with random values
    srand(time(NULL));
    for (int i = 0; i < N; ++i) {
        A[i] = (float)rand() / RAND_MAX;
        B[i] = (float)rand() / RAND_MAX;
        C[i] = (float)rand() / RAND_MAX;
    }

    // Start Timer
    double start = getCurrentTime();

    // A * B += C operation using SSE intrinsics
    for (int i = 0; i < N; i += 4) { // Process 4 elements at a time (SSE)
        __m128 a = _mm_load_ps(&A[i]); // Load 4 elements of A into SSE register
        __m128 b = _mm_load_ps(&B[i]); // Load 4 elements of B into SSE register
        __m128 c = _mm_load_ps(&C[i]); // Load 4 elements of C into SSE register
        __m128 result = _mm_add_ps(_mm_mul_ps(a, b), c); // Perform SSE operation
        _mm_store_ps(&A[i], result); // Store result back to memory
    }

    // Stop Timer
    double end = getCurrentTime();

    // Memory deallocation
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);

    // Print execution time
    printf("temp_op time: %lf seconds\n", end - start);
}

// Main function
int main() {
    int N_values[] = {10, 1000, 1000000, 100000000};

    // Loop through different N values
    for (int i = 0; i < sizeof(N_values) / sizeof(N_values[0]); ++i) {
        int N = N_values[i];
        printf("N = %d\n", N);
        temp_op(N);
        printf("\n");
    }
    
    return 0;
}