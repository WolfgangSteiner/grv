#include "grv/grv_test.h"
#include "grv/grv_path.h"

GRV_TEST_BEGIN_NEW(grv_path_expand_tilde)
    grv_str_t path = grv_str_ref("~/some/path");
    grv_str_t home_path = grv_str_ref(getenv("HOME"));
    GRV_TEST_EQUAL_STR(grv_path_expand_tilde(path), grv_str_cat(home_path, grv_str_ref("/some/path")));
GRV_TEST_END_NEW()
