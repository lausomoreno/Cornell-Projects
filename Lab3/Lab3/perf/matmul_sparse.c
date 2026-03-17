// perf/matmul_sparse.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kernel/matrix_ops.h"   // must declare CSRMatrix, dense_to_csr, csr_matmul_add

static float **generate_random_matrix(int rows, int cols, float zero_prob)
{
    float **matrix = (float **)malloc(rows * sizeof(float *));
    if (!matrix) return NULL;

    for (int i = 0; i < rows; i++) {
        matrix[i] = (float *)malloc(cols * sizeof(float));
        if (!matrix[i]) {
            for (int k = 0; k < i; k++) free(matrix[k]);
            free(matrix);
            return NULL;
        }

        for (int j = 0; j < cols; j++) {
            float r = (float)rand() / (float)RAND_MAX;
            if (r < zero_prob) {
                matrix[i][j] = 0.0f;                // sparse
            } else {
                matrix[i][j] = (float)rand() / RAND_MAX;
            }
        }
    }

    return matrix;
}

static void free_matrix(float **matrix, int rows)
{
    if (!matrix) return;
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

static void zero_matrix(float **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i][j] = 0.0f;
}

int main(void)
{
    srand((unsigned int)time(NULL));

    // Match naive perf style: biggish square matrices
    int A_rows = 2048, A_cols = 2048;
    int B_rows = 2048, B_cols = 2048;

    if (A_cols != B_rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }

    // Choose sparsity level (tweak as needed)
    // e.g. 90% zeros -> good to see sparse benefit
    float zero_prob = 0.5f;

    // 1) Generate sparse-ish dense matrices
    float **A = generate_random_matrix(A_rows, A_cols, zero_prob);
    float **B = generate_random_matrix(B_rows, B_cols, zero_prob);
    if (!A || !B) {
        printf("Allocation failed for input matrices.\n");
        free_matrix(A, A_rows);
        free_matrix(B, B_rows);
        return 1;
    }

    // 2) Part 1: build CSR once (SETUP, not inside the timed loop)
    CSRMatrix Acsr = dense_to_csr(A, A_rows, A_cols);
    CSRMatrix Bcsr = dense_to_csr(B, B_rows, B_cols);

    // 3) Allocate output matrix once
    float **C = (float **)malloc(A_rows * sizeof(float *));
    if (!C) {
        printf("Allocation failed for C.\n");
        free_matrix(A, A_rows);
        free_matrix(B, B_rows);
        free_csr(&Acsr);
        free_csr(&Bcsr);
        return 1;
    }
    for (int i = 0; i < A_rows; i++) {
        C[i] = (float *)malloc(B_cols * sizeof(float));
        if (!C[i]) {
            for (int k = 0; k < i; k++) free(C[k]);
            free(C);
            free_matrix(A, A_rows);
            free_matrix(B, B_rows);
            free_csr(&Acsr);
            free_csr(&Bcsr);
            return 1;
        }
    }

    // 4) Part 2: loop only over the sparse kernel
    //    This is what you profile with `time` / `toplev`.
    int iters = 50;  // or more, depending on how long you want it to run

    printf("Performing matmul_sparse CSR kernel, iters=%d, sparsity≈%.0f%% zeros...\n",
           iters, zero_prob * 100.0f);

    for (int t = 0; t < iters; t++) {
        zero_matrix(C, A_rows, B_cols);
        csr_matmul_add(&Acsr, &Bcsr, C);
    }

    // Touch one element so compiler can't optimize the loop away
    printf("Done. Sample C[0][0] = %f\n", C[0][0]);

    // 5) Cleanup
    free_matrix(A, A_rows);
    free_matrix(B, B_rows);
    for (int i = 0; i < A_rows; i++)
        free(C[i]);
    free(C);
    free_csr(&Acsr);
    free_csr(&Bcsr);

    return 0;
}
