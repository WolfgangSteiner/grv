#ifndef GRV_STR_H
#define GRV_STR_H

#include "grv_base.h"
#include <stdarg.h>

typedef s32 grv_str_size_t;

struct grv_strarr_t;

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

// create grv_str, no ownership
grv_str_t grv_str_ref(char* cstr);

// create grv_str by copying cstr
grv_str_t grv_str_new(char* cstr);

grv_str_t grv_str_new_with_format(char* fmt, ...);
grv_str_t grv_str_new_with_capacity(grv_str_size_t capacity);
void grv_str_free(grv_str_t*);

grv_str_t grv_str_substr(grv_str_t, grv_str_size_t start, grv_str_size_t length);

grv_str_t grv_str_substr_with_iters(grv_str_iter_t start_iter, grv_str_iter_t end_iter);

GRV_INLINE grv_str_size_t grv_str_len(grv_str_t str) { return str.size; }
GRV_INLINE bool grv_str_empty(grv_str_t str) { return str.size == 0; }
GRV_INLINE bool grv_str_owns_data(grv_str_t str) { return str.owns_data; }
GRV_INLINE bool grv_str_is_valid(grv_str_t str) { return str.is_valid; }

bool grv_str_is_int(grv_str_t str);
int grv_str_to_int(grv_str_t str);
s64 grv_str_to_s64(grv_str_t str);
bool grv_str_is_float(grv_str_t str);
f32 grv_str_to_f32(grv_str_t str);
char grv_str_at(grv_str_t str, grv_str_size_t idx);
char grv_str_get_char(grv_str_t str, grv_str_size_t pos);

bool grv_str_eq_str(grv_str_t a, grv_str_t b);
static inline bool grv_str_eq_cstr(grv_str_t a, char* b) {return grv_str_eq_str(a, grv_str_ref(b));}
#define grv_str_eq(a, b) _Generic((b), grv_str_t: grv_str_eq_str, char*: grv_str_eq_cstr)(a,b)

bool grv_str_contains_str(grv_str_t, grv_str_t);
static inline bool grv_str_contains_cstr(grv_str_t a, char* b) {return grv_str_contains_str(a, grv_str_ref(b));}
bool grv_str_contains_char(grv_str_t, char);
#define grv_str_contains(a, b) _Generic((b), grv_str_t: grv_str_contains_str, char*: grv_str_contains_cstr, int: grv_str_contains_char)(a,b)

bool grv_str_starts_with_str(grv_str_t, grv_str_t);
static inline bool grv_str_starts_with_char(grv_str_t s, char c) { return s.size && s.data[0] == c; }
static inline bool grv_str_starts_with_cstr(grv_str_t a, char* b) {return grv_str_starts_with_str(a, grv_str_ref(b));}
#define grv_str_starts_with(a, b) _Generic((b), grv_str_t: grv_str_starts_with_str, char*: grv_str_starts_with_cstr, int: grv_str_starts_with_char)(a,b)

bool grv_str_ends_with_str(grv_str_t, grv_str_t);
static inline bool grv_str_ends_with_char(grv_str_t s, char c) { return s.size && s.data[s.size-1] == c; }
static inline bool grv_str_ends_with_cstr(grv_str_t a, char* b) {return grv_str_ends_with_str(a, grv_str_ref(b));}
#define grv_str_ends_with(a, b) _Generic((b), grv_str_t: grv_str_ends_with_str, char*: grv_str_ends_with_cstr, int: grv_str_ends_with_char)(a,b)

grv_str_t grv_str_cat_str_str(grv_str_t a, grv_str_t b);
static inline grv_str_t grv_str_cat_str_cstr(grv_str_t a, char* b) {return grv_str_cat_str_str(a, grv_str_ref(b));}
static inline grv_str_t grv_str_cat_cstr_str(char* a, grv_str_t b) {return grv_str_cat_str_str(grv_str_ref(a), b);}
static inline grv_str_t grv_str_cat_cstr_cstr(char* a, char* b) {return grv_str_cat_str_str(grv_str_ref(a), grv_str_ref(b));}
#define grv_str_cat(a,b) _Generic((a), \
    grv_str_t: _Generic((b), \
        grv_str_t: grv_str_cat_str_str, \
        char*: grv_str_cat_str_cstr \
    ), \
    char*: _Generic((b), \
        grv_str_t: grv_str_cat_cstr_str, \
        char*: grv_str_cat_cstr_cstr \
    ) \
)(a,b)

