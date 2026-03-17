#include "matrix_ops.h"





/*************** PART 1: DENSE -> CSR ***************/

/* Build CSR from a dense float** matrix (row-major). */
 CSRMatrix dense_to_csr(float **M, int rows, int cols)
{
    CSRMatrix csr;
    csr.rows = rows;
    csr.cols = cols;

    // count number of non zero values
    int nnz = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (M[i][j] != 0.0f)
                nnz++;
        }
    }
    csr.nnz = nnz;

    // allocate given the number of non zero values is known already
    csr.values = (float *)malloc(nnz * sizeof(float));
    csr.col_idx = (int   *)malloc(nnz * sizeof(int));
    csr.row_ptr = (int   *)malloc((rows + 1) * sizeof(int));

    
    int idx = 0;
    csr.row_ptr[0] = 0; // first element in row array has to be zero

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float v = M[i][j];
            if (v != 0.0f) {
                csr.values[idx] = v;
                csr.col_idx[idx] = j;
                idx++;
            }
        }
        csr.row_ptr[i + 1] = idx; // cumulative nnz up to end of row i
    }

    return csr;
}

 void free_csr(CSRMatrix *csr)
{
    if (!csr) return;
    free(csr->values);
    free(csr->col_idx);
    free(csr->row_ptr);
    csr->values = NULL;
    csr->col_idx = NULL;
    csr->row_ptr = NULL;
    csr->rows = csr->cols = csr->nnz = 0;
}

/*************** PART 2: CSR MATMUL ONLY ************/
/*
 * C += A * B  where A,B are in CSR.
 * C is dense [A_rows x B_cols], assumed allocated & zero-initialized.
 *
 * NOTE: This is the part you should time/loop over for the lab:
 * it touches ONLY the CSR structures, not the original dense matrices.
 */
 void csr_matmul_add(const CSRMatrix *A,
                           const CSRMatrix *B,
                           float **C)
{
    int A_rows = A->rows;
    int B_cols = B->cols;

    for (int i = 0; i < A_rows; i++) {

        int startA = A->row_ptr[i];
        int endA   = A->row_ptr[i + 1];

        // for each non-zero a(i,k)
        for (int p = startA; p < endA; p++) {
            int   k    = A->col_idx[p];
            float a_ik = A->values[p];

            // row k of B
            int startB = B->row_ptr[k];
            int endB   = B->row_ptr[k + 1];

            // for each non-zero b(k,j)
            for (int q = startB; q < endB; q++) {
                int   j    = B->col_idx[q];
                float b_kj = B->values[q];

                C[i][j] += a_ik * b_kj;
            }
        }
    }
}

float **matmul(float **A, float **B, int A_rows, int A_cols, int B_rows, int B_cols)
{
    // TODO: Implement matrix multiplication
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

float **matmul_sparse(float **A, float **B, int A_rows, int A_cols, int B_rows, int B_cols)
{
    // TODO: Implement sparse matrix multiplication

if (A_cols != B_rows)
        return NULL;

    // allocate output
    float **C = (float **)malloc(A_rows * sizeof(float *));
    if (!C) return NULL;

    for (int i = 0; i < A_rows; i++) {
        C[i] = (float *)malloc(B_cols * sizeof(float));
        if (!C[i]) {
            for (int r = 0; r < i; r++) free(C[r]);
            free(C);
            return NULL;
        }
        for (int j = 0; j < B_cols; j++)
            C[i][j] = 0.0f;
    }

    // Part 1: setup (CSR build)
    CSRMatrix Acsr = dense_to_csr(A, A_rows, A_cols);
    CSRMatrix Bcsr = dense_to_csr(B, B_rows, B_cols);

    // Part 2: actual sparse multiplication
    csr_matmul_add(&Acsr, &Bcsr, C);

    // cleanup
    free_csr(&Acsr);
    free_csr(&Bcsr);

    return C;
}
