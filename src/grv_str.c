#include "grv/grv_str.h"
#include "grv/grv_error.h"
#include "grv/grv_strarr.h"
#include "grv/grv_base.h"
#include "grv/grv_common.h"
#include "grv/grv_memory.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define GRV_STR_MIN_CAPACITY_16
#define GRV_STR_ALLOC_GRANULARITY 16

#include "grv_str/grv_str_append_word.c"
#include "grv_str/grv_str_append_line.c"
#include "grv_str/grv_str_compare.c"
#include "grv_str/grv_str_get_word.c"
#include "grv_str/grv_str_remove_trailing_newline.c"
#include "grv_str/grv_str_lstrip.c"
#include "grv_str/grv_str_rstrip.c"
#include "grv_str/grv_str_split_char.c"
#include "grv_str/grv_str_split_head_front.c"
#include "grv_str/grv_str_strip.c"
#include "grv_str/grv_str_substr_with_iters.c"
#include "grv_str/grv_str_to_snake_case.c"
#include "grv_str/grv_str_read_from_file.c"

typedef struct {
    char* data;
    grv_str_size_t capacity;
} str_cstr_buffer_t;

static grv_str_size_t _grv_str_capacity_for_size(grv_str_size_t size) {
    if (size % GRV_STR_ALLOC_GRANULARITY == 0) return size;
    return (size / GRV_STR_ALLOC_GRANULARITY + 1) * GRV_STR_ALLOC_GRANULARITY;
}

static str_cstr_buffer_t str_cstr_buffer = { .data=0, .capacity=0 };

char* _get_cstr_buffer(grv_str_size_t capacity) {
    capacity = grv_max_i64(capacity, 1024);
    if (str_cstr_buffer.data == 0) {
        str_cstr_buffer.capacity = capacity;
        str_cstr_buffer.data = grv_alloc(str_cstr_buffer.capacity);
    } else if (str_cstr_buffer.capacity < capacity) {
        while (str_cstr_buffer.capacity < capacity) str_cstr_buffer.capacity *= 2;
        str_cstr_buffer.data = grv_realloc(str_cstr_buffer.data, str_cstr_buffer.capacity);
    }
    return str_cstr_buffer.data;
}

static bool _grv_str_iter_eq_str(grv_str_iter_t* iter, grv_str_t match_str) {
    if (iter->pos + match_str.size > iter->str->size) return false;
    return memcmp(iter->str->data + iter->pos, match_str.data, match_str.size) == 0;
}

grv_str_t grv_str_ref(char* cstr) {
    return (grv_str_t){ 
        .data=cstr,
        .size=strlen(cstr),
        .is_valid=true,
        .owns_data=false}; 
}

grv_str_t grv_str_repeat_char(char c, grv_str_size_t count) {
    grv_str_t r = {
        .data=malloc(count),
        .size=count,
        .is_valid=true,
        .owns_data=true};
    memset(r.data, c, count);
    return r;
}

char* grv_str_cstr(grv_str_t str) {
    if (str_cstr_buffer.data == 0) {
        str_cstr_buffer.capacity = 1024;
        str_cstr_buffer.data = grv_alloc(str_cstr_buffer.capacity);
    } 
    
    if (str_cstr_buffer.capacity < str.size + 1) {
        while (str_cstr_buffer.capacity < str.size + 1) str_cstr_buffer.capacity *= 2;
        str_cstr_buffer.data = grv_realloc(str_cstr_buffer.data, str_cstr_buffer.capacity);
    }

    memcpy(str_cstr_buffer.data, str.data, str.size);
    str_cstr_buffer.data[str.size] = '\0';
    return str_cstr_buffer.data;
}

char* grv_str_copy_to_cstr(grv_str_t str) {
    char* cstr = grv_alloc(str.size + 1);
    memcpy(cstr, str.data, str.size);
    cstr[str.size] = '\0';
    return cstr;
}

grv_str_t grv_str_substr(grv_str_t str, grv_str_size_t start, grv_str_size_t length) {
    grv_str_size_t size = length > 0 ? length : str.size - start + length + 1;
    return (grv_str_t){ 
        .data=str.data + start,
        .size=size,
        .is_valid=str.is_valid,
        .owns_data=false};
}

