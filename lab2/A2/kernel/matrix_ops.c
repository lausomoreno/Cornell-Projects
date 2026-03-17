#include "matrix_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef BLOCK_SIZE_I
#define BLOCK_SIZE_I 160
#endif

#ifndef BLOCK_SIZE_J
#define BLOCK_SIZE_J 64
#endif

#ifndef BLOCK_SIZE_K
#define BLOCK_SIZE_K 64
#endif


float **matmul(float **A, float **B, int A_rows, int A_cols, int B_rows, int B_cols)
{
      
    float** output = (float**)malloc(A_rows * sizeof(float*));
    for (int i = 0; i < A_rows; i++) {
        output[i] = (float*)malloc(B_cols * sizeof(float));
    }

    if (A_cols == B_rows) {
    
        for (int i = 0; i < A_rows; i++) {
            for (int j = 0; j < B_cols; j++) {
                float sum = 0.0f;
                for (int k = 0; k < A_cols; k++) {
                    sum += A[i][k] * B[k][j];
                }
                output[i][j] = sum;
            }
        }
    
    
        return output;
    }

    else {
        return NULL;
    } 
}

// Matmul with blocking optimization
float **matmul_blocking(float **A, float **B, int A_rows, int A_cols, int B_rows, int B_cols)
{
    // TODO: Implement matrix multiplication with blocking (loop tiling)
    //allocate memory for output
    float** output = (float**)malloc(A_rows * sizeof(float*));
     if (!output) return NULL;
    for (int i = 0; i < A_rows; i++) {
        output[i] = (float*)calloc(B_cols, sizeof(float)); // zero-init for accumulation
        if (!output[i]) {
            for (int r = 0; r < i; ++r) free(output[r]);
            free(output);
            return NULL;
        }
    }
    

    //checking for correct dimensions
    if (A_cols == B_rows)
    {

    // Tiled i-k-j order (reuse A[i][k] and contiguous B[k][j] across j)
    for (int ii = 0; ii < A_rows; ii += BLOCK_SIZE_I) {
        int iimax = imin(ii + BLOCK_SIZE_I, A_rows);
        for (int jj = 0; jj < A_cols; jj += BLOCK_SIZE_J) {
            int jjmax = imin(jj + BLOCK_SIZE_J, A_cols);
            for (int kk = 0; kk < B_cols; kk += BLOCK_SIZE_K) {
                int kkmax = imin(kk + BLOCK_SIZE_K, B_cols);

                // Inner micro-kernel
                for (int i = ii; i < iimax; i++) {
                    for (int j = jj; j < jjmax; j++) {
                        float aij = A[i][j]; // keep in register
                        // Walk B row contiguously; update C row slice
                        for (int k = kk; k < kkmax; k++) {
                            output[i][k] += aij * B[j][k];
                        }
                    }
                }
            }
        }
    }

    return output;

    }

    else {
        return NULL;
    }


}

// Matmul with quantization
float **matmul_quant(unsigned int **A, int **B, int A_rows, int A_cols, int B_rows, int B_cols)
{
    // TODO: Implement matrix multiplication with quantized matrix A and unquantized matrix B.
    //       The input matrix A stores data in a 4-byte granularity, i.e., A[i][k] is a 4-byte unsigned integer.
    //       However, each element of A takes 8-bit (1-byte) storage space, meadning that A[i][k] contains multiple elements.
    //       The input matrix B is unquantized, and each element of B is a 4-byte integer.
    //       The returned output matrix should be "float". You can achieve this by performing dot product in integer, and use type casting to convert the output to float.

    if ( ((QUANT_PRECISION % 2) != 0) || (QUANT_PRECISION > 16) )
    {
        printf("Incompatible precision assigned to the matrix.\n");
        return NULL;
    }

    if ( (A_cols * (sizeof(unsigned int)*8) / QUANT_PRECISION) != B_rows )
    {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return NULL;
    }

    //There are 4 1-byte elements ineach 4-byte unsigned integer = elements per word
    const int elements_per_word = 32 / QUANT_PRECISION;

// Allocate output
    float **output = (float **)malloc(A_rows * sizeof(float *));
    if (!output) { printf("Allocation failed.\n"); return NULL; }
    for (int i = 0; i < A_rows; i++) {
        output[i] = (float *)malloc(B_cols * sizeof(float));
        if (!output[i]) {
            for (int r = 0; r < i; ++r) free(output[r]);
            free(output);
            printf("Allocation failed.\n");
            return NULL;
        }
    }

    for (int i = 0; i < A_rows; i++) {
        for (int j = 0; j < B_cols; j++) {
            int64_t sum = 0;

            for (int k = 0; k < A_cols; k++) {
                uint32_t word = A[i][k];

                // Extract bytes (least-significant byte first)
                uint32_t a0 =  word & 0xFFu;   // least-significant byte
                uint32_t a1 = (word >>  8) & 0xFFu;
                uint32_t a2 = (word >> 16) & 0xFFu;
                uint32_t a3 = (word >> 24) & 0xFFu;   // most-significant byte

                int k0 = k * elements_per_word;       // base logical row into B
                sum += (int64_t)a0 * (int64_t)B[k0 + 0][j]
                     + (int64_t)a1 * (int64_t)B[k0 + 1][j]
                     + (int64_t)a2 * (int64_t)B[k0 + 2][j]
                     + (int64_t)a3 * (int64_t)B[k0 + 3][j];
            }

            output[i][j] = (float)sum;
        }
    }

    return output;
    // HINT: 1. Since every quantized element in matrix A is 8-bit, how many elements can be packed in a 4-byte unsigned integer?
    // HINT: 2. For a 4-byte unsigned integer in matrix A, how to use Boolean masking to extract the actual 8-bit elements?
    // HINT: 3. Make sure to calculate the correct 'k' index of A and B when performing dot-product.
}