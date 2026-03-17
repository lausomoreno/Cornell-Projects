#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kernel/conv.h"
#include "functional/conv.h"


float ***generate_random_image(int channels, int size) {
    float ***image = (float ***)malloc(channels * sizeof(float **));
    for (int c = 0; c < channels; c++) {
        image[c] = (float **)malloc(size * sizeof(float *));
        for (int i = 0; i < size; i++) {
            image[c][i] = (float *)malloc(size * sizeof(float));
            for (int j = 0; j < size; j++) {
                image[c][i][j] = (float)rand() / RAND_MAX;
            }
        }
    }
    return image;
}

float ****generate_random_kernel(int numFilters, int channels, int kSize) {
    float ****kernel = (float ****)malloc(numFilters * sizeof(float ***));
    for (int f = 0; f < numFilters; f++) {
        kernel[f] = (float ***)malloc(channels * sizeof(float **));
        for (int c = 0; c < channels; c++) {
            kernel[f][c] = (float **)malloc(kSize * sizeof(float *));
            for (int i = 0; i < kSize; i++) {
                kernel[f][c][i] = (float *)malloc(kSize * sizeof(float));
                for (int j = 0; j < kSize; j++) {
                    kernel[f][c][i][j] = (float)rand() / RAND_MAX;
                }
            }
        }
    }
    return kernel;
}

void free_image(float ***image, int channels, int size) {
    for (int c = 0; c < channels; c++) {
        for (int i = 0; i < size; i++) free(image[c][i]);
        free(image[c]);
    }
    free(image);
}

void free_kernel(float ****kernel, int numFilters, int channels, int kSize) {
    for (int f = 0; f < numFilters; f++) {
        for (int c = 0; c < channels; c++) {
            for (int i = 0; i < kSize; i++) free(kernel[f][c][i]);
            free(kernel[f][c]);
        }
        free(kernel[f]);
    }
    free(kernel);
}

void free_conv_output(float ***out, int numFilters, int size) {
    for (int f = 0; f < numFilters; f++) {
        for (int i = 0; i < size; i++) free(out[f][i]);
        free(out[f]);
    }
    free(out);
}

int main() {
    srand(time(NULL));
    int sizes[] = {16, 64, 12};   // image sizes
    int num_sizes = sizeof(sizes)/sizeof(sizes[0]);
    int reps = 10;

    int numChannels = 3;
    int numFilters = 8;
    int kernelSize = 3;

    for (int s = 0; s < num_sizes; s++) {
        int N = sizes[s];
        printf("Profiling convolution with image %dx%dx%d, kernel %dx%d...\n", numChannels, N, N, kernelSize, kernelSize);

        float ***image = generate_random_image(numChannels, N);
        float ****kernel = generate_random_kernel(numFilters, numChannels, kernelSize);
        float *biasData = (float *)malloc(numFilters * sizeof(float));
        for (int f = 0; f < numFilters; f++) biasData[f] = 0.1f;

        for (int r = 0; r < reps; r++) {
            float ***out = convolution(image, numChannels, kernel, biasData, numFilters, N, kernelSize);
            int outSize = N - kernelSize + 1;
            free_conv_output(out, numFilters, outSize);
        }

        free_image(image, numChannels, N);
        free_kernel(kernel, numFilters, numChannels, kernelSize);
        free(biasData);
    }

    return 0;
}
