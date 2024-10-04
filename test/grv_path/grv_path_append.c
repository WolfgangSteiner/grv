#include "grv/grv_test.h"
#include "grv/grv.h"

GRV_TEST_BEGIN_NEW(grv_path_append)
    grv_str_t p = grv_str_ref("");
    grv_path_append(&p, grv_str_ref(""));
    GRV_TEST_EQUAL_STR(p, "");

    grv_path_append(&p, grv_str_ref("test"));
    GRV_TEST_EQUAL_STR(p, "test");

    p = grv_str_ref("/");
    grv_path_append(&p, grv_str_ref("usr"));
    GRV_TEST_EQUAL_STR(p, "/usr");
    grv_path_append(&p, grv_str_ref("bin"));
    GRV_TEST_EQUAL_STR(p, "/usr/bin");

    grv_path_append(&p, grv_str_ref(""));
    GRV_TEST_EQUAL_STR(p, "/usr/bin");

    grv_path_append(&p, grv_str_ref("sub/"));
    GRV_TEST_EQUAL_STR(p, "/usr/bin/sub/");
    
    grv_path_append(&p, grv_str_ref("sub2"));
    GRV_TEST_EQUAL_STR(p, "usr/bin/sub/sub2");

GRV_TEST_END_NEW()
