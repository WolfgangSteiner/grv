#ifndef GRV_STR_H
#define GRV_STR_H

#include "grv_base.h"
#include <stdarg.h>

typedef s32 grv_str_size_t;

typedef struct {
    char* data;
    grv_str_size_t size;
    bool is_valid;
    bool owns_data;
} grv_str_t;

typedef struct {
    grv_str_t* str;
    grv_str_size_t pos;
} grv_str_iter_t;

grv_str_t grv_str_ref(char* cstr);
grv_str_t grv_str_new(char* cstr);
grv_str_t grv_str_new_with_capacity(grv_str_size_t capacity);
void grv_str_free(grv_str_t*);

grv_str_t str_substr(grv_str_t, grv_str_size_t start, grv_str_size_t length);

static inline grv_str_size_t grv_str_len(grv_str_t str) { return str.size; }

int grv_str_to_int(grv_str_t str);
s64 grv_str_to_s64(grv_str_t str);
char grv_str_at(grv_str_t str, grv_str_size_t idx);
char grv_str_get_char(grv_str_t str, grv_str_size_t pos);

bool grv_str_eq(grv_str_t a, grv_str_t b);
bool grv_str_contains(grv_str_t, grv_str_t);
bool grv_str_contains_char(grv_str_t, char);
static inline bool grv_str_starts_with_char(grv_str_t s, char c) { return s.size && s.data[0] == c; }
static inline bool grv_str_ends_with_char(grv_str_t s, char c) { return s.size && s.data[s.size-1] == c; }
bool grv_str_starts_with(grv_str_t, grv_str_t);
bool grv_str_ends_with(grv_str_t, grv_str_t);
// create new str by referencing cstr
// new str does not own memory

grv_str_t grv_str_cat(grv_str_t a, grv_str_t b);

void grv_str_append(grv_str_t* str, grv_str_t append_str);
void grv_str_append_space(grv_str_t* str);
void grv_str_append_char(grv_str_t* str, char c);
void grv_str_prepend(grv_str_t* str, grv_str_t prepend_str);
void grv_str_prepend_char(grv_str_t* str, char c);
char* grv_str_cstr(grv_str_t str);
char* grv_str_copy_to_cstr(grv_str_t str);
grv_str_t grv_str_copy(grv_str_t str);

grv_str_t grv_str_repeat_char(char c, s32 count);

grv_str_t grv_str_lstrip(grv_str_t);
grv_str_t grv_str_lstrip_char(grv_str_t, char);
grv_str_t grv_str_rstrip_char(grv_str_t, char);
grv_str_t grv_str_strip_char(grv_str_t, char);

typedef grv_str_t(*grv_str_format_callback_t)(va_list*, grv_str_t pattern);
grv_str_t grv_str_format(grv_str_t fmt, ...); 
void grv_str_print_format(grv_str_t fmt, ...);
void grv_str_format_register_pattern(grv_str_t pattern, grv_str_format_callback_t callback);

grv_str_iter_t grv_str_find_char(grv_str_t* str, char c);
grv_str_iter_t grv_str_find_char_from_back(grv_str_t* str, char c);
grv_str_iter_t grv_str_find_any_char(grv_str_t str, grv_str_t chars);
grv_str_t grv_str_split_tail_at_char(grv_str_t str, char c);
grv_str_t grv_str_reduce_char_spans(grv_str_t, char);


bool grv_str_iter_is_end(grv_str_iter_t* iter);
bool grv_str_iter_is_rend(grv_str_iter_t* iter);
grv_str_iter_t grv_str_iter_begin(grv_str_t* str);
grv_str_iter_t grv_str_iter_rbegin(grv_str_t* str);
void grv_str_print(grv_str_t str);
grv_str_t grv_get_line(grv_str_iter_t* iter);
grv_str_t grv_read_file(grv_str_t file_name);
bool grv_file_exists(grv_str_t file_name);

int grv_char_to_int(char c);
bool grv_is_digit(char c);
bool grv_is_white_space(char c);
bool grv_is_word_separator(char c);
bool grv_is_newline(char c);

grv_str_t grv_str_from_int(int x);
grv_str_t grv_str_from_s64(s64 x);

