#include "linear.h"

float *linear(float *input, float **weights, float *biases, int inputSize, int outputSize)
{
    
     // TODO: Implement the linear function (fully connected layer)
    float* output = (float*)malloc(outputSize * sizeof(float));

    for (int j = 0; j < outputSize; j++) {
        float sum = biases[j];  // start with bias
        for (int i = 0; i < inputSize; i++) {
            sum += weights[j][i] * input[i];
        }
        output[j] = sum;
    }

    return output;
}
