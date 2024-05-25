/*===============================================================================================

                      PROYECTO DE LA ASIGNATURA COMPUTACIÓN DE ALTAS PRESTACIONES

                                      Alba Márquez Rodríguez
                                      Paola Montenegro Cantos

================================================================================================*/

// gcc driver_gemm.c -o driver_gemm
// driver_gemm

// Número de hilos: 2
// Matrices almacenadas por filas


// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <xmmintrin.h> // SSE
#include <omp.h> // Parallelization

//Definitions
#define DT float
#define CNN_LAYERS 20
#define CNN_LOOP   CNN_LAYERS

//Macros Aux
#define dabs(a) ( (a) > 0.0 ? (a) : -(a) )

//Colors
#define COLOR_RESET      "\033[0m"
#define COLOR_GREEN      "\033[32m"
#define COLOR_RED        "\033[31m"

//Headers
void init_tensor(size_t, DT *);
void gemm_base(DT *, DT *, DT *, size_t, size_t, size_t);
double dclock();
double validate_multiplication(DT *, DT *, size_t, size_t);

//Optimized GEMM
//un registro para la columna de a , 4 registros para b  y  4 registros para c, cargados por filas
void gemm_op(DT *A, DT *B, DT *C, size_t M, size_t N, size_t K) {
    size_t m, n, k;
    for (m = 0; m < M; m++) {
        for (k = 0; k < K; k++) {
            __m128 a_vec_1 = _mm_set1_ps(A[m * K + k]); // Carga un valor de 'A' en un registro SIMD
          
            for (n = 0; n < N ; n += 4) {
                __m128 b_vec_1 = _mm_load_ps(&B[k * N + n]); // Carga 4 valores de 'B' en un registro SIMD
                __m128 c_vec_1 = _mm_load_ps(&C[m * N + n]); // Carga 4 valores de 'C' en un registro SIMD
                // Multiplica los valores en 'a_vec' y 'b_vec' y almacena el resultado en 'c_vec'
                c_vec_1 = _mm_add_ps(c_vec_1,_mm_mul_ps(a_vec_1, b_vec_1));
                _mm_store_ps(&C[m * N + n], c_vec_1); // Almacena los valores en 'c_vec' de vuelta en 'C'

            }
        }
    }
}






