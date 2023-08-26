#include "grv/grv_base.h"
#include "grv/grv_str.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h> 

// set the length value of the string
static inline void grv_str_set_len_impl(grv_str_t* s, size_t len) {
  if (grv_str_is_short(s)) {
    s->descriptor = len & GRV_STR_SSO_SIZE_MASK;
  } else {
    s->end = s->start + len;
  }
}

static inline u64 grv_str_compute_capacity(u64 length) {
    size_t c = length + 1;
    size_t s = GRV_STR_ALLOC_GRANULARITY;
    return (c + (s - 1)) / s * s;
}

void grv_str_init_with_cstr(grv_str_t* s, char* cstr) {
  size_t len = strlen(cstr);
  if (len <= GRV_STR_SSO_MAX_LENGTH) {
    s->descriptor = len & GRV_STR_SSO_SIZE_MASK;
    grv_strcpy(s->sso, GRV_STR_SSO_CAPACITY, cstr);
  } else {
    u64 capacity = grv_str_compute_capacity(len);
    s->capacity = capacity << 8;
    s->descriptor = GRV_STR_FLAG_IS_LARGE_STRING;
    s->start = 0;
    s->end = len;
    s->buffer = malloc(capacity);
    grv_strcpy(s->buffer, capacity, cstr);
    s->buffer[capacity - 1] = 0x0;
  }
}

grv_str_t grv_str_new(char* cstr) {
  grv_str_t res;
  grv_str_init_with_cstr(&res, cstr);
  return res;
}

grv_str_t grv_str_new_with_capacity(size_t length) {
  grv_str_t res;
  grv_str_init_with_capacity(&res, length);
  return res;
}

void grv_str_add_null_terminator(grv_str_t* s) {
  if (grv_str_is_short(s)) {
    size_t len = grv_str_len(s);
    s->sso[len] = 0x0;
  } else {
    s->buffer[s->end] = 0x0;
  }
}

void grv_str_append(grv_str_t* s, grv_str_t* t) {
  size_t len = grv_str_len(s);
  size_t tlen = grv_str_len(t);
  grv_str_resize(s, len + tlen);
  char* dst = grv_str_cstr(s) + len;
  char* src = grv_str_cstr(t);
  memcpy(dst, src, tlen);
  grv_str_cstr(s)[len + tlen] = 0x0;
  grv_str_set_size(s, len + tlen);
}

void grv_str_prepend_cstr(grv_str_t* s, char* cstr) {
  size_t len = grv_str_len(s);
  size_t clen = strlen(cstr);
  grv_str_resize(s, len + clen);
  char* dst = grv_str_cstr(s) + clen;
  char* src = grv_str_cstr(s);
  memcpy(dst, src, len);
  memcpy(grv_str_cstr(s), cstr, clen);
  grv_str_cstr(s)[len + clen] = 0x0;
  grv_str_set_size(s, len + clen);
}


void grv_str_remove_trailing_newline(grv_str_t* s) {
  if (grv_str_is_short(s)) {
    size_t len = grv_str_len(s);
    if (s->sso[len - 1] == '\n') {
      s->sso[len - 1] = 0x0;
      grv_str_set_size(s, len - 1);
    } 
  } else {
    size_t len = grv_str_len(s);
    if (s->buffer[len - 1] == '\n') {
      grv_str_set_size(s, len - 1);
    }
  }
}

grv_str_t grv_str_ref(char* cstr, u64 start, u64 end) {
  grv_str_t res;
  res.descriptor = GRV_STR_FLAG_IS_REFERENCE;
  res.start = start;
  res.end = end;
  res.buffer = cstr;
  return res;
}

grv_str_t grv_str_substr(grv_str_t* s, u64 start, u64 end) {
  grv_str_t res = {0};
  size_t len = end - start;
  if (len < GRV_STR_SSO_MAX_LENGTH) {
    res.descriptor = len & GRV_STR_SSO_SIZE_MASK;
    memcpy(res.sso, grv_str_cstr(s) + start, len);
    res.sso[len] = 0x0;
  } else {
    res.descriptor = GRV_STR_FLAG_IS_REFERENCE;
    res.start = start;
    res.end = end;
    res.buffer = s->buffer;
  }
  return res;
}

