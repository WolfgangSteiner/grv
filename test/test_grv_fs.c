#include "grv/grv_test.h"
#include "grv/grv_fs.h"

GRV_TEST_BEGIN(grv_fs_basename)
  grv_str_t path = grv_str_ref("test/test_grv_fs.c");
  grv_str_t basename = grv_fs_basename(path);
  GRV_TEST_EQUAL_STR(basename, "test_grv_fs.c");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_fs_stem)
  grv_str_t path = grv_str_ref("test/test_grv_fs.c");
  grv_str_t stem = grv_fs_stem(path);
  GRV_TEST_EQUAL_STR(stem, "test/test_grv_fs");

  path = grv_str_ref("test/my.funny.directory/test_grv_fs.c.tmp");
  stem = grv_fs_stem(path);
  GRV_TEST_EQUAL_STR(stem, "test/my.funny.directory/test_grv_fs.c");
GRV_TEST_END()

int main(void) {
  GRV_TEST_INIT(grv_fs);
  GRV_TEST_RUN(grv_fs_basename)
  GRV_TEST_RUN(grv_fs_stem)
}