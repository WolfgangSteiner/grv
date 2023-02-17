#include "grv_defines.h"
#include "grv_str.h"
#include <string.h>
#include <stdlib.h>

static inline u64 grv_str_compute_capacity(u64 length) {
    size_t c = length + 1;
    size_t s = GRV_STR_ALLOC_GRANULARITY;
    return (c + (s - 1)) / s * s;
}

void grv_str_init_with_cstr(grv_str* s, char* cstr) {
  u32 len = strlen(cstr);
  if (len <= GRV_STR_SSO_MAX_LENGTH) {
    s->descriptor = len & GRV_STR_SSO_SIZE_MASK;
    strcpy(s->sso, cstr);
  } else {
    u64 capacity = grv_str_compute_capacity(len);
    s->capacity = capacity << 8;
    s->descriptor = GRV_STR_FLAG_IS_LARGE_STRING;
    s->start = 0;
    s->end = len;
    s->buffer = malloc(capacity);
    strncpy(s->buffer, cstr, capacity);
    s->buffer[capacity - 1] = 0x0;
  }
}

grv_str grv_str_new(char* cstr) {
  grv_str res;
  grv_str_init_with_cstr(&res, cstr);
  return res;
}

grv_str grv_str_empty() {
  grv_str res = {};
  return res;
}

grv_str grv_str_new_with_capacity(u64 capacity) {
  grv_str s;
  grv_str_init_with_capacity(&s, capacity);
  return s;
}

void grv_str_init_with_capacity(grv_str* s, u64 capacity) {  
  if (capacity <= GRV_STR_SSO_CAPACITY) {
    memset(s, 0, sizeof(grv_str));
  } else {
    capacity = grv_str_compute_capacity(capacity);
    s->capacity = capacity << 8;
    s->descriptor = GRV_STR_FLAG_IS_LARGE_STRING;
    s->buffer = malloc(capacity);
    s->start = 0;
    s->end = 0;
  }
}

void grv_str_free(grv_str* s) {
  if (s->descriptor & GRV_STR_FLAG_IS_LARGE_STRING
      && (~s->descriptor) & GRV_STR_FLAG_IS_REFERENCE
      && s->buffer) {
    free(s->buffer);
    s->buffer=NULL;
  }
}

void gvr_str_destroy(grv_str* s) {
  grv_str_free(s);
  free(s);
}

void grv_str_resize(grv_str* s, size_t size) {
  size_t new_capacity = grv_str_compute_capacity(size);
  if (grv_str_get_capacity(s) >= size + 1) {
    return;  
  } else if (grv_str_is_short(s)) {
    char* new_buffer = malloc(new_capacity);
    memcpy(new_buffer, s->sso, GRV_STR_SSO_CAPACITY);
    size_t length = grv_str_len(s);
    s->capacity = new_capacity << 8;
    s->descriptor = GRV_STR_FLAG_IS_LARGE_STRING;
    s->start = 0;
    s->end = length;
    s->buffer = new_buffer;
  } else {
    s->buffer = realloc(s->buffer, new_capacity);
    u8 desc = s->descriptor;
    s->capacity = new_capacity << 8;
    s->descriptor = desc;
  }
}

char* grv_str_get_buffer(grv_str* s) {
  if (s->descriptor & GRV_STR_FLAG_IS_LARGE_STRING) {
    return s->buffer + s->start;
  } else {
    return s->sso;
  }
}

char* grv_str_get_cstr(grv_str* s) {
  char* buffer = calloc(grv_str_len(s) + 1, 1);
  memcpy(buffer, grv_str_get_buffer(s), grv_str_len(s));
}

void grv_str_copy_to_buffer(char* dst, grv_str* s) {
  if (grv_str_is_short(s)) {
    memcpy(dst, s->sso, grv_str_len(s));
  } else {
    memcpy(dst, s->buffer + s->start, grv_str_len(s));
  }
}

