#ifndef GRV_STR_H
#define GRV_STR_H

#include "grv_defines.h"
#include <stddef.h>

#define GRV_STR_ALLOC_GRANULARITY 64
#define GRV_STR_FLAG_IS_LARGE_STRING 0x80
#define GRV_STR_FLAG_IS_REFERENCE 0x40
#define GRV_STR_FLAG_IS_INVALID 0x20
#define GRV_STR_SSO_SIZE_MASK 0x1f
#define GRV_STR_CAPACITY_MASK 0x1f
#define GRV_STR_SSO_MAX_LENGTH 30
#define GRV_STR_SSO_CAPACITY (GRV_STR_SSO_MAX_LENGTH + 1)

#define GRV_STRARR_DEFAULT_CAPACITY 32
#define GRV_STRARR_GROW_FACTOR 2

typedef struct {
  union {
    struct {
      u64 capacity;
      u64 start;
      u64 end;
      char* buffer;
    };
    struct {
      u8 descriptor;
      char sso[GRV_STR_SSO_MAX_LENGTH + 1];
    }; 
  };
} grv_str;


typedef struct {
  u32 capacity;
  u32 size;
  grv_str* arr;
} grv_strarr;


// create a new string object
grv_str grv_str_new(char*);
void grv_str_init_with_cstr(grv_str*, char*);

// create a new empty string object with given capacity
grv_str grv_str_empty();

void grv_str_init_with_capacity(grv_str*, u64);

// free the string memory
void grv_str_free(grv_str*);

// destroy a string object
void grv_str_destroy(grv_str*);

char* grv_str_get_buffer(grv_str*);

static inline bool grv_str_is_short(grv_str* s) {
  return !(s->descriptor & GRV_STR_FLAG_IS_LARGE_STRING);
}

// compute the length of the string
static inline u64 grv_str_len(grv_str* s) { 
  return s->descriptor & GRV_STR_FLAG_IS_LARGE_STRING
    ? s->end - s->start
    : s->descriptor & GRV_STR_SSO_SIZE_MASK;
}

static inline u64 grv_str_get_start(grv_str* s) {
  return grv_str_is_short(s) ? 0 : s->start;
}

static inline u64 grv_str_get_end(grv_str* s) {
  return grv_str_is_short(s) 
    ? s->descriptor & GRV_STR_CAPACITY_MASK
    : s->end;
}

static inline u64 grv_str_get_capacity(grv_str* s) {
  if (grv_str_is_short(s)) {
    return GRV_STR_SSO_MAX_LENGTH + 1;
  } else {
    u64 result = s->capacity >> 8;
    return result;
  }
}

static inline void grv_str_set_size(grv_str* s, size_t size) {
  if (grv_str_is_short(s)) {
    s->descriptor &= ~GRV_STR_SSO_SIZE_MASK;
    s->descriptor |= (GRV_STR_SSO_SIZE_MASK & (u8)size);
  } else {
    s->end = s->start + size;
  }
}

char* grv_str_get_cstr(grv_str*);

// concatenate two strings
grv_str grv_str_cat(grv_str*, grv_str*);
//grv_str grv_str_cat_cstr(grv_str, char*);
//#define grv_str_cat(A, B) _Generic((B), grv_str: grv_str_cat(A, B), char*: grv_str_cat_cstr(A, B))

void grv_str_append_cstr(grv_str*, char*);

void grv_str_resize(grv_str*, size_t);

bool grv_str_starts_with_cstr(grv_str*, char*);
bool grv_str_ends_with_cstr(grv_str*, char*);

void grv_str_lstrip(grv_str*);
void grv_str_rstrip(grv_str*);

grv_str grv_str_join(grv_str*, grv_str*, char*);

grv_strarr grv_strarr_new();

// push a string on the list
// the string will be copied to a new grv_str object
void grv_strarr_push_cstr(grv_strarr*, char*);


#endif
