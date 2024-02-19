#include "grv/grv_test.h"
#include "grv/grv_str.h"

GRV_TEST_BEGIN_NEW(grv_str_to_f32)
    GRV_TEST_EQUAL_F32(grv_str_to_f32(grv_str_ref("3.14")), 3.14f);
    GRV_TEST_EQUAL_F32(grv_str_to_f32(grv_str_ref("-3.14")), -3.14f);
    GRV_TEST_EQUAL_F32(grv_str_to_f32(grv_str_ref("-3.14e00")), -3.14f);
    GRV_TEST_EQUAL_F32(grv_str_to_f32(grv_str_ref("-3.14e+00")), -3.14f);
    GRV_TEST_EQUAL_F32(grv_str_to_f32(grv_str_ref("+3.14e2")), 314.0f);
    GRV_TEST_EQUAL_F32(grv_str_to_f32(grv_str_ref("+3.14e-2")), 0.0314f);
    GRV_TEST_EQUAL_F32(grv_str_to_f32(grv_str_ref("+3.14e-2abc")), 0.0314f);
GRV_TEST_END_NEW()