grv_str_t grv_str_lstrip_char(grv_str_t str, char c) {
    grv_str_t res = str;
    res.owns_data = false;
    while (res.size && *res.data == c) {
        res.data++;
        res.size--;
    }
    return res;
}

grv_str_t grv_str_rstrip_char(grv_str_t str, char c) {
    grv_str_t res = str;
    res.owns_data = false;
    while (res.size && res.data[res.size-1] == c) res.size--;
    return res;
}

grv_str_t grv_str_strip_char(grv_str_t str, char c) {
    grv_str_t res = grv_str_lstrip_char(str, c);
    return grv_str_rstrip_char(res, c);
}

bool grv_str_starts_with_str(grv_str_t str, grv_str_t start) {
    if (str.size < start.size) return false;
    return memcmp(str.data, start.data, start.size) == 0;
}

bool grv_str_ends_with_str(grv_str_t str, grv_str_t end) {
    if (str.size < end.size) return false;
    return memcmp(str.data + str.size - end.size, end.data, end.size) == 0;
}

bool grv_str_iter_is_end(grv_str_iter_t* iter) {
    return iter->pos >= iter->str->size;
}

bool grv_str_iter_is_rend(grv_str_iter_t* iter) {
    return iter->pos < 0;
}

grv_str_iter_t grv_str_iter_begin(grv_str_t* str) {
    return (grv_str_iter_t){ str, 0 };
}

grv_str_iter_t grv_str_iter_rbegin(grv_str_t* str) {
    return (grv_str_iter_t){ str, str->size - 1};
}

void grv_str_print(grv_str_t str) {
    for (grv_str_size_t i = 0; i < str.size; ++i) {
        putchar(str.data[i]);
    }
    putchar('\n');
}

grv_str_t grv_get_line(grv_str_iter_t* iter) {
    grv_str_t result = {0};
    if (grv_str_iter_is_end(iter)) {
        result.is_valid = false;
        return result;
    }
    result.data = iter->str->data + iter->pos;
    result.owns_data = false;
    grv_str_size_t pos = iter->pos;
    while (pos < iter->str->size) {
        if (iter->str->data[pos] == '\n') {
            result.size = pos - iter->pos;
            iter->pos = pos + 1;
            return result; 
        }
        pos++;
    }
    result.size = pos - iter->pos;
    iter->pos = pos;
    return result;
}

bool grv_str_write_to_file(grv_str_t str, grv_str_t filename, grv_error_t* error) {
    bool success = true;
    FILE* fp = fopen(grv_str_cstr(filename), "wb");
    if (fp == 0) {
        if (error) {
            *error = grv_error_create_msg_arg(
                GRV_ERROR_FILE_NOT_WRITABLE,
                grv_str_ref(_grv_error_file_not_writable_cstr),
                filename);
        }
        success = false;
        goto end;
    }
    unsigned long bytes_written = fwrite(str.data, str.size, 1, fp);
    if (bytes_written != str.size) {
        if (error) {
            *error = grv_error_create_msg_arg(
                GRV_ERROR_FILE_NO_CAPACITY,
                grv_str_ref(_grv_error_file_no_capacity_cstr),
                filename);
        }
        success = false;
    }
end:
    if (fp) fclose(fp);
    return success;
}

