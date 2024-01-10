#include "grv/grv_test.h"
#include "grv/vec2f.h"
#include <math.h>

GRV_TEST_BEGIN(vec2f_is_zero)
    vec2f zero = {0};
    GRV_TEST_TRUE(vec2f_is_zero(zero));
GRV_TEST_END()

GRV_TEST_BEGIN(vec2f_abs)
    vec2f v1 = {1.0f, 1.0f};
    vec2f v1_abs = {1.0f, 1.0f};
    GRV_TEST_TRUE(vec2f_eq(v1, v1_abs));
GRV_TEST_END()

int main(void) {
    GRV_TEST_INIT(vec2f);
    GRV_TEST_RUN(vec2f_is_zero);
    GRV_TEST_RUN(vec2f_abs);
}