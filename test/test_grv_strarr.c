#include "grv/grv_str.h"
#include "grv/grv_strarr.h"
#include "grv/grv_test.h"
#include <stdlib.h>

GRV_TEST_BEGIN(grv_strarr_join)
  grv_strarr_t sa = grv_strarr_new();
  grv_strarr_push(&sa, grv_str_ref("Hello"));
  grv_strarr_push(&sa, grv_str_ref("World"));
  grv_str_t s = grv_strarr_join(sa, grv_str_ref(" "));
  GRV_TEST_EQUAL_STR(s, "Hello World");
GRV_TEST_END()

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  GRV_TEST_INIT(grv_strarr);
  GRV_TEST_RUN(grv_strarr_join);
}
