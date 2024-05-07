#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <xmmintrin.h> // SSE
#include <omp.h> // Parallelization

// g++ -o Tema_3\class_2 Tema_3\class_2.cpp -fopenmp
// Tema_3\class_2.exe

int main () {
    int thid;
    int a;

    for (int i = 0; i < 10; i++) {
        a= 0;
        printf("Itr %d ------------------>\n", i);
        
        #pragma omp parallel sections firstprivate(a) //#pragma omp parallel sections private(a) // #pragma omp sections
        {
            // Esto indica que las siguientes secciones del código serán ejecutadas en paralelo por diferentes hilos.
            #pragma omp section
            {
                a += 1;
                thid = omp_get_thread_num();
                printf("Section 1 - Thread id=%d, a=%d\n", thid, a);
            }
            #pragma omp section
            {
                a += 1;
                thid = omp_get_thread_num();
                printf("Section 2 - Thread id=%d a=%d\n", thid, a);
            }
        }
    }
}