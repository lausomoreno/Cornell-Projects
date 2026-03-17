#include "conv.h"

// Im2col algorithm
float **im2col(float ***image, int numChannels, int imageSize, int kernelSize, int stride, int *outputSize)
{
    // TODO: Implement the im2col algorithm
}

// Im2col algorithm's inverse
float ***col2im(float **result, int num_kernels, int conv_rows, int conv_cols)
{
    // TODO: Implement the col2im algorithm
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

// Convolution with im2col algorithm
float ***convolution_im2col(float ***image, int numChannels, float ****kernel, float *biasData, int numFilters, int inputSize, int kernelSize, MatmulType matmul_type)
{
    // TODO: Implement the convolution operation with im2col algorithm
    // Flatten kernel

    // Apply im2col

    // Apply matmul

    // Apply col2im

    // Add bias and apply ReLU

    // Cleanup
}