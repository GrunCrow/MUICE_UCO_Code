#include <stdio.h>
#include <cuda.h>

/*
                        Práctica 4

                Paola Montenegro Cantos
                Alba Márquez Rodríguez
*/

// Kernel para sumar las matrices en el device
__global__ void add(int *a, int *b, int *c, int N) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index < N)
        c[index] = a[index] + b[index];
}

int main() {
    int N = 16; // número de elementos del vector
    int num_bytes = N * N * sizeof(int); // tamaño en bytes

    // punteros para el host
    int *h_a = (int*) malloc(num_bytes);
    int *h_b = (int*) malloc(num_bytes);
    int *h_c = (int*) malloc(num_bytes);

    // inicializar las matrices en el host
    for (int i = 0; i < N * N; i++) {
        h_a[i] = i;
        h_b[i] = i;
    }

    // punteros para el device
    int *d_a, *d_b, *d_c;
    cudaMalloc((void**)&d_a, num_bytes);
    cudaMalloc((void**)&d_b, num_bytes);
    cudaMalloc((void**)&d_c, num_bytes);

    // copiar las matrices del host al device
    cudaMemcpy(d_a, h_a, num_bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, num_bytes, cudaMemcpyHostToDevice);

    // lanzar el kernel
    add<<<N, N>>>(d_a, d_b, d_c, N * N);

    // copiar el resultado del device al host
    cudaMemcpy(h_c, d_c, num_bytes, cudaMemcpyDeviceToHost);

    // imprimir el resultado
    for (int i = 0; i < N * N; i++)
        printf("%d ", h_c[i]);
        printf("\n");

    // liberar la memoria en el host y en el device
    free(h_a);
    free(h_b);
    free(h_c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}
