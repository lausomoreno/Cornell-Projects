#include "conv.h"

// Im2col algorithm
float **im2col(float ***image, int numChannels, int imageSize, int kernelSize, int stride, int *outputSize)
{
   
    (void)numChannels; // avoid unused warning if they always pass 1

    int outSize = (imageSize - kernelSize) / stride + 1;
    if (outSize <= 0) {
        if (outputSize) *outputSize = 0;
        return NULL;
    }

    int patchSize  = kernelSize * kernelSize;   // rows
    int numPatches = outSize * outSize;         // cols

    // Allocate [patchSize][numPatches]
    float **cols = (float **)malloc(patchSize * sizeof(float *));
    if (!cols) {
        if (outputSize) *outputSize = 0;
        return NULL;
    }

    for (int r = 0; r < patchSize; r++) {
        cols[r] = (float *)malloc(numPatches * sizeof(float));
        if (!cols[r]) {
            for (int i = 0; i < r; i++) free(cols[i]);
            free(cols);
            if (outputSize) *outputSize = 0;
            return NULL;
        }
    }

    // Fill: each column corresponds to one KxK patch,
    // row index matches kernel_flatten: i * kernelSize + j
    for (int out_y = 0; out_y < outSize; out_y++) {
        for (int out_x = 0; out_x < outSize; out_x++) {

            int col_idx = out_y * outSize + out_x;
            int in_y0   = out_y * stride;
            int in_x0   = out_x * stride;

            for (int i = 0; i < kernelSize; i++) {
                for (int j = 0; j < kernelSize; j++) {

                    int row_idx = i * kernelSize + j;
                    int in_y    = in_y0 + i;
                    int in_x    = in_x0 + j;

                    cols[row_idx][col_idx] = image[0][in_y][in_x];
                }
            }
        }
    }

    if (outputSize)
        *outputSize = outSize;

    return cols;
}

// Im2col algorithm's inverse
float ***col2im(float **result, int num_kernels, int conv_rows, int conv_cols)
{
    // TODO: Implement the col2im algorithm
    float ***out = (float ***)malloc(num_kernels * sizeof(float **));
    if (!out) return NULL;

    for (int k = 0; k < num_kernels; k++) {
        out[k] = (float **)malloc(conv_rows * sizeof(float *));
        if (!out[k]) {
            // cleanup
            for (int kk = 0; kk < k; kk++) {
                for (int i = 0; i < conv_rows; i++)
                    free(out[kk][i]);
                free(out[kk]);
            }
            free(out);
            return NULL;
        }

        for (int i = 0; i < conv_rows; i++) {
            out[k][i] = (float *)malloc(conv_cols * sizeof(float));
            if (!out[k][i]) {
                // cleanup
                for (int ii = 0; ii <= i; ii++)
                    if (out[k][ii]) free(out[k][ii]);
                free(out[k]);
                for (int kk = 0; kk < k; kk++) {
                    for (int r = 0; r < conv_rows; r++)
                        free(out[kk][r]);
                    free(out[kk]);
                }
                free(out);
                return NULL;
            }
        }
    }

    // Fill from result[k][flat_index]
    for (int k = 0; k < num_kernels; k++) {
        for (int i = 0; i < conv_rows; i++) {
            for (int j = 0; j < conv_cols; j++) {
                int col_idx = i * conv_cols + j;
                out[k][i][j] = result[k][col_idx];
            }
        }
    }

    return out;
}

float **kernel_flatten(float ****kernel, int num_kernels, int kernel_size)
{
    float **flattened_kernels = (float **)malloc(num_kernels * sizeof(float *));
    for (int i = 0; i < num_kernels; i++)
    {
        flattened_kernels[i] = (float *)malloc(kernel_size * kernel_size * sizeof(float));
    }

    for (int k = 0; k < num_kernels; k++)
    {
        for (int i = 0; i < kernel_size; i++)
        {
            for (int j = 0; j < kernel_size; j++)
            {
                flattened_kernels[k][i * kernel_size + j] = kernel[k][0][i][j];
            }
        }
    }

    return flattened_kernels;
}

