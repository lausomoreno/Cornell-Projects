#include "attention.h"
#include <stdio.h>
#include <math.h>



static inline float **alloc_matrix(int rows, int cols) {
    float **M = (float **)malloc(rows * sizeof(float *));
    if (!M) return NULL;
    for (int i = 0; i < rows; ++i) {
        M[i] = (float *)malloc(cols * sizeof(float));
        if (!M[i]) { for (int r = 0; r < i; ++r) free(M[r]); free(M); return NULL; }
    }
    return M;
}
static inline void free_matrix(float **M, int rows) {
    if (!M) return;
    for (int i = 0; i < rows; ++i) free(M[i]);
    free(M);
}



static float **transpose(float **X, int rows, int cols) {
    float **XT = alloc_matrix(cols, rows);
    if (!XT) return NULL;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            XT[j][i] = X[i][j];
    return XT;
}
// Scaled dot-product attention
float **scaled_dot_product_attention(float **Q, float **K, float **V, int seqLength, int depth)
{
    // TODO: Implement the attention algorithm

   if (seqLength <= 0 || depth <= 0) return NULL;

    // 1) K^T
    float **KT = transpose(K, seqLength, depth);
    if (!KT) { fprintf(stderr, "transpose(K) failed\n"); return NULL; }

    // 2) scores = Q * K^T  -> (seqLength x seqLength)
    float **scores = matmul_blocking(Q, KT, seqLength, depth, depth, seqLength);
    if (!scores) {
        fprintf(stderr, "matmul(Q, K^T) failed\n");
        free_matrix(KT, depth);
        return NULL;
    }

    // 3) scale by 1/sqrt(depth)
    const float scale = 1.0f / sqrtf((float)depth);
    for (int i = 0; i < seqLength; ++i)
        for (int j = 0; j < seqLength; ++j)
            scores[i][j] *= scale;

    // 4) softmax row-wise (overwrite scores with probabilities)
    for (int i = 0; i < seqLength; ++i) {
        float *row_probs = softmax(scores[i], seqLength);
        if (!row_probs) {
            free_matrix(scores, seqLength);
            free_matrix(KT, depth);
            return NULL;
        }
        // copy back and free temp
        for (int j = 0; j < seqLength; ++j) scores[i][j] = row_probs[j];
        free(row_probs);
    }

    // 5) out = scores * V  -> (seqLength x depth)
    float **out = matmul_blocking(scores, V, seqLength, /*A_cols=*/seqLength,
                         /*B_rows=*/seqLength, /*B_cols=*/depth);
    if (!out) {
        fprintf(stderr, "matmul(scores, V) failed\n");
        free_matrix(scores, seqLength);
        free_matrix(KT, depth);
        return NULL;
    }

    // 6) cleanup
    free_matrix(scores, seqLength);
    free_matrix(KT, depth);

    return out;


    
}