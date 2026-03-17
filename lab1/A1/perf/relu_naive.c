#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kernel/functional.h"

// Generate random vector with positive and negative values
float *generate_random_vector(int size) {
    float *vec = (float *)malloc(size * sizeof(float));
    for (int i = 0; i < size; i++) {
        // Random values between -1.0 and +1.0
        vec[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    }
    return vec;
}

int main() {
    srand(time(NULL));

    int sizes[] = {1024, 164, 650};  
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int reps = 10;

    for (int s = 0; s < num_sizes; s++) {
        int N = sizes[s];
        printf("Profiling ReLU with vector size %d...\n", N);

        float *input = generate_random_vector(N);

        for (int r = 0; r < reps; r++) {
            // Apply ReLU in-place
            applyRelu(input, N);
        }

        free(input);
        printf("Completed profiling for ReLU size %d.\n", N);
    }

    return 0;
}
