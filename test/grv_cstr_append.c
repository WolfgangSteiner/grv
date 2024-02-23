#include "grv/grv_test.h"

GRV_TEST_BEGIN_NEW(grv_cstr_append)
    char* str = grv_cstr_new("Hello ");
    str = grv_cstr_append(str, "World!");
    GRV_TEST_EQUAL_STR(grv_str_ref(str), "Hello World!");
GRV_TEST_END_NEW()