//Main program
int main() {

    size_t M, N, K, nreps;
    DT *A, *B, *C, *Cref;
    double t1, t2, tref, top, Gfref, Gfop, err;
    double ref_avg, op_avg;
    double tmin = 2.0;

    size_t CNN[CNN_LAYERS][3] = {{12544, 64,  147},
                                 {3136,  64,  64},
                                 {3136,  64,  576},
                                 {3136,  256, 64},
                                 {3136,  64,  256},
                                 {3136,  128, 256},
                                 {784, 128, 1152},
                                 {784, 512, 128},
                                 {784, 512, 256},
                                 {784, 128, 512},
                                 {784, 256, 512},
		    	                 {196, 256,  2304},
                                 {196, 1024, 256},
                                 {196, 1024, 512},
                                 {196, 256,  1024},
                                 {196, 512,  1024},
			                     {49, 512,  4608},
                                 {49, 2048, 512},
                                 {49, 2048, 1024},
                                 {49, 512,  2048}} ;

    printf("\n");
    printf("+----------------------------------------------------------------------------------+\n");
    printf("|                            GEMM Optimizator Driver                               |\n");
    printf("+-----------------------------+------------------------------------------+---------+\n");
    printf("|     M        N        K     |   Ref-GFlops    OP-GFlops    Speed-up    |  Test   |\n");
    printf("+-----------------------------+------------------------------------------+---------+\n");

    ref_avg = 0.0;
    op_avg  = 0.0;

    for (uint32_t layer=0; layer < CNN_LOOP; layer++) {
        M = CNN[layer][0];
        N = CNN[layer][1];
        K = CNN[layer][2];

        A    = (DT *)malloc(sizeof(DT) * M * K);
        B    = (DT *)malloc(sizeof(DT) * N * K);
        C    = (DT *)calloc(M * N, sizeof(DT));
        Cref = (DT *)calloc(M * N, sizeof(DT));

        init_tensor(M * K, A);
        init_tensor(N * K, B);

        //Reference Timing
        nreps = 0;
        t1 = dclock();
        while (tref <= tmin) {
            nreps++;
            gemm_base(A, B, Cref, M, N, K);
            t2 = dclock();
            tref = ( t2 > t1 ? t2 - t1 : 0.0 );
        }
        tref = tref / nreps;
        Gfref = (2.0 * M * N * K) / (1.0e+9 * tref );

        //New implementation Timing
        nreps = 0;
        t1 = dclock();
        while (top <= tmin) {
            nreps++;
            gemm_op(A, B, C, M, N, K);
            t2 = dclock();
            top = ( t2 > t1 ? t2 - t1 : 0.0 );
        }
        top = top / nreps;
        Gfop  = (2.0 * M * N * K) / (1.0e+9 * top );

        //Algorithm Validation
        for (size_t i=0; i < M*N; i++) { Cref[i] = (DT)0; C[i] = (DT)0; }
        gemm_base(A, B, Cref, M, N, K);
        gemm_op(A, B, C, M, N, K);

        printf("|  %-8d  %-8d  %-6d |    %-8.2f       %-8.2f     %-8.2f  |", M, N, K, Gfref, Gfop, Gfop / Gfref);

        err=validate_multiplication(C, Cref,  M,  N);
        if (err < 1.0e-6) printf("    OK   |\n");
        else              printf("  ERROR  |\n");

        free(A);
        free(B);
        free(C);
        free(Cref);

        ref_avg += Gfref;
        op_avg  += Gfop; 
    }

    printf("+-----------------------------+------------------------------------------+---------+\n");
    printf("|          AVERAGE            |    %-8.2f       %-8.2f     %-8.2f  |    -    |\n", ref_avg / CNN_LOOP, op_avg / CNN_LOOP, (op_avg / CNN_LOOP) / (ref_avg / CNN_LOOP) );
    printf("+-----------------------------+------------------------------------------+---------+\n");

}

//************* DON'T CHANGE THIS SECTION! ********************//

//Auxiliar functions
void gemm_base(DT *A, DT *B, DT *C, size_t M, size_t N, size_t K) {
  __m128 a_row, b_row, c_row, c_tmp;
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      c_row = _mm_setzero_ps(); // Inicializar el registro SSE de salida a cero
      for (size_t k = 0; k < K; k++) {
        // Cargar un elemento de 'A' y un elemento de 'B' en registros SSE
        a_row = _mm_set1_ps(A[i*K+k]);
        b_row = _mm_set1_ps(B[k*N+j]);
        // Realizar la multiplicación de matriz por matriz y acumular en 'c_row'
        c_tmp = _mm_mul_ps(a_row, b_row);
        c_row = _mm_add_ps(c_row, c_tmp);
      }
      // Sumar el resultado a la matriz de salida 'C'
      C[i*N + j] += ((DT*)&c_row)[0];
    }
  }
}


double validate_multiplication(DT *C, DT *Ct, size_t M, size_t N) {
  double error = 0.0;
  double nrm   = 0.0;
  double tmp;
  
  for ( size_t i = 0; i < M; i++ ) {
    for ( size_t j = 0; j < N; j++ ) {
      tmp = (double) Ct[i * N + j];
      nrm += tmp*tmp;
      tmp = (double) dabs(C[i * N + j] - Ct[i * N + j]);
      error += tmp*tmp;
    }
  }

  if ( nrm != 0.0 )
    error = sqrt(error) / sqrt(nrm);
  else
    error = sqrt(error);

  return error;
}

double dclock() {
  struct timeval  tv;
  gettimeofday( &tv, NULL );
  return (double) (tv.tv_sec + tv.tv_usec*1.0e-6);
}

void init_tensor(size_t size, DT *T) {
    for (size_t i=0; i < size; i++) T[i] = (DT) (i % 10);
}