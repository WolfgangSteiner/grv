#include "grv/grv_str.h"
#include "grv/grv_test.h"
#include <stdlib.h>

GRV_TEST_BEGIN(grv_strarr_join)
  grv_strarr_t sa = grv_strarr_new();
  grv_strarr_push_cstr(&sa, "Hello");
  grv_strarr_push_cstr(&sa, "World");
  grv_str_t s = grv_strarr_join(&sa, " ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&s), "Hello World");
GRV_TEST_END()


int main(int argc, char **argv) {
  GRV_TEST_INIT(grv_strarr);
  GRV_TEST_RUN(grv_strarr_join);
}