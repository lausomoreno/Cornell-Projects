#include "conv.h"

// Basic convolution operation
float*** convolution(float*** image, int numChannels, float**** kernel, float* biasData, int numFilters, int inputSize, int kernelSize)

    // TODO: Implement the convolution operation
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

