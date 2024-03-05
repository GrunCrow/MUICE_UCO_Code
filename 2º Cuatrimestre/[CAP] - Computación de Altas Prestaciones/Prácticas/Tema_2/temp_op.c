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

// Block of code given in the slides - GetCurrentTime
double getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
}

void temp_op(int N) {
    // Declaration and memory allocation for vectors A, B, and C
    double *A = (double*)malloc(N * sizeof(double));
    double *B = (double*)malloc(N * sizeof(double));
    double *C = (double*)malloc(N * sizeof(double));

    // Initialization of vectors with random values
    srand(time(NULL));
    for (int i = 0; i < N; ++i) {
        A[i] = (double)rand() / RAND_MAX;
        B[i] = (double)rand() / RAND_MAX;
        C[i] = (double)rand() / RAND_MAX;
    }

    // Start Timer
    double start = getCurrentTime();

    // A * B += C operation
    for (int i = 0; i < N; ++i) {
        A[i] = A[i] * B[i] + C[i];
    }

    // Stop Timer
    double end = getCurrentTime();

    // Memory deallocation
    free(A);
    free(B);
    free(C);

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