grv_str_t grv_str_copy_substr(grv_str_t* s, u64 start, u64 end) {
  grv_str_t res = {0};
  size_t len = end - start;
  grv_str_init_with_capacity(&res, len + 1);
  memcpy(grv_str_cstr(&res), grv_str_cstr(s) + start, len);
  grv_str_set_len_impl(&res, len);
  grv_str_add_null_terminator(&res);
  return res;
}

grv_str_t grv_str_split_head_from_front(grv_str_t* s, char* delim) {
  grv_str_t res = {0};
  char* buffer = grv_str_cstr(s);
  size_t len = grv_str_len(s);
  size_t delim_len = strlen(delim);
  if (len <= delim_len) {
    return res;
  }

  size_t pos = 0;
  while (pos < len - delim_len) {
    if (memcmp(buffer + pos, delim, delim_len) == 0) {
      res = grv_str_copy_substr(s, 0, pos);    
      grv_str_lchop(s, pos + delim_len); 
      break;
    }
    pos++;
  }

  return res;
}

grv_str_t grv_str_split_head_from_back(grv_str_t* s, char* delim) {
  grv_str_t res = {0};
  char* buffer = grv_str_cstr(s);
  size_t len = grv_str_len(s);
  size_t delim_len = strlen(delim);
  if (len <= delim_len) {
    return res;
  }

  size_t pos = len - delim_len;
  while (pos >= 0) {
    if (memcmp(buffer + pos, delim, delim_len) == 0) {
      res = grv_str_substr(s, 0, pos);
      break;
    }
    pos--;
  }

  return res;
}

grv_str_t grv_str_split_tail_from_back(grv_str_t* s, char* delim) {
  grv_str_t res = {0};
  char* buffer = grv_str_cstr(s);
  size_t len = grv_str_len(s);
  size_t delim_len = strlen(delim);
  if (len <= delim_len) {
    return res;
  }

  size_t pos = len - delim_len;
  while (pos >= 0) {
    if (memcmp(buffer + pos, delim, delim_len) == 0) {
      res = grv_str_substr(s, pos + delim_len, len);
      break;
    }
    pos--;
  }

  return res;
}

grv_str_t grv_str_copy(grv_str_t* s) {
  grv_str_t res = {0};
  size_t len = grv_str_len(s);
  if (grv_str_is_short(s)) {
    res.descriptor = s->descriptor;
    memcpy(res.sso, s->sso, GRV_STR_SSO_CAPACITY);
  } else {
    grv_str_init_with_capacity(&res, len + 1);
    char* dst = grv_str_cstr(&res);
    memcpy(dst, grv_str_cstr(s), len);
    dst[len] = 0x0;
    if (grv_str_is_short(&res)) {
      res.descriptor = len & GRV_STR_SSO_SIZE_MASK;
    } else {
      res.end = len;
    }
  }
  return res;
}

void grv_str_slice(grv_str_t* s, u64 start, u64 end) {
  size_t new_len = end - start;
  if (grv_str_is_short(s)) {
    s->descriptor = new_len & GRV_STR_SSO_SIZE_MASK;
    if (start > 0) {
      memcpy(s->sso, s->sso + start, new_len);
    }
  } else if (new_len < GRV_STR_SSO_MAX_LENGTH && ! grv_str_is_ref(s)) {
    char* buffer = s->buffer;
    s->descriptor = new_len & GRV_STR_SSO_SIZE_MASK;
    memcpy(s->sso, buffer + start, new_len);
    free(buffer);
  } else {
    s->start = start;
    s->end = end;
  }
  grv_str_add_null_terminator(s);
}

void grv_str_rchop(grv_str_t* s, size_t n) {
  n = min_size_t(n, grv_str_len(s));
  if (grv_str_is_short(s)) {
    s->descriptor -= (u8)n;
    s->sso[s->descriptor & GRV_STR_SSO_SIZE_MASK] = 0x0;
  } else {
    s->end -= n;
    s->buffer[s->end] = 0x0;
  }
}

