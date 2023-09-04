#ifndef GRV_STR_H
#define GRV_STR_H

#include "grv_base.h"
#include <stddef.h>
#include <errno.h>

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
} grv_str_t;


typedef struct {
  u32 capacity;
  u32 size;
  grv_str_t* arr;
} grv_strarr_t;

// create a new string object
grv_str_t grv_str_new(char*);


// create a new string given a char* and a length
grv_str_t grv_str_new_with_len(char*, size_t len);

// create a new string object that fits a string of at least the given length
// parameters:
//  length: the minimum capacity of the string
grv_str_t grv_str_new_with_capacity(u64 length);

// create a copy of the given string
grv_str_t grv_str_copy(grv_str_t*);

// initialize a string object with a c string
// this will copy the c string into the string object
void grv_str_init_with_cstr(grv_str_t*, char*);

// initialize a string object with a c string and a given length
// this will copy the c string into the string object
void grv_str_init_with_cstr_and_length(grv_str_t*, char*, size_t len);


// create a new string reference object given a c string, a start and end index
grv_str_t grv_str_ref(char*, u64, u64);

// create a substring of the given string
// if the resulting string is a short string, it will be copied
// otherwise it will be a reference to the original string and not be null terminated
grv_str_t grv_str_substr(grv_str_t*, u64, u64);

// create a copy of a substring of the given string
// arguments:
//  s:     the string to copy from
//  start: the index of the first character to copy
//  end:   the index of the last character to copy
grv_str_t grv_str_copy_substr(grv_str_t* s, u64 start, u64 end);

// create a new empty string object with given capacity
grv_str_t grv_str_empty();

// initialize a string object with a given capacity
// the string will have a capacity of at least the given value
void grv_str_init_with_capacity(grv_str_t*, u64);

// free the string memory
void grv_str_free(grv_str_t*);

// destroy a string object
void grv_str_destroy(grv_str_t*);

// get the c string of the string object
char* grv_str_cstr(grv_str_t*);

// check if the string is a short string
static inline bool grv_str_is_short(grv_str_t* s) {
  if (s->descriptor & GRV_STR_FLAG_IS_LARGE_STRING || s->descriptor & GRV_STR_FLAG_IS_REFERENCE) {
    return false;
  } else {
    return true;
  } 
}

// check if the string is a reference
static inline bool grv_str_is_ref(grv_str_t* s) {
  return s->descriptor & GRV_STR_FLAG_IS_REFERENCE;
}


// compute the length of the string
static inline u64 grv_str_len(grv_str_t* s) { 
  return grv_str_is_short(s) 
    ? s->descriptor & GRV_STR_SSO_SIZE_MASK
    : s->end - s->start;
}

// get the index of the first character of the string
static inline u64 grv_str_get_start(grv_str_t* s) {
  return grv_str_is_short(s) ? 0 : s->start;
}

// get the index of the last character of the string
static inline u64 grv_str_get_end(grv_str_t* s) {
  return grv_str_is_short(s) 
    ? s->descriptor & GRV_STR_CAPACITY_MASK
    : s->end;
}

// get the capacity of the string
static inline u64 grv_str_capacity(grv_str_t* s) {
  if (grv_str_is_short(s)) {
    return GRV_STR_SSO_MAX_LENGTH + 1;
  } else {
    u64 result = s->capacity >> 8;
    return result;
    }
}


static inline void grv_str_set_size(grv_str_t* s, size_t size) {
  if (grv_str_is_short(s)) {
    s->descriptor &= ~GRV_STR_SSO_SIZE_MASK;
    s->descriptor |= (GRV_STR_SSO_SIZE_MASK & (u8)size);
  } else {
    s->end = s->start + size;
  }
}

// add null terminator to the string
void grv_str_add_null_terminator(grv_str_t*);


// create a cstring from the string object
// the caller is responsible for freeing the memory
char* grv_str_copy_cstr(grv_str_t*);


// slice a string by removing all characters outside the given range
// the input string shortened to the given range
// parameters:
//  s:     the string to slice
//  start: the index of the first character to keep (inclusive)
//  end:   the index of the last character to keep (exclusive)
void grv_str_slice(grv_str_t* s, u64 start, u64 end);

// split the string into two strings, given a delimiter
// the delimiter is not included in either string
// the input string is modified to contain the tail of the string
grv_str_t grv_str_split_head_from_front(grv_str_t*, char* delim);


grv_str_t grv_str_split_head_from_back(grv_str_t*, char* delim);
grv_str_t grv_str_split_tail_from_back(grv_str_t*, char* delim);

