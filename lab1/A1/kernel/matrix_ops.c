#include "matrix_ops.h"
#include <stdlib.h>

float **matmul(float **A, float **B, int A_rows, int A_cols, int B_rows, int B_cols) 
{
    // TODO: Implement matrix multiplication
       // Allocate output matrix [A_rows][B_cols]
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