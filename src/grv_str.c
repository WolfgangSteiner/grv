#include "grv/grv_str.h"
#include "grv/grv_base.h"
#include "grv/grv_common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define GRV_STR_MIN_CAPACITY_16
#define GRV_STR_ALLOC_GRANULARITY 16

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
    capacity = max_s64(capacity, 1024);
    if (str_cstr_buffer.data == 0) {
        str_cstr_buffer.capacity = capacity;
        str_cstr_buffer.data = malloc(str_cstr_buffer.capacity);
    } 
    return str_cstr_buffer.data;
}

grv_str_t grv_str_ref(char* cstr) {
    return (grv_str_t){ 
        .data=cstr,
        .size=strlen(cstr),
        .is_valid=true,
        .owns_data=false}; 
}

grv_str_t grv_str_copy(grv_str_t str) {
    grv_str_t r = {
        .data=malloc(str.size),
        .size=str.size,
        .is_valid=str.is_valid,
        .owns_data=true};
    memcpy(r.data, str.data, str.size);
    return r;
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
        str_cstr_buffer.data = calloc(str_cstr_buffer.capacity, 1);
    } 
    
    if (str_cstr_buffer.capacity < str.size + 1) {
        while (str_cstr_buffer.capacity < str.size + 1) str_cstr_buffer.capacity *= 2;
        str_cstr_buffer.data = realloc(str_cstr_buffer.data, str_cstr_buffer.capacity);
    }

    memcpy(str_cstr_buffer.data, str.data, str.size);
    str_cstr_buffer.data[str.size] = '\0';
    return str_cstr_buffer.data;
}

char* grv_str_copy_to_cstr(grv_str_t str) {
    char* cstr = malloc(str.size + 1);
    memcpy(cstr, str.data, str.size);
    cstr[str.size] = '\0';
    return cstr;
}   

grv_str_t str_substr(grv_str_t str, grv_str_size_t start, grv_str_size_t length) {
    grv_str_size_t size = length > 0 ? length : str.size - start + length + 1;
    return (grv_str_t){ 
        .data=str.data + start,
        .size=size,
        .is_valid=str.is_valid,
        .owns_data=false};
}