void grv_str_rchop(grv_str_t*, size_t);
void grv_str_lchop(grv_str_t*, size_t);

// concatenate two strings
grv_str_t grv_str_cat(grv_str_t*, grv_str_t*);

// concatenate a two cstrings
grv_str_t grv_str_cat_cstr_cstr(char*, char*);

//grv_str grv_str_cat_cstr(grv_str, char*);
//#define grv_str_cat(A, B) _Generic((B), grv_str: grv_str_cat(A, B), char*: grv_str_cat_cstr(A, B))

// append a string
void grv_str_append(grv_str_t*, grv_str_t*);

// prepend a cstring to the string
void grv_str_prepend_cstr(grv_str_t*, char*);


// append a character to the string 
void grv_str_append_char(grv_str_t*, char);

void grv_str_append_char_n(grv_str_t*, char, u64);

void grv_str_rpad(grv_str_t*, u64, char);
void grv_str_lpad(grv_str_t*, u64, char);
void grv_str_center(grv_str_t*, u64, char);

grv_str_t grv_str_repeat_char(char c, s32 n);

void grv_str_append_cstr(grv_str_t*, char*);

void grv_str_resize(grv_str_t*, size_t);


// check if the string is empty
bool grv_str_is_empty(grv_str_t*);

bool grv_str_starts_with_cstr(grv_str_t*, char*);
bool grv_str_starts_with(grv_str_t*, grv_str_t*);
bool grv_str_ends_with_cstr(grv_str_t*, char*);
bool grv_str_ends_with(grv_str_t*, grv_str_t*);

bool grv_str_contains_cstr(grv_str_t*, char*);

void grv_str_lstrip(grv_str_t*);
void grv_str_rstrip(grv_str_t*);
void grv_str_strip(grv_str_t*);

void grv_str_remove_trailing_newline(grv_str_t*);

// create a string from a signed 32-bit integer
grv_str_t grv_str_from_s32(s32 x);

// convert a string to a float value
f32 grv_str_to_f32(grv_str_t*);

// check if the represents a float
bool grv_str_is_float(grv_str_t*);

// format a u64 as a hex string
grv_str_t grv_str_from_u64(u64);


grv_str_t grv_str_join(grv_str_t*, grv_str_t*, char*);

// split a string into an array of strings
grv_strarr_t grv_str_split(grv_str_t*, char*);

// check for equality of two strings
bool grv_str_eq_cstr(grv_str_t*, char*);
bool grv_str_eq(grv_str_t*, grv_str_t*);

// create a new string array
grv_strarr_t grv_strarr_new();


// create a new string array from a c string array
// the strings are copied to new grv_str objects
grv_strarr_t grv_strarr_from_cstr_array(char**, size_t);

// get the size of the string array
size_t grv_strarr_size(grv_strarr_t*);

// free the string array
void grv_strarr_free(grv_strarr_t*);

// clear the string array
void grv_strarr_clear(grv_strarr_t*);

// push a string on the list
// the string will be copied to a new grv_str object
void grv_strarr_push_cstr(grv_strarr_t*, char*);

// push a string on the list
// the array takes ownership of the string, in case of a reference, the string is copied
void grv_strarr_push(grv_strarr_t*, grv_str_t*);

// push a string on the front of the list
void grv_strarr_push_front(grv_strarr_t*, grv_str_t*);

// append the contents of one string array to another
void grv_strarr_append(grv_strarr_t*, grv_strarr_t*);

grv_str_t* grv_strarr_at(grv_strarr_t*, size_t);
grv_str_t* grv_strarr_front(grv_strarr_t*);
grv_str_t* grv_strarr_back(grv_strarr_t*);

grv_str_t grv_strarr_get_copy(grv_strarr_t*, size_t);

// pop the first string off the list
// ownership of the string is transferred to the caller
grv_str_t grv_strarr_pop_front(grv_strarr_t*);

// pop the last string off the list
// ownership of the string is transferred to the caller
grv_str_t grv_strarr_pop_back(grv_strarr_t*);

// remove and free the first string from the list
void grv_strarr_remove_front(grv_strarr_t*);

// remove and free the last string from the list
void grv_strarr_remove_back(grv_strarr_t*);

// join the strings in the array with the given delimiter
// the caller takes ownership of the returned string
grv_str_t grv_strarr_join(grv_strarr_t*, char*);

// check if the string array contains the given cstring
bool grv_strarr_contains_cstr(grv_strarr_t*, char*);

int grv_strcpy(char* dst, size_t dstsz, const char* src);
int grv_strcat(char* dst, size_t dstsz, const char* src);
size_t grv_strlen(const char* s, size_t max);
#endif
