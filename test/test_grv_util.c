#include "grv_test.h"
#include "grv_util.h"

GRV_TEST_BEGIN(grv_util_glob)
  grv_str pattern = grv_str_new("test/test_*.c");
  grv_strarr files = grv_util_glob(&pattern);
  GRV_TEST_ASSERT_TRUE(grv_strarr_size(&files) > 0);
  GRV_TEST_ASSERT_TRUE(grv_strarr_contains_cstr(&files, "test/test_grv_util.c"));
  GRV_TEST_ASSERT_TRUE(grv_strarr_contains_cstr(&files, "test/test_grv_str.c"));
  GRV_TEST_ASSERT_TRUE(grv_strarr_contains_cstr(&files, "test/test_grv_strarr.c"));
GRV_TEST_END()

int main(void) {
  GRV_TEST_INIT(grv_util);
  GRV_TEST_RUN(grv_util_glob)
}