void grv_str_append_str(grv_str_t* str, grv_str_t append_str);
void grv_str_append_char(grv_str_t* str, char c);
static inline void grv_str_append_cstr(grv_str_t* str, char* append_str) { grv_str_append_str(str, grv_str_ref(append_str)); }
#define grv_str_append(a,b) _Generic((b), grv_str_t: grv_str_append_str, char*: grv_str_append_cstr, int: grv_str_append_char)(a,b)

void grv_str_prepend_str(grv_str_t* str, grv_str_t prepend_str);
void grv_str_prepend_char(grv_str_t* str, char c);
static inline void grv_str_prepend_cstr(grv_str_t* str, char* prepend_str) { grv_str_prepend_str(str, grv_str_ref(prepend_str)); }
#define grv_str_prepend(a,b) _Generic((b), grv_str_t: grv_str_prepend_str, char*: grv_str_prepend_cstr, int: grv_str_prepend_char)(a,b)

void grv_str_append_space(grv_str_t* str);

char* grv_str_cstr(grv_str_t str);
char* grv_str_copy_to_cstr(grv_str_t str);
grv_str_t grv_str_copy(grv_str_t str);

grv_str_t grv_str_repeat_char(char c, s32 count);

grv_str_t grv_str_lstrip(grv_str_t);
grv_str_t grv_str_lstrip_char(grv_str_t, char);
grv_str_t grv_str_rstrip_char(grv_str_t, char);
grv_str_t grv_str_strip_char(grv_str_t, char);
GRV_INLINE grv_str_t grv_str_remove_trailing_newline(grv_str_t str) { return grv_str_rstrip_char(str, '\n'); }

typedef grv_str_t(*grv_str_format_callback_t)(va_list*, grv_str_t pattern);
grv_str_t grv_str_format(grv_str_t fmt, ...); 
void grv_str_print_format(grv_str_t fmt, ...);
void grv_str_format_register_pattern(grv_str_t pattern, grv_str_format_callback_t callback);

grv_str_iter_t grv_str_find_char(grv_str_t* str, char c);
grv_str_iter_t grv_str_rfind_char(grv_str_t* str, char c);
grv_str_iter_t grv_str_find_any_char(grv_str_t* str, grv_str_t chars);

grv_str_t grv_str_split_tail_at_char(grv_str_t str, char c);
grv_str_t grv_str_reduce_char_spans(grv_str_t, char);
struct grv_strarr_t grv_str_split(grv_str_t str, grv_str_t sep);
struct grv_strarr_t grv_str_split_whitespace(grv_str_t str);

grv_str_iter_t grv_str_find_str(grv_str_t* str, grv_str_t match_str);
grv_str_iter_t grv_str_rfind_str(grv_str_t* str, grv_str_t match_str);

grv_str_t grv_separate_head_front(grv_str_t* str, grv_str_t sep);
grv_str_t grv_separate_head_back(grv_str_t* str, grv_str_t sep);
grv_str_t grv_separate_tail_front(grv_str_t* str, grv_str_t sep);
grv_str_t grv_separate_tail_back(grv_str_t* str, grv_str_t sep);

bool grv_str_iter_is_end(grv_str_iter_t* iter);
bool grv_str_iter_is_rend(grv_str_iter_t* iter);
grv_str_iter_t grv_str_iter_begin(grv_str_t* str);
GRV_INLINE grv_str_iter_t grv_str_iter_end(grv_str_t* str) { return (grv_str_iter_t){.str=str, .pos=str->size}; } 
grv_str_iter_t grv_str_iter_rbegin(grv_str_t* str);
GRV_INLINE grv_str_iter_t grv_str_iter_rend(grv_str_t* str) { return (grv_str_iter_t){.str=str, .pos=-1}; } 
void grv_str_print(grv_str_t str);
grv_str_t grv_get_line(grv_str_iter_t* iter);
grv_str_t grv_read_file(grv_str_t file_name);
bool grv_file_exists(grv_str_t file_name);