char grv_str_iter_get_char(grv_str_iter_t* iter);
void grv_str_iter_set_char(grv_str_iter_t* iter, char c);
void grv_str_iter_inc(grv_str_iter_t* iter);
void grv_str_iter_dec(grv_str_iter_t* iter);
bool grv_str_iter_match(grv_str_iter_t* iter, grv_str_t match_str);
bool grv_str_iter_rmatch(grv_str_iter_t* iter, grv_str_t match_str);
int grv_str_iter_match_int(grv_str_iter_t* iter);
s64 grv_str_iter_match_s64(grv_str_iter_t* iter);
bool grv_str_iter_is_digit(grv_str_iter_t* iter);
bool grv_str_iter_is_white_space(grv_str_iter_t* iter);
bool grv_str_iter_match_white_space(grv_str_iter_t* iter);
bool grv_str_iter_match_char(grv_str_iter_t* iter, char match);
bool grv_str_iter_match_any_char(grv_str_iter_t* iter, grv_str_t chars);
bool grv_str_iter_match_newline(grv_str_iter_t* iter);
grv_str_t grv_str_iter_match_up_to_char(grv_str_iter_t* iter, char match);
grv_str_t grv_str_iter_match_word(grv_str_iter_t* iter);

#ifndef GRV_NO_SHORT_NAMES
typedef grv_str_t str_t;
typedef grv_str_iter_t str_iter_t;
typedef grv_str_format_callback_t str_format_callback_t;
#define str_ref grv_str_ref
#define str_new grv_str_new
#define str_new_with_capacity grv_str_new_with_capacity
#define str_free grv_str_free
#define str_len grv_str_len
#define str_to_int grv_str_to_int
#define str_to_s64 grv_str_to_s64
#define str_at grv_str_at
#define str_get_char grv_str_get_char
#define str_eq grv_str_eq
#define str_contains grv_str_contains
#define str_contains_char grv_str_contains_char
#define str_starts_with_char grv_str_starts_with_char
#define str_ends_with_char grv_str_ends_with_char
#define str_starts_with grv_str_starts_with
#define str_ends_with grv_str_ends_with
#define str_cat grv_str_cat
#define str_append grv_str_append
#define str_append_space grv_str_append_space
#define str_append_char grv_str_append_char
#define str_prepend grv_str_prepend
#define str_prepend_char grv_str_prepend_char
#define str_cstr grv_str_cstr
#define str_copy_to_cstr grv_str_copy_to_cstr
#define str_copy grv_str_copy
#define str_repeat_char grv_str_repeat_char
#define str_lstrip grv_str_lstrip
#define str_lstrip_char grv_str_lstrip_char
#define str_rstrip_char grv_str_rstrip_char
#define str_strip_char grv_str_strip_char
#define str_format grv_str_format
#define str_print_format grv_str_print_format
#define str_format_register_pattern grv_str_format_register_pattern
#define str_find_char grv_str_find_char
#define str_find_char_from_back grv_str_find_char_from_back
#define str_find_any_char grv_str_find_any_char
#define str_split_tail_at_char grv_str_split_tail_at_char
#define str_reduce_char_spans grv_str_reduce_char_spans
#define str_iter_is_end grv_str_iter_is_end
#define str_iter_is_rend grv_str_iter_is_rend
#define str_iter_begin grv_str_iter_begin
#define str_iter_rbegin grv_str_iter_rbegin
#define str_print grv_str_print
#define get_line grv_get_line
#define read_file grv_read_file
#define file_exists grv_file_exists
#define char_to_int grv_char_to_int
#define is_digit grv_is_digit
#define is_white_space grv_is_white_space
#define is_word_separator grv_is_word_separator
#define is_newline grv_is_newline
#define str_from_int grv_str_from_int
#define str_from_s64 grv_str_from_s64
#define str_iter_get_char grv_str_iter_get_char
#define str_iter_set_char grv_str_iter_set_char
#define str_iter_inc grv_str_iter_inc
#define str_iter_dec grv_str_iter_dec
#define str_iter_match grv_str_iter_match
#define str_iter_rmatch grv_str_iter_rmatch
#define str_iter_match_int grv_str_iter_match_int
#define str_iter_match_s64 grv_str_iter_match_s64
#define str_iter_is_digit grv_str_iter_is_digit
#define str_iter_is_white_space grv_str_iter_is_white_space
#define str_iter_match_white_space grv_str_iter_match_white_space
#define str_iter_match_char grv_str_iter_match_char
#define str_iter_match_any_char grv_str_iter_match_any_char
#define str_iter_match_newline grv_str_iter_match_newline
#define str_iter_match_up_to_char grv_str_iter_match_up_to_char
#define str_iter_match_word grv_str_iter_match_word
#endif


#endif