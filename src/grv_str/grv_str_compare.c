#include "grv/grv_str.h"

#ifndef GRV_TEST_COMPILE
#include "grv/grv_common.h"
#include "libc/memcmp.h"

int grv_str_compare(grv_str_t a, grv_str_t b) {
    int res = memcmp(a.data, b.data, GRV_MIN(a.size, b.size));
    return res == 0 ? GRV_COMPARE(a.size, b.size) : res;
}

#else
#include "grv/grv_test.h"

GRV_TEST_BEGIN_NEW(grv_str_compare)
    grv_str_t a = grv_str_ref("aaa");
    grv_str_t b = grv_str_ref("bb");
    GRV_TEST_EQUAL_INT(grv_str_compare(a, b), -1);
    a = grv_str_ref("aaad");
    b = grv_str_ref("aaac");
    GRV_TEST_EQUAL_INT(grv_str_compare(a, b), 1);
    a = grv_str_ref("aaa");
    b = grv_str_ref("aaac");
    GRV_TEST_EQUAL_INT(grv_str_compare(a, b), -1);
    a = grv_str_ref("aaa");
    b = grv_str_ref("aaa");
    GRV_TEST_EQUAL_INT(grv_str_compare(a, b), 0);
    a = grv_str_ref("");
    b = grv_str_ref("aaa");
    GRV_TEST_EQUAL_INT(grv_str_compare(a, b), -1);
    a = grv_str_ref("");
    b = grv_str_ref("");
    GRV_TEST_EQUAL_INT(grv_str_compare(a, b), 0);
GRV_TEST_END_NEW()

#endif
