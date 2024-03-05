/* 
   This program is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
   You should have received a copy of the GNU General Public License along with
   this program. If not, see <http://www.gnu.org/licenses/>.

   -----

   author    = "Héctor Martínez Pérez" contact   = "el2mapeh@uco.es"
   copyright = "Copyright 2023, Universidad de Córdoba"
   license   = "GPLv3"
   status    = "Production"
   version   = "1.1"
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <emmintrin.h>

#include "include/gemm_library.h"
#include "include/my_gemm.h"

#define CNN_LAYERS 30

#define UNDEFINED  -1
#define MAX_RAND   10

//COLORS
#define COLOR_RESET      "\033[0m"
#define COLOR_GREEN      "\033[32m"
#define COLOR_RED        "\033[31m"
#define COLOR_BOLDRED    "\033[1m\033[31m"
#define COLOR_BOLDGREEN  "\033[1m\033[32m"
#define COLOR_BOLDYELLOW "\033[1m\033[33m"
#define COLOR_BOLDWHITE   "\033[1m\033[37m"

typedef float DT;

void new_tensor (DT **, size_t);
void free_tensor(DT **);
void init_tensor(DT *, size_t);
void show_tensor(DT *, size_t, size_t);

int main(int argc, char **argv) {
  /*************************************************************************/
  // Driver developed for testing matrix multiplicacions performance in CNN
  //
  // This software has been developed solely and exclusively for teaching 
  // purposes.
  /*************************************************************************/
  
  //DON'T CHANGE THIS VALUES!! 
  const double errorthd = 1.0e-5;
   
  DT *A, *B, *C, *Ct;

  size_t m, n, k;
  unsigned int l, nreps;
  double error, tmult, t1, t2, flops, gflops, gflopsREF, errorREF;
  double totgflops, totgflopsREF;
  
  int visual  = atoi(argv[1]); 
  int level   = atoi(argv[2]);
  int dataset = atoi(argv[3]);

  printf("\n");
  printf("[*] Level Selected    : %s", COLOR_BOLDYELLOW);
  switch(level) {
      case 1:
	      printf("EASY (25%%)");
	      break;
      case 2:
	      printf("MEDIUM (50%%)");
	      break;
      case 3:
	      printf("HARD (75%%)");
	      break;
      case 4:
	      printf("VERY HARD (100%%)");
	      break;
      default:
	      printf("Level Error. Please insert a correct level value [0-4].\n"); 
	      exit(-1);
	      break;
  };
  printf("%s\n", COLOR_RESET);
  printf("[*] Data Tesing       : %sFLOATS%s\n", COLOR_BOLDYELLOW, COLOR_RESET);
  
  // ResNet50v15
  size_t nlayers = 20;
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
  if (dataset == 1) {
    printf("[*] Convolutions GEMM : %sResNet50v15_imagenet%s\n\n", COLOR_BOLDYELLOW, COLOR_RESET);
  } else {
    printf("[*] Convolutions GEMM : %sTesting%s\n\n", COLOR_BOLDYELLOW, COLOR_RESET);
  
    nlayers = 2;
    //Small Test0
    CNN[0][0] = 5;
    CNN[0][1] = 5;
    CNN[0][2] = 5;
    //Small Test1
    CNN[1][0] = 10;
    CNN[1][1] = 10;
    CNN[1][2] = 10;
  }

  totgflops    = 0.0;
  totgflopsREF = 0.0;


  printf("========================================================================================================\n"); 
  printf("|                %sD R I V E R    F O R    M A T R I X    M U L T I P L I C A T I O N%s                    |\n", COLOR_BOLDYELLOW, COLOR_RESET);
  printf("========================================================================================================\n"); 
  printf("|       MATRIX DIMENSIONS         |      YOUR IMPLEMENTATION     |     GOAL IMPLEMENTATION      | TEST |\n");
  printf("+---------------------------------+------------------------------+------------------------------|      |\n");
  printf("|      m        n        k        |    GFlops      Test Error    |   GFlops      Test Error     |  WIN |\n");
  printf("+---------------------------------+------------------------------+------------------------------+------+\n");


  for (l = 0; l < nlayers; l++) { 
    m = CNN[l][0]; 
    n = CNN[l][1]; 
    k = CNN[l][2];

    /** MALLOC Tensors **/
    //Tensor A = m x k
    new_tensor(&A, m * k);
    //Tensor B = k x n 
    new_tensor(&B, k * n);
    //Tensor C = m x n
    new_tensor(&C,  (m + m % 4) * (n + n % 4));

    /** INIT Tensors **/
    init_tensor(A, m * k);
    init_tensor(B, k * n);

    //memset(C,  0, (m + (m % 4))* (n + n % 4) * sizeof(DT));

    //===========================================//
    //   YOUR MATRIX MULTIPLICATION EVALUATION   // 
    //===========================================//
    nreps = 1;
    t1 = dclock();
    //PUT YOUR <<GEMM>> IMPLEMENTATION HERE!!
    my_gemm(A, B, C, m, n, k); 
    //PUT YOUR <<GEMM>> IMPLEMENTATION HERE!! 
    t2    = dclock();
    tmult = ( t2 > t1 ? t2 - t1 : 0.0 );
    tmult = tmult / nreps;

    flops  = 2.0 * m * n * k;
    gflops = flops / (1.0e+9 * tmult );
    //===========================================//
  
    /** RESULT VALIDATION: Test Matrix **/
    new_tensor(&Ct, (m + m % 4) * (n + n % 4));
    //memset(Ct, 0, (m + (m % 4))* (n + n % 4) * sizeof(DT));
      
    base_matrix_multiplication(A, B, Ct, m, n, k);
    error = validate_multiplication(C, Ct, m, n);
      
    
    if (visual) {
      printf("\n+----------+\n");
      printf("| TENSOR A |\n");
      printf("+----------+\n");
      show_tensor(A, m, k);

      printf("\n+----------+\n");
      printf("| TENSOR B |\n");
      printf("+----------+\n");
      show_tensor(B, k, n);
    
      printf("\n+----------+\n");
      printf("| TENSOR C |\n");
      printf("+----------+\n");
      show_tensor(C, m, n);
      
      printf("\n+----------+\n");
      printf("| TENSOR Ct|\n");
      printf("+----------+\n");
      show_tensor(Ct, m, n);
    }

    gflopsREF = gemm_reference(A, B, C, m, n, k, level, &errorREF);

    if (error <= errorthd) 
      printf("|%8zu %8zu %8zu       |   %6.2f      %s%10.2e%s     |  %6.2f    ", m, n, k,  gflops, COLOR_GREEN, error, COLOR_RESET, gflopsREF );
    else { 
      printf("|%8zu %8zu %8zu       |   %6.2f      %s%10.2e%s    \n", m, n, k, gflops, COLOR_RED, error, COLOR_RESET );
    }

    if (errorREF <= errorthd) 
      printf("  %s%10.2e%s      | ", COLOR_GREEN, errorREF, COLOR_RESET );
    else {
      printf("  %s%10.2e%s      | ", COLOR_RED, errorREF, COLOR_RESET );
    }

    if ((error <= errorthd) && (gflops >= gflopsREF)) 
        printf(" %sYES%s |\n", COLOR_BOLDGREEN, COLOR_RESET);
    else 
        printf(" %sNO%s  |\n", COLOR_BOLDRED, COLOR_RESET);
    
    
    totgflops    += gflops;
    totgflopsREF += gflopsREF;

    /** FREE Tensors **/
    free_tensor(&A);
    free_tensor(&B);
    free_tensor(&C);
    free_tensor(&Ct);
  }

  printf("+---------------------------------+------------------------------+------------------------------+------+\n");
  if (dataset == 1) {
    if (totgflops > totgflopsREF)
      printf("|            %sP A S S!%s             |   %s%6.2f%s                     |  %s%6.2f%s                      |      |\n", COLOR_BOLDGREEN, COLOR_RESET, 
  		                                                                                                                     COLOR_BOLDWHITE, totgflops/nlayers, COLOR_RESET,
  		                                                                                                                     COLOR_BOLDWHITE, totgflopsREF/nlayers, COLOR_RESET);
    else 
      printf("|       %sN O T    P A S S!%s         |   %s%6.2f%s                     |  %s%6.2f%s                      |      |\n", COLOR_BOLDRED, COLOR_RESET, 
		                                                                                                                    COLOR_BOLDWHITE, totgflops/nlayers, COLOR_RESET, 
		                                                                                                                    COLOR_BOLDWHITE, totgflopsREF/nlayers, COLOR_RESET); 
  } else {
      printf("|        %sD I S A B L E D%s          |   %s%6.2f%s                     |  %s%6.2f%s                      |      |\n", COLOR_BOLDYELLOW, COLOR_RESET, 
		                                                                                                                    COLOR_BOLDWHITE, totgflops/nlayers, COLOR_RESET, 
		                                                                                                                    COLOR_BOLDWHITE, totgflopsREF/nlayers, COLOR_RESET); 
  
  }
  printf("+------------------------------------------------------------------------------------------------------+\n");
  printf("\n");

  return 0;

}


//-------------------------------------------------
// TENSOR DEFINITIONS
//-------------------------------------------------
void show_tensor(DT *T, size_t rows, size_t cols) {
  size_t ldT = cols;

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      printf("%-5.3f ", Trow(i, j));
    }
    printf("\n");
  }
  printf("\n");
}

void new_tensor(DT **tensor, size_t size) {
  *tensor = (DT *) calloc (size, sizeof(DT));
}

void init_tensor(DT *tensor, size_t size ) {
   for (size_t i = 0; i < size; i++)
     (tensor)[i] = ((DT) rand()) / (DT)RAND_MAX;
}

void free_tensor(DT **tensor) { free(*tensor); }

