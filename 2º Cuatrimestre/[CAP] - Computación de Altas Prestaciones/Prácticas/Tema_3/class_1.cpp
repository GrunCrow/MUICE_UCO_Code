#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <xmmintrin.h> // SSE
#include <omp.h> // Parallelization

// g++ -o Tema_3\class Tema_3\class.cpp -fopenmp
// Tema_3\class.exe

int main(){

    int n = 100;

    omp_set_num_threads(3);

    printf("Number of threads: %d\n", omp_get_max_threads());

    #pragma omp parallel for
    for (int i=0;i<n;i++){
        printf("Thread %d\n", omp_get_num_threads());
        printf("I am thread %d\n", omp_get_thread_num());
    }

    return 0;
}