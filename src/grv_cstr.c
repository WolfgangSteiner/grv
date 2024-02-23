#include "grv/grv_cstr.h"
#include "grv/grv_memory.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#define GRV_CSTR_ALLOC_GRANULARITY 16

static grv_cstr_size_t _cstr_compute_capacity(grv_cstr_size_t len) {
  grv_cstr_size_t new_capacity = len + 1;
  if (new_capacity % GRV_CSTR_ALLOC_GRANULARITY == 0) {
    return new_capacity + GRV_CSTR_ALLOC_GRANULARITY;
  } else {
    return (new_capacity / GRV_CSTR_ALLOC_GRANULARITY + 1) * GRV_CSTR_ALLOC_GRANULARITY;
  }
}

char* grv_cstr_alloc(grv_cstr_size_t len) {
  grv_cstr_size_t new_capacity = _cstr_compute_capacity(len);
  char* new_data = grv_alloc(new_capacity);
  assert(new_data != NULL);
  new_data[0] = '\0';
  return new_data;
}

char* grv_cstr_new(const char* src) {
  grv_cstr_size_t len = grv_cstr_len(src);
  char* res = grv_cstr_alloc(len);
  memcpy(res, src, len + 1);
  return res;
}

void grv_cstr_free(char* str) {
  grv_free(str);
}

char* cstr_grow(char* str, grv_cstr_size_t new_len) {
  grv_cstr_size_t current_len = strlen(str);
  grv_cstr_size_t current_capacity = _cstr_compute_capacity(current_len);
  grv_cstr_size_t new_capacity = _cstr_compute_capacity(new_len);
  if (new_capacity > current_capacity) {
    char* new_data = grv_realloc(str, new_capacity);
    assert(new_data != NULL);
    return new_data;
  } else {
    return str;
  }
}

char* grv_cstr_cat(const char* a, const char* b) {
  grv_cstr_size_t a_len = grv_cstr_len(a);
  grv_cstr_size_t b_len = grv_cstr_len(b);
  grv_cstr_size_t new_len = a_len + b_len;
  char* new_data = grv_cstr_alloc(new_len);
  memcpy(new_data, a, a_len);
  memcpy(new_data + a_len, b, b_len);
  return new_data;
}

char* grv_cstr_append(char* str, const char* append_str) {
  grv_cstr_size_t str_len = grv_cstr_len(str);
  grv_cstr_size_t append_str_len = grv_cstr_len(append_str);
  grv_cstr_size_t new_len = str_len + append_str_len;
  str = cstr_grow(str, new_len);
  memcpy(str + str_len, append_str, append_str_len + 1);
  return str;
}

grv_cstr_size_t grv_cstr_len(const char* s) {
  grv_cstr_size_t len = 0;
  grv_cstr_size_t max_len = 0x7fffffff;
  while (len < max_len && s[len] != '\0') len++;
  return len;
}

char* grv_cstr_new_with_format(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  grv_cstr_size_t len = vsnprintf(NULL, 0, fmt, args);
  va_end(args);
  char* str = grv_cstr_alloc(len);
  va_start(args, fmt);
  vsnprintf(str, len + 1, fmt, args);
  va_end(args);
  return str;
}

char* grv_cstr_repeat_char(char c, grv_cstr_size_t count) {
  char* cstr = grv_cstr_alloc(count);
  memset(cstr, c, count);
  cstr[count] = '\0';
  return cstr;
}

bool grv_cstr_eq(char* a, char* b) {
    while (*a != 0 && *b != 0 && *a++ == *b++);
    return *a == 0 && *b == 0;
}

bool grv_cstr_contains(char* str, char* substr) {
    char* p = strstr(str, substr);
    return p != NULL;
}
