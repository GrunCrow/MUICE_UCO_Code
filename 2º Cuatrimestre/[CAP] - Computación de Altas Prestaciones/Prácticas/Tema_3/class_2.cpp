#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <xmmintrin.h> // SSE
#include <omp.h> // Parallelization

// g++ -o Tema_3\class_2 Tema_3\class_2.cpp -fopenmp
// Tema_3\class_2.exe

int a, b, i, tid;
float x;

#pragma omp threadprivate(a, x) //¿Qué ocurre?
int main () {
    omp_set_dynamic(0); /* Explicitly turn off dynamic threads */

    #pragma omp parallel private(b,tid)
    {
        tid = omp_get_thread_num();
        a = tid;
        b = tid;
        x = 1.1 * tid +1.0;
        printf("0: Thread %d: a,b,x= %d %d %f\n",tid,a,b,x);
    } /* end of parallel section */

    #pragma omp parallel private(tid)
    {
        tid = omp_get_thread_num();
        printf("1: Thread %d: a,b,x= %d %d %f\n",tid,a,b,x);
    } /* end of parallel section */
}
