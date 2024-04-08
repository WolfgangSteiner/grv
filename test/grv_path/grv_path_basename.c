#include "grv/grv_test.h"
#include "grv/grv_path.h"

GRV_TEST_BEGIN_NEW(grv_path_basename)
    grv_str_t path = grv_str_ref("test/test_grv_fs.c");
    grv_str_t basename = grv_path_basename(path);
    GRV_TEST_EQUAL_STR(basename, "test_grv_fs.c");
GRV_TEST_END_NEW()
