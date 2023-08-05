#include "grv/test.h"
#include "grv/util.h"
#include <assert.h>
#include <stdlib.h>


GRV_TEST_BEGIN(grv_util_glob)
  grv_str_t pattern = grv_str_new("test/test_*.c");
  grv_strarr_t files = grv_util_glob(&pattern);
  GRV_TEST_ASSERT_TRUE(grv_strarr_size(&files) > 0);
  GRV_TEST_ASSERT_TRUE(grv_strarr_contains_cstr(&files, "test/test_grv_util.c"));
  GRV_TEST_ASSERT_TRUE(grv_strarr_contains_cstr(&files, "test/test_grv_str.c"));
  GRV_TEST_ASSERT_TRUE(grv_strarr_contains_cstr(&files, "test/test_grv_strarr.c"));
GRV_TEST_END()

size_t get_alloc_size(void* ptr) {
  #ifdef GRV_DEBUG_MEMORY
    return *((size_t*)ptr - 1);
  #else
    return 0;
  #endif
}

extern void grv_free_prepare(void*);

GRV_TEST_BEGIN(grv_alloc_realloc_free)
  void* ptr = grv_alloc(10);
  GRV_TEST_ASSERT_EQUAL(*((size_t*)ptr - 1), 24);
  ptr = grv_realloc(ptr, 16);
  GRV_TEST_ASSERT_EQUAL(*((size_t*)ptr - 1), 24);
  ptr = grv_realloc(ptr, 20);
  GRV_TEST_ASSERT_EQUAL(*((size_t*)ptr - 1), 32);
  size_t alloc_size = get_alloc_size(ptr);
  u8* byte_ptr = (u8*)ptr;
  for (size_t i = 0; i < alloc_size - sizeof(size_t); ++i) {
    GRV_TEST_ASSERT_EQUAL_HEX(byte_ptr[i], 0xef);
  }
  grv_free_prepare(ptr);
  byte_ptr -= sizeof(size_t);
  for (size_t i = 0; i < alloc_size; ++i) {
    GRV_TEST_ASSERT_EQUAL_HEX(byte_ptr[i], 0xcd);
  }
  free(ptr - sizeof(size_t));  
GRV_TEST_END()

int main(void) {
  GRV_TEST_INIT(grv_util);
  GRV_TEST_RUN(grv_util_glob);
  GRV_TEST_RUN(grv_alloc_realloc_free);
}