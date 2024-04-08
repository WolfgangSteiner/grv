#include "grv/grv_test.h"
#include "grv/grv_str.h"

GRV_TEST_BEGIN_NEW(grv_str_is_float)
    GRV_TEST_TRUE(grv_str_is_float(grv_str_ref("3.14")));
    GRV_TEST_TRUE(grv_str_is_float(grv_str_ref("-3.14")));
    GRV_TEST_TRUE(grv_str_is_float(grv_str_ref("-3.14e00")));
    GRV_TEST_TRUE(grv_str_is_float(grv_str_ref("-3.14e+00")));
    GRV_TEST_TRUE(grv_str_is_float(grv_str_ref("+3.14e2")));
    GRV_TEST_TRUE(grv_str_is_float(grv_str_ref("+3.14e-2")));
    GRV_TEST_FALSE(grv_str_is_float(grv_str_ref("+3.14e-2abc")));
    GRV_TEST_FALSE(grv_str_is_float(grv_str_ref("++3.14e-2")));
    GRV_TEST_FALSE(grv_str_is_float(grv_str_ref("+3.14e-2-2")));
GRV_TEST_END_NEW()
