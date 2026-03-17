#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#ifndef QUANT_PRECISION
#define QUANT_PRECISION 8
#endif

#ifndef INT_MIN_HELPERS_H
#define INT_MIN_HELPERS_H

static inline int imin(int a, int b) { return a < b ? a : b; }
static inline int imax(int a, int b) { return a > b ? a : b; }

#endif

#include <stdio.h>
#include <stdlib.h>

float **matmul(float **A, float **B, int A_rows, int A_cols, int B_rows, int B_cols);
float **matmul_blocking(float **A, float **B, int A_rows, int A_cols, int B_rows, int B_cols);
float **matmul_quant(unsigned int **A, int **B, int A_rows, int A_cols, int B_rows, int B_cols);

#endif /* MATRIX_OPS_H */