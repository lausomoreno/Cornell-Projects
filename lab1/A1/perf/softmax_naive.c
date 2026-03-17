#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kernel/functional.h"

// Generate random vector (positive values for stability)
float *generate_random_vector(int size) {
    float *vec = (float *)malloc(size * sizeof(float));
    for (int i = 0; i < size; i++) {
        vec[i] = (float)rand() / RAND_MAX;  // [0,1]
    }
    return vec;
}

int main() {
    srand(time(NULL));

    int sizes[] = {128, 1024, 8192};  // test vectors for softmax
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int reps = 10;

    for (int s = 0; s < num_sizes; s++) {
        int N = sizes[s];
        printf("Profiling Softmax with vector size %d...\n", N);

        float *input = generate_random_vector(N);

        for (int r = 0; r < reps; r++) {
            float *output = softmax(input, N);
            free(output);
        }

        free(input);
        printf("Completed profiling for Softmax size %d.\n", N);
    }

    return 0;
}
