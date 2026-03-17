#include "unity/unity.h"
#include "../kernel/kernel.h"
#include "test_linear.h"


void test_linear_basic(void)
{
    float input[] = {1.0, 2.0, 3.0};
    float *weights[] = {(float[]){1.0, 2.0, 3.0}, (float[]){4.0, 5.0, 6.0}};
    float biases[] = {0.1, 0.2};
    float *output = linear(input, weights, biases, 3, 2);
    TEST_ASSERT_EQUAL_FLOAT(14.1, output[0]);
    TEST_ASSERT_EQUAL_FLOAT(32.2, output[1]);

    // Cleanup
    free(output);
}

void test_linear_basic_2(void)
{
    float input[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    float *weights[] = {(float[]){1.0, 2.0, 3.0, 4.0, 5.0, 6.0}, (float[]){6.0, 5.0, 4.0, 3.0, 2.0, 1.0}};
    float biases[] = {1.0, 10.0};
    float *output = linear(input, weights, biases, 6, 2);
    TEST_ASSERT_EQUAL_FLOAT(92.0, output[0]);
    TEST_ASSERT_EQUAL_FLOAT(66.0, output[1]);

    // Cleanup
    free(output);
}

// Add more test cases as needed

void test_linear_basic_3(void)
{
    float input[] = {13.0, 46.0};
    float *weights[] = {(float[]){0.0, 0.0}, (float[]){0.0, 0.0}};
    float biases[] = {3.7, 5.8};
        float *output = linear(input, weights, biases, 2, 2);
    TEST_ASSERT_EQUAL_FLOAT(3.7, output[0]);
    TEST_ASSERT_EQUAL_FLOAT(5.8, output[1]);
    free(output);
}