bool grv_file_exists(grv_str_t file_name) {
    FILE* file = fopen(grv_str_cstr(file_name), "rb");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

bool grv_str_contains_str(grv_str_t str, grv_str_t search_str) {
    if (str.size < search_str.size) return false;
    char* cptr = str.data;
    for (grv_str_size_t i = 0; i <= str.size - search_str.size; ++i) {
        if (0 == memcmp(cptr++, search_str.data, search_str.size)) return true;
    }
    return false;
}

bool grv_str_contains_char(grv_str_t str, char c) {
    for (grv_str_size_t i = 0; i < str.size; ++i) {
        if (str.data[i] == c) return true;
    }
    return false;
}

bool grv_str_contains_char_insensitive(grv_str_t str, char c) {
    for (grv_str_size_t i = 0; i < str.size; ++i) {
        if (grv_char_to_lower(str.data[i]) == grv_char_to_lower(c)) return true;
    }
    return false;
}

grv_str_t grv_str_reduce_char_spans(grv_str_t str, char c) {
    grv_str_t res = grv_str_copy(str);
    char* r = str.data;
    char* w = res.data;
    res.size = 0;
    bool is_in_span = false;
    for (grv_str_size_t i = 0; i < str.size; ++i) {
        char ic = *r++;
        if (ic == c) {
            if (!is_in_span) {
                is_in_span = true;
                *w++ = ic;
                res.size++;
            }
        } else {
            is_in_span = false;
            *w++ = ic;
            res.size++;
        }
    }
    return res;
}

bool grv_str_eq_str(grv_str_t a, grv_str_t b) {
    if (a.size != b.size) return false;
    return memcmp(a.data, b.data, a.size) == 0;
}

int grv_char_to_int(char c) {
    return (int)(c - '0');
}

grv_str_t grv_str_to_lower(grv_str_t str) {
    grv_str_t res = grv_str_copy(str);
    for (grv_str_size_t i = 0; i < str.size; ++i) {
         res.data[i] = grv_char_to_lower(str.data[i]);
    }
    return res;
}

grv_str_t grv_str_to_upper(grv_str_t str) {
    grv_str_t res = grv_str_copy(str);
    for (grv_str_size_t i = 0; i < str.size; ++i) {
         res.data[i] = grv_char_to_upper(str.data[i]);
    }
    return res;
}

bool grv_is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool grv_is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool grv_is_word_separator(char c) {
    static grv_str_t separators = {.data=" \t,;.:\"'(){}*+~=#|            ", .size=32};
    return grv_str_contains_char(separators, c);
}

bool grv_is_newline(char c) {
    return c == '\n';
}

char grv_str_at(grv_str_t str, int idx) {
    assert(idx < str.size);
    return str.data[idx];
}

bool grv_str_is_int(grv_str_t str) {
    if (str.size == 0) return false;
    if (str.size == 1) return grv_is_digit(str.data[0]);
    bool first_char = true;
    for (i32 i = 0; i < str.size; ++i) {
        char c = grv_str_at(str, i);
        bool valid_char = grv_is_digit(c) || (first_char && (c == '+' || c == '-'));
        if (!valid_char) return false;
        first_char = false;
    }
    return true;
}

int grv_str_to_int(grv_str_t str) {
    int res = 0;
    bool is_negative = false;
    bool is_first_char = true;
    bool found_digit = false;
    for (grv_str_size_t i = 0; i < str.size; ++i) {
        char c = grv_str_at(str, i);        
        if (c == '+') {
            assert(is_first_char);
            is_first_char = false;
        } else if (c == '-') {
            assert(is_first_char);
            is_negative = true;
            is_first_char = false;
        } else if (grv_is_digit(c)) {
            found_digit = true;
            is_first_char = false;
            res = 10 * res + grv_char_to_int(c);
        } else {
            assert(found_digit);
            break;
        }
    }
    return is_negative ? -res : res;
}

i64 grv_str_to_i64(grv_str_t str) {
    int res = 0;
    for (grv_str_size_t i = 0; i < str.size; ++i) {
        char c = grv_str_at(str, i);        
        assert(grv_is_digit(c));
        res = 10 * res + grv_char_to_int(c);
    }   
    return res;
}

bool grv_str_is_float(grv_str_t str) {
    enum {
        state_integer_part = 0,
        state_fractional_part = 1,
        state_exponent = 2
    } current_state = state_integer_part;   

    bool exponent_sign_encountered = false;
    bool digit_in_exponent_encountered = false;

    if (str.size == 0) return false;
    i32 idx = 0;
    if (str.data[0] == '-') {
        idx++;
    } else if (str.data[0] == '+') {
        idx++;
    } 

    while (idx < str.size) {
        char c = str.data[idx];
        if (c == '.') {
            if (current_state != state_integer_part) return false;
            current_state = state_fractional_part;
        } else if (c=='e' || c=='E') {
            if (current_state == state_exponent) return false;
            current_state = state_exponent;
        } else if (c=='-' || c=='+') {
            if (current_state != state_exponent || exponent_sign_encountered) return false;
            else if (current_state == state_exponent && digit_in_exponent_encountered) return false;
            exponent_sign_encountered = true;
        } else if (grv_is_digit(c)) {
            if (current_state == state_exponent) digit_in_exponent_encountered = true;
        } else {
            return false;
        }
        idx++;
    }

    return true;
}

f32 grv_str_to_f32(grv_str_t str) {
    grv_str_iter_t iter = grv_str_iter_begin(&str);
    return grv_str_iter_match_f32(&iter);
}

char grv_str_get_char(grv_str_t str, grv_str_size_t pos) { return str.data[pos]; }
char grv_str_iter_get_char(grv_str_iter_t* iter) { return iter->str->data[iter->pos]; }
void grv_str_iter_set_char(grv_str_iter_t* iter, char c) {
    assert(!grv_str_iter_is_end(iter));
    iter->str->data[iter->pos] = c;
}

void grv_str_iter_inc(grv_str_iter_t* iter, grv_str_size_t inc) { iter->pos+= inc; }
void grv_str_iter_dec(grv_str_iter_t* iter, grv_str_size_t dec) { iter->pos-= dec; }

grv_str_t grv_str_iter_get_line(grv_str_iter_t* iter) {
    grv_str_t line = grv_str_iter_match_up_to_char(iter, '\n');
    if (!grv_str_iter_is_end(iter)) {
        grv_str_iter_match_newline(iter);
    }
    return line;
}

grv_str_iter_t grv_str_find_str(grv_str_t* str, grv_str_t match_str) {
    if (match_str.size == 0 || str->size < match_str.size) return grv_str_iter_end(str);
    grv_str_iter_t iter = grv_str_iter_begin(str);
    while (iter.pos <= str->size - match_str.size) {
        if (_grv_str_iter_eq_str(&iter, match_str)) return iter;
        grv_str_iter_inc(&iter, 1);
    }
    return grv_str_iter_end(str);
}

grv_str_iter_t grv_str_rfind_str(grv_str_t* str, grv_str_t match_str) {
    if (match_str.size == 0 || str->size < match_str.size) return grv_str_iter_rend(str);
    grv_str_iter_t iter = {str, str->size - match_str.size};
    while (!grv_str_iter_is_rend(&iter)) {
        if (_grv_str_iter_eq_str(&iter, match_str)) return iter;
        grv_str_iter_dec(&iter, 1);
    }
    return grv_str_iter_rend(str);
}

grv_str_iter_t grv_str_find_char(grv_str_t* str, char c) {
    grv_str_iter_t iter = grv_str_iter_begin(str);
    while (!grv_str_iter_is_end(&iter)) {
        if (grv_str_iter_get_char(&iter) == c) return iter;
        grv_str_iter_inc(&iter, 1);
    }
    return iter;
}

grv_str_iter_t grv_str_rfind_char(grv_str_t* str, char c) {
    grv_str_iter_t iter = grv_str_iter_rbegin(str);
    while (!grv_str_iter_is_rend(&iter)) {
        if (grv_str_iter_get_char(&iter) == c) return iter;
        grv_str_iter_dec(&iter, 1);
    }
    return iter;
}

grv_str_iter_t grv_str_find_any_char(grv_str_t* str, grv_str_t chars) {
    grv_str_iter_t iter = grv_str_iter_begin(str);
    while (!grv_str_iter_is_end(&iter)) {
        char c = grv_str_iter_get_char(&iter);
        if (grv_str_contains_char(chars, c)) return iter;
        grv_str_iter_inc(&iter, 1);
    }
    return iter;
}

grv_str_t grv_str_split_head_at_char(grv_str_t str, char c) {
    grv_str_iter_t start_iter = grv_str_iter_begin(&str);
    grv_str_iter_t end_iter = grv_str_find_char(&str, c);
    return grv_str_substr_with_iters(start_iter, end_iter);
}

grv_str_t grv_str_split_tail_at_char(grv_str_t str, char c) {
    grv_str_iter_t iter = grv_str_find_char(&str, c);
    // the char is not in the string
    if (grv_str_iter_is_end(&iter)) return grv_str_ref("");
    grv_str_iter_inc(&iter, 1);
    // the char is the last char in the string
    if (grv_str_iter_is_end(&iter)) return grv_str_ref("");
    grv_str_t result = {
        .data=str.data + iter.pos,
        .size=str.size - iter.pos,
        .is_valid=true,
        .owns_data=false
    };
    return result;
}

struct grv_strarr_t grv_str_split(grv_str_t str, grv_str_t sep) {
    grv_strarr_t arr = grv_strarr_new();
    grv_str_iter_t start_iter = grv_str_iter_begin(&str);
    grv_str_iter_t end_iter = start_iter;
    while (!grv_str_iter_is_end(&end_iter)) {
        grv_str_iter_match_up_to_str(&end_iter, sep);
        grv_strarr_push(&arr, grv_str_substr_with_iters(start_iter, end_iter));
        if (grv_str_iter_is_end(&end_iter)) break;
        end_iter.pos += sep.size;
        if (grv_str_iter_is_end(&end_iter)) grv_strarr_push(&arr, grv_str_ref(""));
        start_iter = end_iter; 
    }
    return arr;
}

struct grv_strarr_t grv_str_split_whitespace(grv_str_t str) {
    grv_strarr_t arr = grv_strarr_new();
    grv_str_iter_t start_iter = grv_str_iter_begin(&str);
    grv_str_iter_t end_iter = start_iter;
    while (!grv_str_iter_is_end(&end_iter)) {
        grv_str_t substr = grv_str_iter_match_up_to_whitespace(&end_iter);
        grv_strarr_push(&arr, substr);
        if (grv_str_iter_is_end(&end_iter)) break;
        grv_str_iter_match_up_to_non_whitespace(&end_iter);
        if (grv_str_iter_is_end(&end_iter)) grv_strarr_push(&arr, grv_str_ref(""));
        start_iter = end_iter; 
    }
    return arr;
}

grv_str_t grv_str_split_tail_from_front(grv_str_t str, grv_str_t sep) {
    grv_str_iter_t start_iter = grv_str_find_str(&str, sep);
    if (grv_str_iter_is_end(&start_iter)) return grv_str_ref("");
    grv_str_iter_inc(&start_iter, sep.size);
    grv_str_iter_t end_iter = grv_str_iter_end(&str);
    return grv_str_substr_with_iters(start_iter, end_iter);
}

bool grv_str_iter_match(grv_str_iter_t* iter, grv_str_t match_str) {
    bool match = _grv_str_iter_eq_str(iter, match_str);
    if (match) {
        iter->pos += match_str.size;
        return true;
    } else {
        return false;
    } 
}

bool grv_str_iter_rmatch(grv_str_iter_t* iter, grv_str_t match_str) {
    if (iter->pos < match_str.size) return false;
    bool match = (memcmp(iter->str->data + iter->pos - match_str.size + 1, match_str.data, match_str.size) == 0);
    if (match) {
        iter->pos -= match_str.size;
        return true;
    } else {
        return false;
    } 
}

int grv_str_iter_match_int(grv_str_iter_t* iter) {
    int res = 0;
    if (grv_str_iter_is_whitespace(iter)) {
        grv_str_iter_match_white_space(iter);
    }
    while (!grv_str_iter_is_end(iter)) {
        char c = grv_str_iter_get_char(iter);
        if (grv_is_digit(c)) {
            res = 10 * res + grv_char_to_int(c);
            grv_str_iter_inc(iter, 1);
        } else {
          break;
        }
    }    
    return res;
}

i64 grv_str_iter_match_i64(grv_str_iter_t* iter) {
    i64 res = 0;
    i64 sign = 1;
    if (grv_str_iter_is_whitespace(iter)) {
        grv_str_iter_match_white_space(iter);
    }
    char c = grv_str_iter_get_char(iter);
    if (c == '-') {
        sign = -1;
        grv_str_iter_inc(iter, 1);
    } else if (c == '+') {
        grv_str_iter_inc(iter, 1);
    }
    while (!grv_str_iter_is_end(iter)) {
        char c = grv_str_iter_get_char(iter);
        if (grv_is_digit(c)) {
            res = 10 * res + grv_char_to_int(c);
            grv_str_iter_inc(iter, 1);
        } else {
          break;
        }
    }    
    return sign * res;
}

f32 grv_str_iter_match_f32(grv_str_iter_t* iter) {
    enum {
        state_integer_part = 0,
        state_fractional_part = 1,
        state_exponent = 2
    } current_state = state_integer_part;   

    f32 sign_mantissa = 1.0f;
    f32 sign_exponent = 1.0f;
    f32 result = 0.0f;
    f32 integer_part = 0.0f;
    f32 fractional_part = 0.0f;
    i32 fractional_position = 0;
    f32 exponent = 0.0f;
    bool exponent_sign_encountered = false;

    if (grv_str_iter_is_end(iter)) return result;

    if (grv_str_iter_get_char(iter) == '-') {
        sign_mantissa = -1.0f;
        grv_str_iter_inc(iter, 1);
    } else if (grv_str_iter_get_char(iter) == '+') {
        grv_str_iter_inc(iter, 1);
    } 

    while (!grv_str_iter_is_end(iter)) {
        char c = grv_str_iter_get_char(iter);
        if (c == '.') {
            if (current_state != state_integer_part) break;
            current_state = state_fractional_part;
        } else if (c=='e' || c=='E') {
            if (current_state == state_exponent) break;
            current_state = state_exponent;
        } else if (c=='-') {
            if (current_state != state_exponent || exponent_sign_encountered) break; 
            exponent_sign_encountered = true;
            sign_exponent = -1.0f;
        } else if (c=='+') {
            if (current_state != state_exponent || exponent_sign_encountered) break; 
            exponent_sign_encountered = true;
        } else if (grv_is_digit(c)) {
            f32 digit = (f32)grv_char_to_int(c);
            if (current_state == state_integer_part) {
                integer_part = 10.0f * integer_part + digit;
            } else if (current_state == state_fractional_part) {
                fractional_position++;
                fractional_part += digit * powf(10.0f, -fractional_position);
            } else if (current_state == state_exponent) {
                exponent = 10.0f * exponent + digit;
            }
        } else {
            break;        
        }
        grv_str_iter_inc(iter, 1);
    }

    return sign_mantissa * (integer_part + fractional_part) * powf(10.0f, sign_exponent * exponent);
}

bool grv_str_iter_match_white_space(grv_str_iter_t* iter) {
    if (grv_str_iter_is_end(iter)) return false;
    char c = grv_str_iter_get_char(iter);
    if (!grv_is_whitespace(c)) return false;
    grv_str_iter_inc(iter, 1);
    while(!grv_str_iter_is_end(iter)) {
        c = grv_str_iter_get_char(iter);
        if (!grv_is_whitespace(c)) break;
        grv_str_iter_inc(iter, 1);
    }
    return true;
}

bool grv_str_iter_match_char(grv_str_iter_t* iter, char match) {
    if (grv_str_iter_is_end(iter)) return false;
    if (grv_str_iter_get_char(iter) == match) {
        grv_str_iter_inc(iter, 1);
        return true;
    } else {
        return false;
    }
}

bool grv_str_iter_match_any_char(grv_str_iter_t* iter, grv_str_t chars) {
    if (grv_str_iter_is_end(iter)) return false;
    if (grv_str_contains_char(chars, grv_str_iter_get_char(iter))) {
        grv_str_iter_inc(iter, 1);
        return true;
    } else {
        return false;
    }
}

bool grv_str_iter_match_str(grv_str_iter_t* iter, grv_str_t match_str) {
    if (grv_str_iter_is_end(iter) || iter->pos + match_str.size > iter->str->size) return false;
    if (memcmp(iter->str->data + iter->pos, match_str.data, match_str.size) == 0) {
        iter->pos += match_str.size;
        return true;
    } else {
        return false;
    }
}

bool grv_str_iter_match_newline(grv_str_iter_t* iter) {
    return grv_str_iter_match_char(iter, '\n');
}

bool grv_str_iter_is_digit(grv_str_iter_t* iter) {
    if (grv_str_iter_is_end(iter)) return false;
    return grv_is_digit(grv_str_iter_get_char(iter));
}

bool grv_str_iter_is_whitespace(grv_str_iter_t* iter) {
    if (grv_str_iter_is_end(iter)) return false;
    return grv_is_whitespace(grv_str_iter_get_char(iter));
}

grv_str_t grv_str_iter_match_up_to_char(grv_str_iter_t* iter, char match) {
    grv_str_t r = {
        .data=iter->str->data + iter->pos,
        .size=0,
        .is_valid=true,
        .owns_data=false
    };

    while (!grv_str_iter_is_end(iter)) {
        char c = grv_str_iter_get_char(iter);

        if (c == match) {
            return r;
        } else {
            r.size++;
            grv_str_iter_inc(iter, 1);
        }
    }
    return r;
}

grv_str_t grv_str_iter_match_up_to_whitespace(grv_str_iter_t* iter) {
    grv_str_t r = {
        .data=iter->str->data + iter->pos,
        .size=0,
        .is_valid=true,
        .owns_data=false
    };

    while (!grv_str_iter_is_end(iter)) {
        char c = grv_str_iter_get_char(iter);

        if (grv_is_whitespace(c)) {
            return r;
        } else {
            r.size++;
            grv_str_iter_inc(iter, 1);
        }
    }
    return r;
}

grv_str_t grv_str_iter_match_up_to_non_whitespace(grv_str_iter_t* iter) {
    grv_str_t r = {
        .data=iter->str->data + iter->pos,
        .size=0,
        .is_valid=true,
        .owns_data=false
    };

    while (!grv_str_iter_is_end(iter)) {
        char c = grv_str_iter_get_char(iter);

        if (!grv_is_whitespace(c)) {
            return r;
        } else {
            r.size++;
            grv_str_iter_inc(iter, 1);
        }
    }
    return r;
}

grv_str_t grv_str_iter_match_up_to_str(grv_str_iter_t* iter, grv_str_t match) {
    grv_str_t r = {
        .data=iter->str->data + iter->pos,
        .size=0,
        .is_valid=true,
        .owns_data=false
    };

    while (!grv_str_iter_is_end(iter)) {
        if (grv_str_iter_eq_str(iter, match)) {
            return r;
        } 
        grv_str_iter_inc(iter, 1);
        r.size++;
    }

    return r;
}

grv_str_t grv_str_iter_match_word(grv_str_iter_t* iter) {
    grv_str_t r = {
        .data=iter->str->data + iter->pos,
        .size=0,
        .is_valid=true,
        .owns_data=false
    };

    while (!grv_str_iter_is_end(iter)) {
        char c = grv_str_iter_get_char(iter);
        if (grv_is_word_separator(c)) {
            return r;
        } 
        grv_str_iter_inc(iter, 1);
        r.size++;
    }
    return r;
}

bool grv_str_iter_eq_str(grv_str_iter_t* iter, grv_str_t match_str) {
    if (grv_str_iter_is_end(iter) || iter->pos + match_str.size > iter->str->size) return false;
    return memcmp(iter->str->data + iter->pos, match_str.data, match_str.size) == 0;
}

#if 0
int_arr_t str_iter_match_int_list(grv_str_iter_t* iter) {
    int_arr_t r = int_arr_create();

    while (!grv_str_iter_is_end(iter)) {
        grv_str_iter_match_white_space(iter);
        grv_str_iter_match_any_char(iter, grv_str_ref(","));
        grv_str_iter_match_white_space(iter);
        char c = grv_str_iter_get_char(iter);
        if (!grv_is_digit(c) && c!='-' && c!='+') break; 
        i64 v = grv_str_iter_match_i64(iter);
        int_arr_push(&r, v);
    }
    return r;
}

#endif

grv_str_t grv_str_new(char* cstr) {
    grv_str_t res = {0};
    grv_str_size_t len = strlen(cstr);
    grv_str_size_t capacity = _grv_str_capacity_for_size(len);
    res.data = grv_alloc(capacity);
    assert(res.data);
    res.size = len;
    res.is_valid = true;
    res.owns_data = true;
    memcpy(res.data, cstr, len);
    return res;
}

grv_str_t grv_str_new_with_capacity(grv_str_size_t capacity) {
    capacity = _grv_str_capacity_for_size(capacity);
    return (grv_str_t) {.data=grv_alloc(capacity), .size=0, .is_valid=true, .owns_data=true};
}

grv_str_t grv_str_copy(grv_str_t str) {
    grv_str_t res = grv_str_new_with_capacity(str.size);
    res.size = str.size;
    memcpy(res.data, str.data, str.size);
    return res;
}

grv_str_t grv_str_new_with_format(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    char* str = _get_cstr_buffer(len+1);
    va_start(args, fmt);
    vsnprintf(str, len + 1, fmt, args);
    va_end(args);
    grv_str_t res = grv_str_new(str);
    return res;
}

void grv_str_free(grv_str_t* str) {
    if (str->owns_data && str->data) grv_free(str->data);
    *str = (grv_str_t){0};
}

grv_str_t grv_str_cat_str_str(grv_str_t a, grv_str_t b) {
    grv_str_t str = grv_str_new_with_capacity(a.size + b.size);
    str.size = a.size + b.size;
    memcpy(str.data, a.data, a.size);
    memcpy(str.data + a.size, b.data, b.size);
    return str;
}

void grv_str_grow(grv_str_t* str, grv_str_size_t new_size) {
    grv_str_size_t new_capacity = _grv_str_capacity_for_size(new_size);
    if (str->owns_data) { 
        assert(new_size > str->size);
        str->data = grv_realloc(str->data, new_capacity);
    } else {
        char* new_data = grv_alloc(new_capacity);
        memcpy(new_data, str->data, str->size);
        str->data = new_data;
        str->owns_data = true;
    }   
}

void grv_str_append_str(grv_str_t* str, grv_str_t append_str) {
    if (append_str.size == 0) return;
    grv_str_size_t new_size = str->size + append_str.size;
    grv_str_grow(str, new_size);
    memcpy(str->data + str->size, append_str.data, append_str.size);
    str->size = new_size;
}

void grv_str_append_space(grv_str_t* str) {
    grv_str_append_char(str, ' ');
}

void grv_str_append_char(grv_str_t* str, char c) {
    grv_str_size_t new_size = str->size + 1;
    grv_str_grow(str, new_size);
    str->data[str->size] = c;
    str->size = new_size;
}

void grv_str_prepend_str(grv_str_t* str, grv_str_t prepend_str) {
    grv_str_size_t new_size = str->size + prepend_str.size;
    grv_str_grow(str, new_size);
    memmove(str->data + prepend_str.size, str->data, str->size);
    memcpy(str->data, prepend_str.data, prepend_str.size);
    str->size = new_size;
}

void grv_str_prepend_char(grv_str_t* str, char c) {
    grv_str_size_t new_size = str->size + 1;
    grv_str_grow(str, new_size);
    memmove(str->data + 1, str->data, str->size);
    str->data[0] = c;
    str->size = new_size;
}


grv_str_t grv_str_from_int(i32 i) {
    char* buffer = _get_cstr_buffer(32);
    sprintf(buffer, "%d", i);
    return grv_str_new(buffer);
}

grv_str_t grv_str_from_i64(i64 i) {
    char* buffer = _get_cstr_buffer(64);
    sprintf(buffer, "%ld", i);
    return grv_str_new(buffer);
}

grv_str_t grv_str_from_u64(u64 i) {
    char* buffer = _get_cstr_buffer(32);
    sprintf(buffer, "%ld", i);
    return grv_str_new(buffer);
}

grv_str_t str_from_f32(f32 f) {
    char* buffer = _get_cstr_buffer(32);
    sprintf(buffer, "%f", f);
    return grv_str_new(buffer);
}

