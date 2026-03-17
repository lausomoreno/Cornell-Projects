#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kernel/linear.h"

float *generate_random_vector(int size) {
    float *vec = (float *)malloc(size * sizeof(float));
    for (int i = 0; i < size; i++) vec[i] = (float)rand() / RAND_MAX;
    return vec;
}

float **generate_random_weights(int outSize, int inSize) {
    float **W = (float **)malloc(outSize * sizeof(float *));
    for (int j = 0; j < outSize; j++) {
        W[j] = (float *)malloc(inSize * sizeof(float));
        for (int i = 0; i < inSize; i++) {
            W[j][i] = (float)rand() / RAND_MAX;
        }
    }
    return W;
}

void free_weights(float **W, int outSize) {
    for (int j = 0; j < outSize; j++) free(W[j]);
    free(W);
}

int main() {
    srand(time(NULL));
    int sizes[] = {128, 512, 2048};  // input sizes
    int num_sizes = sizeof(sizes)/sizeof(sizes[0]);
    int reps = 10;

    for (int s = 0; s < num_sizes; s++) {
        int inSize = sizes[s];
        int outSize = sizes[s] / 2;  // arbitrary scaling
        printf("Profiling linear layer: input %d, output %d...\n", inSize, outSize);

        float *input = generate_random_vector(inSize);
        float **weights = generate_random_weights(outSize, inSize);
        float *biases = generate_random_vector(outSize);

        for (int r = 0; r < reps; r++) {
            float *output = linear(input, weights, biases, inSize, outSize);
            free(output);
        }

        free(input);
        free_weights(weights, outSize);
        free(biases);
    }

    return 0;
}