void grv_str_lchop(grv_str_t* s, size_t n) {
  if (grv_str_is_short(s)) {
    size_t new_len = grv_str_len(s) - n;
    s->descriptor -= n;
    memmove(s->sso, s->sso + n, new_len);
    s->sso[new_len] = 0x0;
  } else {
    s->start += n;
  }
}

grv_str_t grv_str_empty() {
  grv_str_t res = {0};
  return res;
}

bool grv_str_eq_cstr(grv_str_t* s, char* cstr) {
  size_t len = grv_str_len(s);
  size_t cstr_len = strlen(cstr);
  if (len != cstr_len) {
    return false;
  }
  char* buffer = grv_str_cstr(s);
  return memcmp(buffer, cstr, len) == 0;
}

bool grv_str_eq(grv_str_t* s1, grv_str_t* s2) {
  size_t len1 = grv_str_len(s1);
  size_t len2 = grv_str_len(s2);
  if (len1 != len2) {
    return false;
  }
  char* buffer1 = grv_str_cstr(s1);
  char* buffer2 = grv_str_cstr(s2);
  return memcmp(buffer1, buffer2, len1) == 0;
}

void grv_str_init_with_capacity(grv_str_t* s, u64 capacity) {  
  if (capacity <= GRV_STR_SSO_CAPACITY) {
    memset(s, 0, sizeof(grv_str_t));
  } else {
    capacity = grv_str_compute_capacity(capacity);
    s->capacity = capacity << 8;
    s->descriptor = GRV_STR_FLAG_IS_LARGE_STRING;
    s->buffer = malloc(capacity);
    s->start = 0;
    s->end = 0;
  }
}

void grv_str_free(grv_str_t* s) {
  if (s->descriptor & GRV_STR_FLAG_IS_LARGE_STRING
      && (~s->descriptor) & GRV_STR_FLAG_IS_REFERENCE
      && s->buffer) {
    free(s->buffer);
    s->buffer=NULL;
  }
}

void gvr_str_destroy(grv_str_t* s) {
  grv_str_free(s);
  free(s);
}

