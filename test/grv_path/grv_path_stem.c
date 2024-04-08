#include "grv/grv_test.h"
#include "grv/grv_path.h"

GRV_TEST_BEGIN_NEW(grv_fs_stem)
    grv_str_t path = grv_str_ref("test/test_grv_fs.c");
    grv_str_t stem = grv_path_stem(path);
    GRV_TEST_EQUAL_STR(stem, "test/test_grv_fs");

    path = grv_str_ref("test/my.funny.directory/test_grv_fs.c.tmp");
    stem = grv_path_stem(path);
    GRV_TEST_EQUAL_STR(stem, "test/my.funny.directory/test_grv_fs.c");
GRV_TEST_END_NEW()
