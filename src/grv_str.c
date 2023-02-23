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

void grv_str_append(grv_str* s, grv_str* t) {
  size_t len = grv_str_len(s);
  size_t tlen = grv_str_len(t);
  grv_str_resize(s, len + tlen);
  char* dst = grv_str_get_buffer(s) + len;
  char* src = grv_str_get_buffer(t);
  memcpy(dst, src, tlen);
  grv_str_get_buffer(s)[len + tlen] = 0x0;
  grv_str_set_size(s, len + tlen);
}


void grv_str_remove_trailing_newline(grv_str* s) {
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

grv_str grv_str_ref(char* cstr, u64 start, u64 end) {
  grv_str res;
  res.descriptor = GRV_STR_FLAG_IS_REFERENCE;
  res.start = start;
  res.end = end;
  res.buffer = cstr;
  return res;
}

grv_str grv_str_substr(grv_str* s, u64 start, u64 end) {
  grv_str res;
  if (grv_str_is_short(s)) {
    res.descriptor = (end - start) & GRV_STR_SSO_SIZE_MASK;
    strncpy(res.sso, s->sso + start, end - start);
    res.sso[end - start] = 0x0;
  } else {
    res.descriptor = GRV_STR_FLAG_IS_REFERENCE;
    res.start = start;
    res.end = end;
    res.buffer = s->buffer;
  }
  return res;
}

grv_str grv_str_split_tail_from_back(grv_str* s, char* delim) {
  grv_str res = {};
  char* buffer = grv_str_get_buffer(s);
  size_t len = grv_str_len(s);
  size_t delim_len = strlen(delim);
  if (len <= delim_len) {
    return res;
  }

  size_t pos = len - delim_len;
  while (pos > 0) {
    if (memcmp(buffer + pos, delim, delim_len) == 0) {
      res = grv_str_substr(s, pos + delim_len, len);
      break;
    }
    pos--;
  }

  return res;
}


grv_str grv_str_copy(grv_str* s) {
  grv_str res;
  if (grv_str_is_short(s)) {
    res.descriptor = s->descriptor;
    strcpy(res.sso, s->sso);
  } else {
    grv_str_init_with_capacity(&res, grv_str_len(s) + 1);
    memcpy(res.buffer, grv_str_get_buffer(s), grv_str_len(s));
    res.buffer[grv_str_len(s)] = 0x0;
    res.end = grv_str_len(s);
  }
  return res;
}

grv_str grv_str_slice(grv_str* s, u64 start, u64 end) {
  grv_str res;
  if (grv_str_is_short(s)) {
    res.descriptor = (end - start) & GRV_STR_SSO_SIZE_MASK;
    strncpy(res.sso, s->sso + start, end - start);
    res.sso[end - start] = 0x0;
  } else {
    grv_str_init_with_capacity(&res, end - start + 1);
    memcpy(res.buffer, grv_str_get_buffer(s), end - start);
    res.buffer[end - start] = 0x0;
    res.end = end - start;
  }
  return res;
}

void grv_str_rchop(grv_str* s, u64 n) {
  if (grv_str_is_short(s)) {
    s->descriptor -= n;
    s->sso[s->descriptor & GRV_STR_SSO_SIZE_MASK] = 0x0;
  } else {
    s->end -= n;
    s->buffer[s->end] = 0x0;
  }
}

void grv_str_lchop(grv_str* s, u64 n) {
  if (grv_str_is_short(s)) {
    size_t new_len = grv_str_len(s) - n;
    s->descriptor -= n;
    memmove(s->sso, s->sso + n, new_len);
    s->sso[new_len] = 0x0;
  } else {
    s->start += n;
  }
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

char* grv_str_copy_cstr(grv_str* s) {
  char* buffer = calloc(grv_str_len(s) + 1, 1);
  memcpy(buffer, grv_str_get_buffer(s), grv_str_len(s));
  return buffer;
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
  

void grv_str_append_char(grv_str* s, char c) {
  grv_str_resize(s, grv_str_len(s) + 1);
  char* buffer = grv_str_get_buffer(s);
  buffer[grv_str_len(s)] = c;
  buffer[grv_str_len(s) + 1] = '\0';
  grv_str_set_size(s, grv_str_len(s) + 1);
}

void grv_str_append_char_n(grv_str* s, char c, u64 n) {
  grv_str_resize(s, grv_str_len(s) + n);
  char* buffer = grv_str_get_buffer(s);
  memset(buffer + grv_str_len(s), c, n);
  buffer[grv_str_len(s)] = '\0';
  grv_str_set_size(s, grv_str_len(s) + n);
}

void grv_str_rpad(grv_str* s, u64 width, char pad_char) {
  if (grv_str_len(s) < width) {
    grv_str_append_char_n(s, pad_char, width - grv_str_len(s));
  }
}

void grv_str_lpad(grv_str* s, u64 width, char pad_char) {
  if (grv_str_len(s) < width) {
    grv_str_resize(s, width);
    char* buffer = grv_str_get_buffer(s);
    memmove(buffer + width - grv_str_len(s), buffer, grv_str_len(s));
    memset(buffer, pad_char, width - grv_str_len(s));
    buffer[width] = '\0';
    grv_str_set_size(s, width);
  }
}

void grv_str_center(grv_str* s, u64 width, char pad_char) {
  if (grv_str_len(s) < width) {
    grv_str_resize(s, width);
    char* buffer = grv_str_get_buffer(s);
    u64 left_pad = (width - grv_str_len(s)) / 2;
    u64 right_pad = width - grv_str_len(s) - left_pad;
    memmove(buffer + left_pad, buffer, grv_str_len(s));
    memset(buffer, pad_char, left_pad);
    memset(buffer + left_pad + grv_str_len(s), pad_char, right_pad);
    buffer[width] = '\0';
    grv_str_set_size(s, width);
  }
}

// consruct a grv_str by giving a char and a number of times to repeat it
grv_str grv_str_repeat_char(char c, u64 n) {
  grv_str res;
  grv_str_init_with_capacity(&res, n + 1);
  char* buffer = grv_str_get_buffer(&res);
  memset(buffer, c, n);
  buffer[n] = '\0';
  grv_str_set_size(&res, n);
  return res;
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

grv_strarr grv_str_split(grv_str* s, char* delim) {
  grv_strarr r = grv_strarr_new();
  size_t delim_len = strlen(delim);
  size_t start = 0;
  size_t end = 0;
  size_t len = grv_str_len(s);
  char* buffer = grv_str_get_buffer(s);
  while (end < len) {
    if (memcmp(buffer + end, delim, delim_len) == 0) {
      grv_str substr = grv_str_substr(s, start, end);
      grv_strarr_push(&r, &substr);
      start = end + delim_len;
      end = start;
    } else {
      end++;
    }
  }
  grv_str substr = grv_str_substr(s, start, end);
  grv_strarr_push(&r, &substr);
  return r;
}

f32 grv_str_to_f32(grv_str* s) {
  return strtod(grv_str_get_buffer(s), NULL);
}