void grv_str_resize(grv_str_t* s, size_t size) {
  size_t new_capacity = grv_str_compute_capacity(size);
  if (grv_str_capacity(s) >= size + 1) {
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

char* grv_str_cstr(grv_str_t* s) {
  if (grv_str_is_short(s)) {
    return s->sso;
  } else {
    return s->buffer + s->start;
  }
}

char* grv_str_copy_cstr(grv_str_t* s) {
  char* buffer = calloc(grv_str_len(s) + 1, 1);
  memcpy(buffer, grv_str_cstr(s), grv_str_len(s));
  return buffer;
}

void grv_str_copy_to_buffer(char* dst, grv_str_t* s) {
  if (grv_str_is_short(s)) {
    memcpy(dst, s->sso, grv_str_len(s));
  } else {
    memcpy(dst, s->buffer + s->start, grv_str_len(s));
  }
}

grv_str_t grv_str_cat(grv_str_t* a, grv_str_t* b) {
  u64 len = grv_str_len(a) + grv_str_len(b); 
  grv_str_t r;
  grv_str_init_with_capacity(&r, len + 1);
  char* dst = grv_str_cstr(&r);
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
  
grv_str_t grv_str_cat_cstr_cstr(char* a, char* b) {
  u64 len = strlen(a) + strlen(b); 
  grv_str_t r;
  grv_str_init_with_capacity(&r, len + 1);
  char* dst = grv_str_cstr(&r);
  memcpy(dst, a, strlen(a));
  memcpy(dst + strlen(a), b, strlen(b));
  grv_str_set_len_impl(&r, len);
  grv_str_add_null_terminator(&r);
  return r;
}

  
void grv_str_append_char(grv_str_t* s, char c) {
  grv_str_resize(s, grv_str_len(s) + 1);
  char* buffer = grv_str_cstr(s);
  buffer[grv_str_len(s)] = c;
  buffer[grv_str_len(s) + 1] = '\0';
  grv_str_set_size(s, grv_str_len(s) + 1);
}

void grv_str_append_char_n(grv_str_t* s, char c, u64 n) {
  grv_str_resize(s, grv_str_len(s) + n);
  char* buffer = grv_str_cstr(s);
  memset(buffer + grv_str_len(s), c, n);
  buffer[grv_str_len(s)] = '\0';
  grv_str_set_size(s, grv_str_len(s) + n);
}

void grv_str_rpad(grv_str_t* s, u64 width, char pad_char) {
  if (grv_str_len(s) < width) {
    grv_str_append_char_n(s, pad_char, width - grv_str_len(s));
  }
}

void grv_str_lpad(grv_str_t* s, u64 width, char pad_char) {
  if (grv_str_len(s) < width) {
    grv_str_resize(s, width);
    char* buffer = grv_str_cstr(s);
    memmove(buffer + width - grv_str_len(s), buffer, grv_str_len(s));
    memset(buffer, pad_char, width - grv_str_len(s));
    buffer[width] = '\0';
    grv_str_set_size(s, width);
  }
}

void grv_str_center(grv_str_t* s, u64 width, char pad_char) {
  if (grv_str_len(s) < width) {
    grv_str_resize(s, width);
    char* buffer = grv_str_cstr(s);
    u64 left_pad = (width - grv_str_len(s)) / 2;
    u64 right_pad = width - grv_str_len(s) - left_pad;
    memmove(buffer + left_pad, buffer, grv_str_len(s));
    memset(buffer, pad_char, left_pad);
    memset(buffer + left_pad + grv_str_len(s), pad_char, right_pad);
    buffer[width] = '\0';
    grv_str_set_size(s, width);
  }
}

// consruct a grv_str_t by giving a char and a number of times to repeat it
grv_str_t grv_str_repeat_char(char c, s32 n) {
  grv_str_t res = {0};
  if (n <= 0) return res;
  grv_str_init_with_capacity(&res, n + 1);
  char* buffer = grv_str_cstr(&res);
  memset(buffer, c, n);
  buffer[n] = '\0';
  grv_str_set_size(&res, n);
  return res;
}


void grv_str_append_cstr(grv_str_t* s, char* cstr) {
  size_t cstr_len = strlen(cstr);
  grv_str_resize(s, grv_str_len(s) + cstr_len);
  strcpy(grv_str_cstr(s) + grv_str_len(s), cstr);
  grv_str_set_size(s, grv_str_len(s) + cstr_len);
}

grv_str_t grv_str_join(grv_str_t* s1, grv_str_t* s2, char* join_str) {
  grv_str_t res;
  size_t len1 = grv_str_len(s1);
  size_t len2 = grv_str_len(s2);
  size_t join_len = strlen(join_str);
  grv_str_init_with_capacity(&res, len1 + len2 + join_len);
  char* dst = grv_str_cstr(&res);
  char* src1 = grv_str_cstr(s1);
  char* src2 = grv_str_cstr(s2);
  memcpy(dst, src1, len1);
  memcpy(dst + len1, join_str, join_len);
  memcpy(dst + len1 + join_len, src2, len2);
  dst[len1 + join_len + len2] = 0;
  grv_str_set_size(&res, len1 + len2 + join_len);
  return res;
}

bool grv_str_is_empty(grv_str_t* s) {
  return grv_str_len(s) == 0;
}

bool grv_str_starts_with_cstr(grv_str_t* s, char* cstr) {
  size_t len = strlen(cstr);
  char* buffer = grv_str_cstr(s);
  return memcmp(buffer, cstr, len) == 0;
}

bool grv_str_starts_with(grv_str_t* s, grv_str_t* prefix) {
  size_t len = grv_str_len(prefix);
  if (grv_str_len(s) < len) {
    return false;
  }
  return memcmp(grv_str_cstr(s), grv_str_cstr(prefix), len) == 0;
}

bool grv_str_ends_with_cstr(grv_str_t* s, char* cstr) {
  size_t len = strlen(cstr);
  char* buffer = grv_str_cstr(s);
  size_t start_pos = grv_str_len(s) - len;
  return memcmp(buffer + start_pos, cstr, len) == 0;
}

bool grv_str_ends_with(grv_str_t* s, grv_str_t* suffix) {
  size_t len = grv_str_len(suffix);
  if (grv_str_len(s) < len) {
    return false;
  }
  size_t start_pos = grv_str_len(s) - len;
  return memcmp(grv_str_cstr(s) + start_pos, grv_str_cstr(suffix), len) == 0;
}

bool grv_str_contains_cstr(grv_str_t* s, char* cstr) {
  size_t len = grv_str_len(s); 
  size_t cstrlen = strlen(cstr);
  if (len < cstrlen) return false;
  char* buffer = grv_str_cstr(s);
  for (size_t i = 0; i <= len - cstrlen; ++i) {
    if (memcmp(buffer + i, cstr, cstrlen) == 0) {
      return true;
    }
  }
  return false;
} 

void grv_str_lstrip(grv_str_t* s) {
  size_t start = 0;
  size_t len = grv_str_len(s);
  char* buffer = grv_str_cstr(s);
  for (start = 0; start < len; ++start) {
    char c = buffer[start];
    if (!(c == ' ' || c == '\n' || c == '\t')) break;
  }

  if (start == 0) return;

  if (grv_str_is_short(s)) {
    size_t new_len = grv_str_len(s) - start;
    memmove(s->sso, s->sso + start, new_len + 1);
    s->descriptor = new_len & GRV_STR_SSO_SIZE_MASK;
  } else {
    s->start += start;
  }
}

void grv_str_rstrip(grv_str_t* s) {
  size_t end;
  size_t len = grv_str_len(s);
  char* buffer = grv_str_cstr(s);
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
    s->end = s->start + end + 1;
  }
}

grv_strarr_t grv_str_split(grv_str_t* s, char* delim) {
  grv_strarr_t r = grv_strarr_new();
  size_t delim_len = strlen(delim);
  size_t start = 0;
  size_t end = 0;
  size_t len = grv_str_len(s);
  char* buffer = grv_str_cstr(s);
  while (end < len) {
    if (memcmp(buffer + end, delim, delim_len) == 0) {
      grv_str_t substr = grv_str_substr(s, start, end);
      grv_strarr_push(&r, &substr);
      start = end + delim_len;
      end = start;
    } else {
      end++;
    }
  }
  grv_str_t substr = grv_str_substr(s, start, end);
  grv_strarr_push(&r, &substr);
  return r;
}

grv_str_t grv_str_from_s32(int32_t i) {
  grv_str_t s = {0};
  size_t len = snprintf(grv_str_cstr(&s), GRV_STR_SSO_CAPACITY, "%d", i);
  grv_str_set_len_impl(&s, len);
  return s;
}

f32 grv_str_to_f32(grv_str_t* s) {
  return strtod(grv_str_cstr(s), NULL);
}

grv_str_t grv_str_from_u64(u64 n) {
  grv_str_t res = {0};
  res.descriptor = 16;
  res.sso[16] = 0x0;
  for (int i = 0; i < 16; ++i) {
    u8 nibble = n & 0xf;
    res.sso[15 - i] = nibble < 0xa ? '0' + nibble : 'a' + nibble - 0xa;
    n = n >> 4;
  }
  return res;
}

bool grv_str_is_float(grv_str_t* s) {
  char* endptr;
  strtod(grv_str_cstr(s), &endptr);
  return *endptr == 0;
}

int grv_strcpy(char* dst, size_t dst_size, const char* src) {
  size_t len = strlen(src);
  int res = 0;

  if (dst == NULL || dst_size == 0) return EINVAL;
  if (len >= dst_size) {
    len = dst_size - 1;
    res = ERANGE;
  }
  memcpy(dst, src, len);
  dst[len] = '\0';
  return res;
}

int grv_strcat(char* dst, size_t dstsz, const char* src) {
  if (dst == NULL || dstsz == 0) return EINVAL;

  size_t dst_len = grv_strlen(dst, dstsz - 1);
  if (dst_len == dstsz - 1) return ERANGE;

  size_t src_len = strlen(src);
  int res = 0;

  if (dst_len + src_len >= dstsz) {
    src_len = dstsz - dst_len - 1;
    res = ERANGE;
  }
  memcpy(dst + dst_len, src, src_len);
  dst[dst_len + src_len] = '\0';
  return res;
}

size_t grv_strlen(const char* s, size_t max_len) {
  size_t len = 0;
  while (len < max_len && s[len] != '\0') {
    len++;
  }
  return len;
}