grv_str grv_str_cat(grv_str* a, grv_str* b) {
  u64 len = grv_str_len(a) + grv_str_len(b); 
  grv_str r;
  grv_str_init_with_capacity(&r, len + 1);
  char* dst = grv_str_get_buffer(&r);
  grv_str_copy_to_buffer(dst, a);
  grv_str_copy_to_buffer(dst + grv_str_len(a), b);
  dst[len] = '\0';
  if (grv_str_is_short(&r)) {
    r.descriptor = len;
  } else {
    r.start = 0;
    r.end = len;
  }
  return r;
}
  
void grv_str_append_cstr(grv_str* s, char* cstr) {
  size_t cstr_len = strlen(cstr);
  grv_str_resize(s, grv_str_len(s) + cstr_len);
  strcpy(grv_str_get_buffer(s) + grv_str_len(s), cstr);
  grv_str_set_size(s, grv_str_len(s) + cstr_len);
}

grv_str grv_str_join(grv_str* s1, grv_str* s2, char* join_str) {
  grv_str res;
  size_t len1 = grv_str_len(s1);
  size_t len2 = grv_str_len(s2);
  size_t join_len = strlen(join_str);
  grv_str_init_with_capacity(&res, len1 + len2 + join_len);
  char* dst = grv_str_get_buffer(&res);
  char* src1 = grv_str_get_buffer(s1);
  char* src2 = grv_str_get_buffer(s2);
  memcpy(dst, src1, len1);
  memcpy(dst + len1, join_str, join_len);
  memcpy(dst + len1 + join_len, src2, len2);
  dst[len1 + join_len + len2] = 0;
  grv_str_set_size(&res, len1 + len2 + join_len);
  return res;
}

bool grv_str_starts_with_cstr(grv_str* s, char* cstr) {
  size_t len = strlen(cstr);
  char* buffer = grv_str_get_buffer(s);
  return memcmp(buffer, cstr, len) == 0;
}

bool grv_str_ends_with_cstr(grv_str* s, char* cstr) {
  size_t len = strlen(cstr);
  char* buffer = grv_str_get_buffer(s);
  size_t start_pos = grv_str_len(s) - len;
  return memcmp(buffer + start_pos, cstr, len) == 0;
}

void grv_str_lstrip(grv_str* s) {
  size_t start = 0;
  size_t len = grv_str_len(s);
  char* buffer = grv_str_get_buffer(s);
  for (start = 0; start <= len; ++start) {
    char c = buffer[start];
    if (!(c == ' ' || c == '\n' || c == '\t')) break;
  }

  if (start == 0) return;

  if (grv_str_is_short(s)) {
    size_t new_len = grv_str_len(s) - start;
    memmove(s->sso, s->sso + start, new_len + 1);
    s->descriptor = new_len & GRV_STR_SSO_SIZE_MASK;
  } else {
    s->start = start;
  }
}

void grv_str_rstrip(grv_str* s) {
  size_t end;
  size_t len = grv_str_len(s);
  char* buffer = grv_str_get_buffer(s);
  for (end = len - 1; end >= 0; end--) {
    char c = buffer[end];
    if (!(c == ' ' || c == '\n' || c == '\t')) break;
  }
  
  if (end == len - 1) return;

  if (grv_str_is_short(s)) {
    s->sso[end + 1] = 0;
    s->descriptor = (end + 1) & GRV_STR_SSO_SIZE_MASK;
  }
  else {
    s->end = end + 1;
  }
}

// grv_strarr
grv_strarr grv_strarr_new() {
  grv_strarr r = { .size=0, .capacity=GRV_STRARR_DEFAULT_CAPACITY };
  r.arr = calloc(GRV_STRARR_DEFAULT_CAPACITY, sizeof(grv_str));
  return r;
}


void grv_strarr_push_cstr(grv_strarr* a, char* s) {
  if (a->size == a->capacity) {
    a->capacity *= GRV_STRARR_GROW_FACTOR;
    a->arr = realloc(a->arr, a->capacity);
  }
  a->arr[a->size++] = grv_str_new(s);
}