// Basic convolution operation
float ***convolution(float ***image, int numChannels, float ****kernel, float *biasData, int numFilters, int inputSize, int kernelSize)
{
    // TODO: Implement the basic convolution operation

      int outputSize = inputSize - kernelSize + 1;

    // Allocate output
    float ***output = (float ***)malloc(numFilters * sizeof(float **));
    for (int f = 0; f < numFilters; f++) {
        output[f] = (float **)malloc(outputSize * sizeof(float *));
        for (int i = 0; i < outputSize; i++) {
            output[f][i] = (float *)malloc(outputSize * sizeof(float));
        }
    }

    // Convolution
    for (int f = 0; f < numFilters; f++) {
        for (int i = 0; i < outputSize; i++) {
            for (int j = 0; j < outputSize; j++) {
                float sum = 0.0f;

                // Convolution sum over channels and kernel
                for (int c = 0; c < numChannels; c++) {
                    for (int m = 0; m < kernelSize; m++) {
                        for (int n = 0; n < kernelSize; n++) {
                            sum += image[c][i + m][j + n] * kernel[f][c][m][n];
                        }
                    }
                }

                // Add bias (no ReLU here yet)
                output[f][i][j] = sum + biasData[f];
            }

            // Apply ReLU to this row
            applyRelu(output[f][i], outputSize);
        }
    }

    return output;
}


float ***convolution_im2col(float ***image, int numChannels, float ****kernel, float *biasData, int numFilters, int inputSize, int kernelSize, MatmulType matmul_type)
{
    (void)numChannels;   // current im2col/kernel_flatten use image[0] / channel 0

    int stride = 1;
    int outSize = 0;

    // 1) im2col on input: [K*K][outSize*outSize]
    float **cols = im2col(image, numChannels,
                          inputSize, kernelSize,
                          stride, &outSize);
    if (!cols)
        return NULL;

    int patchSize  = kernelSize * kernelSize;
    int numPatches = outSize * outSize;

    // 2) Flatten kernels: [numFilters][K*K]
    float **k_flat = kernel_flatten(kernel, numFilters, kernelSize);
    if (!k_flat) {
        for (int r = 0; r < patchSize; r++)
            free(cols[r]);
        free(cols);
        return NULL;
    }

    // 3) Matmul: choose BASE (dense) or SPARSE
    float **conv_cols = NULL;

    switch (matmul_type) {
        case MATMUL_BASE:
            conv_cols = matmul(k_flat, cols,
                               numFilters, patchSize,
                               patchSize, numPatches);
            break;

        case MATMUL_SPARSE:
            conv_cols = matmul_sparse(k_flat, cols,
                                      numFilters, patchSize,
                                      patchSize, numPatches);
            break;

        default:
            // Fallback to BASE to be safe
            conv_cols = matmul(k_flat, cols,
                               numFilters, patchSize,
                               patchSize, numPatches);
            break;
    }

    if (!conv_cols) {
        for (int f = 0; f < numFilters; f++)
            free(k_flat[f]);
        free(k_flat);

        for (int r = 0; r < patchSize; r++)
            free(cols[r]);
        free(cols);

        return NULL;
    }

    // 4) col2im: [numFilters][numPatches] -> [numFilters][outSize][outSize]
    float ***output = col2im(conv_cols, numFilters, outSize, outSize);
    if (!output) {
        for (int f = 0; f < numFilters; f++)
            free(k_flat[f]);
        free(k_flat);

        for (int r = 0; r < patchSize; r++)
            free(cols[r]);
        free(cols);

        for (int f = 0; f < numFilters; f++)
            free(conv_cols[f]);
        free(conv_cols);

        return NULL;
    }

    // 5) Add bias + ReLU (same style as your direct convolution)
    for (int f = 0; f < numFilters; f++) {
        float b = biasData ? biasData[f] : 0.0f;

        for (int i = 0; i < outSize; i++) {
            for (int j = 0; j < outSize; j++) {
                output[f][i][j] += b;
            }
            applyRelu(output[f][i], outSize);
        }
    }

    // 6) Cleanup temporaries; keep only output
    for (int f = 0; f < numFilters; f++)
        free(k_flat[f]);
    free(k_flat);

    for (int r = 0; r < patchSize; r++)
        free(cols[r]);
    free(cols);

    for (int f = 0; f < numFilters; f++)
        free(conv_cols[f]);
    free(conv_cols);

    return output;
}