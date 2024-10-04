#include "grv/grv_test.h"
#include "grv/grv.h"

GRV_TEST_BEGIN_NEW(grv_str_read_from_file)
    grv_str_t s;
    grv_error_t err;
    bool success = grv_str_read_from_file(grv_str_ref("test/data/s18.txt"), &s, &err);
    GRV_TEST_TRUE(success);
    GRV_TEST_TRUE(err.success);
    GRV_TEST_EQUAL_INT(err.type, GRV_ERROR_OTHER);
    GRV_TEST_EQUAL_SIZE(s.size, 638);
GRV_TEST_END_NEW()
