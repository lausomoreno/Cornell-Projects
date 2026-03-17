#include "unity/unity.h"
#include "../kernel/kernel.h"
#include "test_matrix_ops.h"


#define EPSILON 0.000001f

void assert_float_array_equal_matmul(float **expected, float **actual, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            UNITY_TEST_ASSERT_FLOAT_WITHIN(EPSILON, expected[i][j], actual[i][j], __LINE__, "Arrays Not Equal!");
        }
    }
}

void test_matmul_square_matrices(void)
{
    // Setup
    float **A = (float **)malloc(2 * sizeof(float *));
    for (int i = 0; i < 2; i++)
    {
        A[i] = (float *)malloc(2 * sizeof(float));
    }
    A[0][0] = 1.0f;
    A[0][1] = 2.0f;
    A[1][0] = 3.0f;
    A[1][1] = 4.0f;

    float **B = (float **)malloc(2 * sizeof(float *));
    for (int i = 0; i < 2; i++)
    {
        B[i] = (float *)malloc(2 * sizeof(float));
    }
    B[0][0] = 2.0f;
    B[0][1] = 0.0f;
    B[1][0] = 1.0f;
    B[1][1] = 2.0f;

    float **expected = (float **)malloc(2 * sizeof(float *));
    for (int i = 0; i < 2; i++)
    {
        expected[i] = (float *)malloc(2 * sizeof(float));
    }
    expected[0][0] = 4.0f;
    expected[0][1] = 4.0f;
    expected[1][0] = 10.0f;
    expected[1][1] = 8.0f;

    // Run function under test
    float **C = matmul_blocking(A, B, 2, 2, 2, 2);

    // Check expectations
    assert_float_array_equal_matmul(expected, C, 2, 2);

    // Cleanup
    for (int i = 0; i < 2; i++)
    {
        free(A[i]);
        free(B[i]);
        free(C[i]);
        free(expected[i]);
    }
    free(A);
    free(B);
    free(C);
    free(expected);
}

void test_matmul_incompatible_dimensions(void)
{
    // Setup
    float **A = (float **)malloc(2 * sizeof(float *));
    for (int i = 0; i < 2; i++)
    {
        A[i] = (float *)malloc(3 * sizeof(float));
    }

    float **B = (float **)malloc(2 * sizeof(float *));
    for (int i = 0; i < 2; i++)
    {
        B[i] = (float *)malloc(2 * sizeof(float));
    }

    // Run function under test
    float **C = matmul_blocking(A, B, 2, 3, 2, 2);

    // Check expectations
    UNITY_TEST_ASSERT_NULL(C, __LINE__, "Expected NULL!");

    // Cleanup
    for (int i = 0; i < 2; i++)
    {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);
}

void test_matmul_rectangular_matrices(void)
{
    // Setup
    float **A = (float **)malloc(2 * sizeof(float *));
    for (int i = 0; i < 2; i++)
    {
        A[i] = (float *)malloc(3 * sizeof(float));
    }
    A[0][0] = 1.0f;
    A[0][1] = 2.0f;
    A[0][2] = 3.0f;
    A[1][0] = 4.0f;
    A[1][1] = 5.0f;
    A[1][2] = 6.0f;

    float **B = (float **)malloc(3 * sizeof(float *));
    for (int i = 0; i < 3; i++)
    {
        B[i] = (float *)malloc(2 * sizeof(float));
    }
    B[0][0] = 7.0f;
    B[0][1] = 8.0f;
    B[1][0] = 9.0f;
    B[1][1] = 10.0f;
    B[2][0] = 11.0f;
    B[2][1] = 12.0f;

    float **expected = (float **)malloc(2 * sizeof(float *));
    for (int i = 0; i < 2; i++)
    {
        expected[i] = (float *)malloc(2 * sizeof(float));
    }
    expected[0][0] = 58.0f;
    expected[0][1] = 64.0f;
    expected[1][0] = 139.0f;
    expected[1][1] = 154.0f;

    // Run function under test
    float **C = matmul_blocking(A, B, 2, 3, 3, 2);

    // Check expectations
    assert_float_array_equal_matmul(expected, C, 2, 2);

    // Cleanup
   for (int i = 0; i < 2; i++) {
    free(A[i]);
    free(C[i]);
    free(expected[i]);
}
    for (int i = 0; i < 3; i++) {
    free(B[i]);
}
    free(A);
    free(B);
    free(C);
    free(expected);
}

void test_matmul_quant(void)
{
    // Setup
    int num_quant_element_per_word = sizeof(unsigned int) * 8 / QUANT_PRECISION;
    int A_rows = 2, A_cols = 2;
    int B_rows = num_quant_element_per_word * A_cols, B_cols = 1;

    unsigned int **A = (unsigned int **)malloc(A_rows * sizeof(unsigned int *));
    for (int i = 0; i < A_rows; i++)
    {
        A[i] = (unsigned int *)malloc(A_cols * sizeof(unsigned int));
    }
    A[0][0] = 0x01010101;
    A[0][1] = 0x01010101;
    A[1][0] = 0x02020202;
    A[1][1] = 0x02020202;

    int **B = (int **)malloc(B_rows * sizeof(int *));
    for (int i = 0; i < B_rows; i++)
    {
        B[i] = (int *)malloc(B_cols * sizeof(int));
    }
    for (int i = 0; i < B_rows; i++)
    {
        for (int j = 0; j < B_cols; j++)
        {
            B[i][j] = i;
        }
    }

    float **expected = (float **)malloc(A_rows * sizeof(float *));
    for (int i = 0; i < A_rows; i++)
    {
        expected[i] = (float *)malloc(B_cols * sizeof(float));
    }
    expected[0][0] = 28.0f;
    expected[1][0] = 56.0f;

    // Run function under test
    float **C = matmul_quant(A, B, A_rows, A_cols, B_rows, B_cols);

    // Check expectations
    assert_float_array_equal_matmul(expected, C, A_rows, B_cols);

    // Cleanup
    for (int i = 0; i < A_rows; i++)
    {
        free(A[i]);
        free(C[i]);
        free(expected[i]);
    }
    for (int i = 0; i < B_rows; i++)
    {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);
    free(expected);
}