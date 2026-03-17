#include "functional.h"

float relu(float x)
{
    // TODO: Implement relu
     // TODO: Implement relu
      if (x > 0.0f) {
        return  x;
    }
    else {
        return 0.0f;
    }
}

void applyRelu(float *input, int inputSize)
{
    for (int i = 0; i < inputSize; i++)
    {
        input[i] = relu(input[i]);
    }
}

float *softmax(float *input, int inputSize)
{
      // TODO: Implement softmax

    // Find maximum of input vector

    // Compute exp of input - maxInput to avoid underflow

    // Normalise and apply 
  
    float* output = (float*)malloc(inputSize * sizeof(float));

    // Step 1: find max input
    float maxInput = input[0];
    for (int i = 1; i < inputSize; i++) {
        if (input[i] > maxInput) {
            maxInput = input[i];
        }
    }

    // Step 2: compute sum of exp(input - maxInput)
    float sumExp = 0.0f;
    for (int i = 0; i < inputSize; i++) {
        sumExp += expf(input[i] - maxInput);
    }

    float logSumExp = logf(sumExp);

    // Step 3: changed my softmax functionto compute probabilities. 
    for (int i = 0; i < inputSize; i++) {
        output[i] = expf(input[i] - maxInput) /sumExp;
    }

    return output;
}