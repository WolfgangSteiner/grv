#ifndef GRV_CSTR_H
#define GRV_CSTR_H

#include "grv_base.h"

typedef s32 grv_cstr_size_t;

char* grv_cstr_alloc(grv_cstr_size_t len);
char* grv_cstr_new();
void grv_cstr_free(char* str);
char* grv_cstr_copy(const char* src);
char* grv_cstr_cat(const char* a, const char* b);
char* grv_cstr_append(char* str, const char* append_str);
grv_cstr_size_t grv_cstr_len(const char* s);
char* grv_cstr_new_with_format(const char* fmt, ...);
char* grv_cstr_repeat_char(char c, grv_cstr_size_t n);


#endif