int grv_char_to_int(char c);
bool grv_is_digit(char c);
bool grv_is_whitespace(char c);
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
bool grv_str_iter_is_whitespace(grv_str_iter_t* iter);
bool grv_str_iter_match_white_space(grv_str_iter_t* iter);
bool grv_str_iter_match_char(grv_str_iter_t* iter, char match);
bool grv_str_iter_match_any_char(grv_str_iter_t* iter, grv_str_t chars);
bool grv_str_iter_match_newline(grv_str_iter_t* iter);
bool grv_str_iter_match_str(grv_str_iter_t* iter, grv_str_t match_str);
grv_str_t grv_str_iter_match_up_to_char(grv_str_iter_t* iter, char match);
grv_str_t grv_str_iter_match_up_to_whitespace(grv_str_iter_t* iter);
grv_str_t grv_str_iter_match_up_to_non_whitespace(grv_str_iter_t* iter);
grv_str_t grv_str_iter_match_up_to_str(grv_str_iter_t* iter, grv_str_t match_str);
grv_str_t grv_str_iter_match_word(grv_str_iter_t* iter);
bool grv_str_iter_eq_str(grv_str_iter_t* iter, grv_str_t match_str);

#ifndef GRV_NO_SHORT_NAMES
typedef grv_str_t str_t;
typedef grv_str_iter_t str_iter_t;
typedef grv_str_format_callback_t str_format_callback_t;
#define str_ref grv_str_ref
#define str_new grv_str_new
#define str_new_with_capacity grv_str_new_with_capacity
#define str_substr grv_str_substr
#define str_free grv_str_free
#define str_len grv_str_len
#define str_to_int grv_str_to_int
#define str_to_s64 grv_str_to_s64
#define str_at grv_str_at
#define str_get_char grv_str_get_char

#define str_eq grv_str_eq
#define str_eq_str grv_str_eq_str
#define str_eq_cstr grv_str_eq_cstr

#define str_contains grv_str_contains
#define str_contains_str grv_str_contains_str
#define str_contains_cstr grv_str_contains_cstr
#define str_contains_char grv_str_contains_char

#define str_starts_with grv_str_starts_with
#define str_starts_with_char grv_str_starts_with_char
#define str_starts_with_str grv_str_starts_with_str
#define str_starts_with_cstr grv_str_starts_with_cstr

#define str_ends_with grv_str_ends_with
#define str_ends_with_char grv_str_ends_with_char
#define str_ends_with_str grv_str_ends_with_str
#define str_ends_with_cstr grv_str_ends_with_cstr

#define str_cat grv_str_cat
#define str_cat_str_str grv_str_cat_str_str
#define str_cat_str_cstr grv_str_cat_str_cstr
#define str_cat_cstr_str grv_str_cat_cstr_str
#define str_cat_cstr_cstr grv_str_cat_cstr_cstr

#define str_append grv_str_append
#define str_append_str grv_str_append_str
#define str_append_cstr grv_str_append_cstr
#define str_append_char grv_str_append_char

#define str_prepend grv_str_prepend
#define str_prepend_str grv_str_prepend_str
#define str_prepend_cstr grv_str_prepend_cstr
#define str_prepend_char grv_str_prepend_char

#define str_append_space grv_str_append_space

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
#define str_rfind_char grv_str_rfind_char
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
#define is_whitespace grv_is_whitespace
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
#define str_iter_is_whitespace grv_str_iter_is_whitespace
#define str_iter_match_white_space grv_str_iter_match_white_space
#define str_iter_match_char grv_str_iter_match_char
#define str_iter_match_any_char grv_str_iter_match_any_char
#define str_iter_match_newline grv_str_iter_match_newline
#define str_iter_match_up_to_char grv_str_iter_match_up_to_char
#define str_iter_match_word grv_str_iter_match_word
#endif

#endif