grv_str_t grv_str_lstrip(grv_str_t str) {
    grv_str_t res = str;
    res.owns_data = false;
    while (res.size && grv_is_white_space(res.data[0])) {
        res.data++;
        res.size--;
    }
    return res;
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

grv_str_t grv_read_file(grv_str_t file_name) {
    grv_str_t result = {0};
    FILE* fp = fopen(grv_str_cstr(file_name), "rb");
    if (fp == 0) return result;
    int fseek_result = fseek(fp, 0L, SEEK_END);
    if (fseek_result) return result;
    result.size = ftell(fp);
    rewind(fp);
    result.data = malloc(result.size);
    fread(result.data, result.size, sizeof(char), fp);
    fclose(fp);
    result.is_valid = true;
    result.owns_data = true;
    return result;
}

bool grv_file_exists(grv_str_t file_name) {
    FILE* file = fopen(grv_str_cstr(file_name), "rb");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

bool grv_str_contains(grv_str_t str, grv_str_t search_str) {
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

bool grv_str_eq(grv_str_t a, grv_str_t b) {
    if (a.size != b.size) return false;
    return memcmp(a.data, b.data, a.size) == 0;
}

int grv_char_to_int(char c) {
    return (int)(c - '0');
}

bool grv_is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool grv_is_white_space(char c) {
    return c == ' ' || c == '\t';
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

int grv_str_to_int(grv_str_t str) {
    int res = 0;
    for (grv_str_size_t i = 0; i < str.size; ++i) {
        char c = grv_str_at(str, i);        
        assert(grv_is_digit(c));
        res = 10 * res + grv_char_to_int(c);
    }   
    return res;
}

s64 grv_str_to_s64(grv_str_t str)
{
    int res = 0;
    for (grv_str_size_t i = 0; i < str.size; ++i) {
        char c = grv_str_at(str, i);        
        assert(grv_is_digit(c));
        res = 10 * res + grv_char_to_int(c);
    }   
    return res;
}

char grv_str_get_char(grv_str_t str, grv_str_size_t pos) { return str.data[pos]; }
char grv_str_iter_get_char(grv_str_iter_t* iter) { return iter->str->data[iter->pos]; }
void grv_str_iter_set_char(grv_str_iter_t* iter, char c) {
    assert(!grv_str_iter_is_end(iter));
    iter->str->data[iter->pos] = c;
}

void grv_str_iter_inc(grv_str_iter_t* iter) { iter->pos++; }
void grv_str_iter_dec(grv_str_iter_t* iter) { iter->pos--; }

grv_str_iter_t grv_str_find_char(grv_str_t* str, char c) {
    grv_str_iter_t iter = grv_str_iter_begin(str);
    while (!grv_str_iter_is_end(&iter)) {
        if (grv_str_iter_get_char(&iter) == c) return iter;
        grv_str_iter_inc(&iter);
    }
    return iter;
}

grv_str_iter_t grv_str_find_char_from_back(grv_str_t* str, char c) {
    grv_str_iter_t iter = grv_str_iter_rbegin(str);
    while (!grv_str_iter_is_rend(&iter)) {
        if (grv_str_iter_get_char(&iter) == c) return iter;
        grv_str_iter_dec(&iter);
    }
    return iter;
}

grv_str_iter_t grv_str_find_any_char(grv_str_t str, grv_str_t chars) {
    grv_str_iter_t iter = grv_str_iter_begin(&str);
    while (!grv_str_iter_is_end(&iter)) {
        char c = grv_str_iter_get_char(&iter);
        if (grv_str_contains_char(chars, c)) return iter;
        grv_str_iter_inc(&iter);
    }
    return iter;
}

grv_str_t grv_str_split_tail_at_char(grv_str_t str, char c) {
    grv_str_iter_t iter = grv_str_find_char(&str, c);
    // the char is not in the string
    if (grv_str_iter_is_end(&iter)) return grv_str_ref("");
    grv_str_iter_inc(&iter);
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

bool grv_str_iter_match(grv_str_iter_t* iter, grv_str_t match_str) {
    if (iter->pos + match_str.size > iter->str->size) return false;
    bool match = (memcmp(iter->str->data + iter->pos, match_str.data, match_str.size) == 0);
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
    if (grv_str_iter_is_white_space(iter)) {
        grv_str_iter_match_white_space(iter);
    }
    while (!grv_str_iter_is_end(iter)) {
        char c = grv_str_iter_get_char(iter);
        if (grv_is_digit(c)) {
            res = 10 * res + grv_char_to_int(c);
            grv_str_iter_inc(iter);
        } else {
          break;
        }
    }    
    return res;
}

s64 grv_str_iter_match_s64(grv_str_iter_t* iter) {
    s64 res = 0;
    s64 sign = 1;
    if (grv_str_iter_is_white_space(iter)) {
        grv_str_iter_match_white_space(iter);
    }
    char c = grv_str_iter_get_char(iter);
    if (c == '-') {
        sign = -1;
        grv_str_iter_inc(iter);
    } else if (c == '+') {
        grv_str_iter_inc(iter);
    }
    while (!grv_str_iter_is_end(iter)) {
        char c = grv_str_iter_get_char(iter);
        if (grv_is_digit(c)) {
            res = 10 * res + grv_char_to_int(c);
            grv_str_iter_inc(iter);
        } else {
          break;
        }
    }    
    return sign * res;
}

bool grv_str_iter_match_white_space(grv_str_iter_t* iter) {
    bool matched = false;
    while (!grv_str_iter_is_end(iter)) {
        char c = grv_str_iter_get_char(iter);
        if (grv_is_white_space(c)) {
            grv_str_iter_inc(iter);
            matched = true;
        } else {
            break;
        }
    }
    return matched;
}

bool grv_str_iter_match_char(grv_str_iter_t* iter, char match) {
    if (grv_str_iter_is_end(iter)) return false;
    if (grv_str_iter_get_char(iter) == match) {
        grv_str_iter_inc(iter);
        return true;
    } else {
        return false;
    }
}

bool grv_str_iter_match_any_char(grv_str_iter_t* iter, grv_str_t chars) {
    if (grv_str_iter_is_end(iter)) return false;
    if (grv_str_contains_char(chars, grv_str_iter_get_char(iter))) {
        grv_str_iter_inc(iter);
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

bool grv_str_iter_is_white_space(grv_str_iter_t* iter) {
    if (grv_str_iter_is_end(iter)) return false;
    return grv_is_white_space(grv_str_iter_get_char(iter));
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
        grv_str_iter_inc(iter);

        if (c == match) {
            return r;
        } else {
            r.size++;
        }
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
        grv_str_iter_inc(iter);
        r.size++;
    }
    return r;
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
        s64 v = grv_str_iter_match_s64(iter);
        int_arr_push(&r, v);
    }
    return r;
}

#endif

grv_str_t grv_str_new(char* cstr) {
    grv_str_t res = {0};
    grv_str_size_t len = strlen(cstr);
    grv_str_size_t capacity = _grv_str_capacity_for_size(len);
    res.data = malloc(capacity);
    assert(res.data);
    res.size = len;
    res.is_valid = true;
    res.owns_data = true;
    memcpy(res.data, cstr, len);
    return res;
}

grv_str_t grv_str_new_with_capacity(grv_str_size_t capacity) {
    grv_str_t str = {0};
    capacity = _grv_str_capacity_for_size(capacity);
    str.data = calloc(capacity, 1);
    str.is_valid = true; 
    str.owns_data = true;
    return str;
}

void grv_str_free(grv_str_t* str) {
    if (str->owns_data) free(str->data);
    *str = (grv_str_t){0};
}

grv_str_t grv_str_cat(grv_str_t a, grv_str_t b) {
    grv_str_t str = grv_str_new_with_capacity(a.size + b.size);
    str.size = a.size + b.size;
    memcpy(str.data, a.data, a.size);
    memcpy(str.data + a.size, b.data, b.size);
    return str;
}

void str_grow(grv_str_t* str, grv_str_size_t new_size) {
    if (str->owns_data) { 
        assert(new_size > str->size);
        grv_str_size_t old_capacity = _grv_str_capacity_for_size(str->size);
        grv_str_size_t new_capacity = _grv_str_capacity_for_size(new_size);
        if (new_capacity > old_capacity) str->data = realloc(str->data, new_capacity);
    } else {
        grv_str_size_t new_capacity = _grv_str_capacity_for_size(new_size);
        char* new_data = calloc(new_capacity, 1);
        memcpy(new_data, str->data, str->size);
        str->data = new_data;
        str->owns_data = true;
    }   
}

void grv_str_append(grv_str_t* str, grv_str_t append_str) {
    grv_str_size_t new_size = str->size + append_str.size;
    str_grow(str, new_size);
    memcpy(str->data + str->size, append_str.data, append_str.size);
    str->size = new_size;
}

void grv_str_append_space(grv_str_t* str) {
    grv_str_append_char(str, ' ');
}

void grv_str_append_char(grv_str_t* str, char c) {
    grv_str_size_t new_size = str->size + 1;
    str_grow(str, new_size);
    str->data[str->size] = c;
    str->size = new_size;
}

void grv_str_prepend(grv_str_t* str, grv_str_t prepend_str) {
    grv_str_size_t new_size = str->size + prepend_str.size;
    str_grow(str, new_size);
    memmove(str->data + prepend_str.size, str->data, str->size);
    memcpy(str->data, prepend_str.data, prepend_str.size);
    str->size = new_size;
}

void grv_str_prepend_char(grv_str_t* str, char c) {
    grv_str_size_t new_size = str->size + 1;
    str_grow(str, new_size);
    memmove(str->data + 1, str->data, str->size);
    str->data[0] = c;
    str->size = new_size;
}


grv_str_t grv_str_from_int(grv_str_size_t i) {
    char* buffer = _get_cstr_buffer(32);
    sprintf(buffer, "%d", i);
    return grv_str_new(buffer);
}

grv_str_t grv_str_from_s64(s64 i) {
    char* buffer = _get_cstr_buffer(32);
    sprintf(buffer, "%ld", i);
    return grv_str_new(buffer);
}

grv_str_t str_from_f32(f32 f) {
    char* buffer = _get_cstr_buffer(32);
    sprintf(buffer, "%f", f);
    return grv_str_new(buffer);
}