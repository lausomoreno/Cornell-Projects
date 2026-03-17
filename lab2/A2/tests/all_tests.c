#include "unity/unity.h"
#include "test_conv.h"
#include "test_functional.h"
#include "test_linear.h"
#include "test_matrix_ops.h"
#include "test_attention.h"

void setUp(void) {
    /* Code here will run before each test */
}

void tearDown(void) {
    /* Code here will run after each test */
}

int main(void) {
    UNITY_BEGIN();

    // Test conv
   RUN_TEST(test_conv);
  //RUN_TEST(test_conv_im2col);



    // Test functional
    //RUN_TEST(test_softmax_basic);
    RUN_TEST(test_relu);

    // Test linear
   RUN_TEST(test_linear_basic);
   RUN_TEST(test_linear_basic2);

    // Test matrix_ops
    RUN_TEST(test_matmul_square_matrices);
    RUN_TEST(test_matmul_incompatible_dimensions);
    RUN_TEST(test_matmul_rectangular_matrices);
    RUN_TEST(test_matmul_quant);
    RUN_TEST(test_scaled_dot_product_attention);

    return UNITY_END();
}
