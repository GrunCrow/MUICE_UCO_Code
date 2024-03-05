// gcc Tema_1/temp_op.c -o Tema_1/temp_op
// Tema_1/temp_op

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
    //Declaración y reserva de memoria (z, y, x)
    double *a = (double*)malloc(N * sizeof(double));
    double *b = (double*)malloc(N * sizeof(double));
    double *c = (double*)malloc(N * sizeof(double));
    double *result = (double*)malloc(N * sizeof(double));

    srand(time(NULL));

    // Initializate variables
    for (int i = 0; i < N; ++i) {
        a[i] = (double)rand() / RAND_MAX;
        b[i] = (double)rand() / RAND_MAX;
        c[i] = (double)rand() / RAND_MAX;
    }

    double beta = (double)rand() / RAND_MAX;

    // Start Timer
    double start = getCurrentTime();

    // RESULT = A + B + C * BETA
    for (int i = 0; i < N; ++i) {
        result[i] = a[i] + b[i] + c[i] * beta;
    }

    // Stop Timer
    double end = getCurrentTime();

    // Liberación de memoria
    free(a);
    free(b);
    free(c);
    free(result);

    printf("temp_op time: %lf seconds\n", end - start);
}

int main() {
    int N_values[] = {10, 1000, 1000000};

    for (int i = 0; i < sizeof(N_values) / sizeof(N_values[0]); ++i) {
        int N = N_values[i];
        printf("N = %d\n", N);
        temp_op(N);
        printf("\n");
    }
    
    return 0;
}
