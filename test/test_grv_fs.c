#include "grv/test.h"
#include "grv/fs.h"

GRV_TEST_BEGIN(grv_fs_basename)
  grv_str_t path = grv_str_new("test/test_grv_fs.c");
  grv_str_t basename = grv_fs_basename(&path);
  GRV_TEST_ASSERT_TRUE(grv_str_eq_cstr(&basename, "test_grv_fs.c"));
GRV_TEST_END()

GRV_TEST_BEGIN(grv_fs_stem)
  grv_str_t path = grv_str_new("test/test_grv_fs.c");
  grv_str_t stem = grv_fs_stem(&path);
  GRV_TEST_ASSERT_TRUE(grv_str_eq_cstr(&stem, "test/test_grv_fs"));

  path = grv_str_new("test/my.funny.directory/test_grv_fs.c.tmp");
  stem = grv_fs_stem(&path);
  GRV_TEST_ASSERT_TRUE(grv_str_eq_cstr(&stem, "test/my.funny.directory/test_grv_fs"));
GRV_TEST_END()

int main(void) {
  GRV_TEST_INIT(grv_fs);
  GRV_TEST_RUN(grv_fs_basename)
  GRV_TEST_RUN(grv_fs_stem)
}