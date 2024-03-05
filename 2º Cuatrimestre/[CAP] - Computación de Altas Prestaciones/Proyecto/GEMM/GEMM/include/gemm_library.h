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

#ifndef GEMM_LIBRARY_H
#define GEMM_LIBRARY_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <emmintrin.h>

#define A_BLOCK 4
#define B_BLOCK 4

// NOTE: We assume that all matrices are stored by ROWS.
// Access to Tensors store data
#define Trow(a1, a2) T[a1 * ldT + a2]

#define Arow(a1, a2)  A [a1 * ldA  + a2]
#define Brow(a1, a2)  B [a1 * ldB  + a2]
#define Crow(a1, a2)  C [a1 * ldC  + a2]
#define Ctrow(a1, a2) Ct[a1 * ldCt + a2]

//UTILS: Absolute values.
#define dabs(a) ( (a) > 0.0 ? (a) : -(a) )

typedef float DT;

double dclock();

void base_matrix_multiplication(DT *, DT *, DT *, size_t, size_t, size_t);
double validate_multiplication(DT *, DT *, size_t, size_t);
double gemm_reference(DT* A, DT* B, DT* C, size_t m, size_t n, size_t k, int mode, double *error);

#endif
