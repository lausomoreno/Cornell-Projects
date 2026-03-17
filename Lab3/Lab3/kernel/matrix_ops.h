#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows;      // number of rows
    int cols;      // number of columns
    int nnz;       // number of non-zero entries
    float *values; // length nnz
    int   *col_idx;// length nnz
    int   *row_ptr;// length rows + 1
} CSRMatrix;

/*
 * Build CSR from a dense matrix A[rows][cols].
 * Assumes A is given as float** with each row allocated.
 */
CSRMatrix dense_to_csr(float **A, int rows, int cols);
void csr_matmul_add(const CSRMatrix *A, const CSRMatrix *B,float **C);
void free_csr(CSRMatrix *csr);
float **matmul(float **A, float **B, int A_rows, int A_cols, int B_rows, int B_cols);
float **matmul_sparse(float **A, float **B, int A_rows, int A_cols, int B_rows, int B_cols);




#endif /* MATRIX_OPS_H */