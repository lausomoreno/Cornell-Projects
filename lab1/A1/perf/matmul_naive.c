#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kernel/matrix_ops.h"

float **generate_random_matrix(int rows, int cols) {
    float **matrix = (float **)malloc(rows * sizeof(float *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (float *)malloc(cols * sizeof(float));
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX;
        }
    }
    return matrix;
}

void free_matrix(float **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    srand(time(NULL));
    int sizes[] = {1300, 700, 600};
     int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for(int i=0; i<num_sizes; i++){
        int A_rows = sizes[i], A_cols = sizes[i], B_rows = sizes[i], B_cols = sizes[i];
        if (A_cols != B_rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return 1;}

        float **A = generate_random_matrix(A_rows, A_cols);
        float **B = generate_random_matrix(B_rows, B_cols);

        for (int j=0; j<10; j++){
            printf("Performing naive multiplication with size %d...\n", sizes[i]);
            float **C = matmul(A, B, A_rows, A_cols, B_rows, B_cols);
            free_matrix(C, A_rows);
        }

        // Cleanup
        free_matrix(A, A_rows);
        free_matrix(B, B_rows);
                     
    }
    
   
   

    printf("Naive multiplications completed.\n");

    return 0;
}
