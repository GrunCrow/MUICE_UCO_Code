/*
    This C program allocates memory for an array of buffer_size floats, 
    initializes each element to 1.0, and then sums up all the elements in the array. 
    The sum is stored in the variable result.
*/

// gcc -S -fverbose-asm -O0 class_explanations.c
// gcc -S -fverbose-asm -O3 class_explanations.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int main(){
    int buffer_size = 100;

    // Allocate memory for the buffer
    float *b = (float *) malloc(sizeof(float) * buffer_size);
    size_t result = 0;

    // Initialize the buffer with 1.0
    for (int i=0;i<buffer_size;i++) b[i] = 1.0;

    // Sum all the elements in the buffer
    for (int i=0;i<buffer_size;i++) result += b[i];

    return 0;
}