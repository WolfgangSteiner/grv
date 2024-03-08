#include "grv/grv_test.h"
#include "grv/grv_common.h"

GRV_TEST_BEGIN(grv_common_min_max)
    GRV_TEST_EQUAL_INT(grv_min_i32(1, 2), 1);
    GRV_TEST_EQUAL_INT(grv_min_i32(2, 1), 1);
    GRV_TEST_EQUAL_INT(grv_max_i32(1, 2), 2);
    GRV_TEST_EQUAL_INT(grv_max_i32(2, 1), 2);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_common_round)
    GRV_TEST_EQUAL_INT(grv_round_f32(1.0f), 1);
    GRV_TEST_EQUAL_INT(grv_round_f32(1.5f), 2);
    GRV_TEST_EQUAL_INT(grv_round_f32(-1.1f), -1.0f);
    GRV_TEST_EQUAL_INT(grv_round_f32(-1.6f), -2.0f);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_common_swp)
    i32 a = 1;
    i32 b = 2;
    grv_swp_i32(&a, &b);
    GRV_TEST_EQUAL_INT(a, 2);
    GRV_TEST_EQUAL_INT(b, 1);

    f32 c = 1.0f;
    f32 d = 2.0f;
    grv_swp_f32(&c, &d);
    GRV_TEST_EQUAL_INT(c, 2.0f);
    GRV_TEST_EQUAL_INT(d, 1.0f);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_common_abs)
    GRV_TEST_EQUAL_INT(grv_abs_f32(1.0f), 1.0f);
    GRV_TEST_EQUAL_INT(grv_abs_f32(-1.0f), 1.0f);
    GRV_TEST_EQUAL_INT(grv_abs_f32(0.0f), 0.0f);
    GRV_TEST_EQUAL_INT(grv_abs_i32(-10), 10);
    GRV_TEST_EQUAL_INT(grv_abs_i32(10), 10);
    GRV_TEST_EQUAL_INT(grv_abs_i32(0), 0);
GRV_TEST_END()

int main(void) {
    GRV_TEST_INIT(grv_common);
    GRV_TEST_RUN(grv_common_min_max);
    GRV_TEST_RUN(grv_common_round);
    GRV_TEST_RUN(grv_common_swp);
    GRV_TEST_RUN(grv_common_abs);
    GRV_TEST_FINISH();
}
