#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "grv/grv_base.h"
#include "grv/grv_str.h"
#include "grv/grv_strarr.h"
#include "grv/grv_cstr.h"
#include "grv/grv_memory.h"
#include "grv/grv_log.h"

typedef struct {
    grv_str_t key;
    grv_str_format_callback_t callback;
} str_format_pattern_t;

typedef struct {
    str_format_pattern_t* data;
    grv_str_size_t size, capacity;
} str_format_pattern_list_t;

static str_format_pattern_list_t str_format_patterns = {0};

void _str_format_color(grv_str_t* str, grv_str_t color_format) {
    grv_assert(grv_str_starts_with_cstr(color_format, "color="));
    grv_str_t color_str = grv_str_substr(color_format, 6, -1); 
    grv_str_t esc = {0};
    if (grv_str_eq_cstr(color_str, "red")) {
        esc = grv_str_ref("\033[31m");
    } else if (grv_str_eq_cstr(color_str, "green")) {
        esc = grv_str_ref("\033[32m");
    } else if (grv_str_eq_cstr(color_str, "yellow")) {
        esc = grv_str_ref("\033[33m");
    } else if (grv_str_eq_cstr(color_str, "blue")) {
        esc = grv_str_ref("\033[34m");
    } else if (grv_str_eq_cstr(color_str, "magenta")) {
        esc = grv_str_ref("\033[35m");
    } else if (grv_str_eq_cstr(color_str, "cyan")) {
        esc = grv_str_ref("\033[36m");
    } else if (grv_str_eq_cstr(color_str, "white")) {
        esc = grv_str_ref("\033[37");
    } else {
        grv_assert(false);
    }
    if (!grv_str_empty(esc)) {
        grv_str_prepend(str, esc);
        grv_str_append(str, grv_str_ref("\033[0m"));
    }
}

grv_str_t _str_format_callback_str(va_list* args, grv_str_t specifier) {
    (void) specifier;
    grv_str_t arg = va_arg(*args, grv_str_t);
    grv_str_t str = grv_str_copy(arg);
    if (specifier.size) {
        grv_strarr_t specifier_arr = grv_str_split(specifier, grv_str_ref(","));
        for (size_t i = 0; i < specifier_arr.size; ++i) {
            grv_str_t s = specifier_arr.arr[i];
            if (grv_str_starts_with_cstr(s, "color=")) {
                _str_format_color(&str, s);
            }
        }
        grv_strarr_free(&specifier_arr);
    }
    return str;
}   

grv_str_t _str_format_callback_cstr(va_list* args, grv_str_t specifier) {
    (void) specifier;
    char* arg = va_arg(*args, char*);
    return grv_str_new(arg);
}

grv_str_t _str_format_callback_u16(va_list* args, grv_str_t specifier) {
    // u16 is promoted to int when passed to va_arg
    int arg = va_arg(*args, int);

    if (grv_str_eq(specifier, grv_str_ref("hex"))) {
        char* formatted_cstr = grv_cstr_new_with_format("%x", arg);
        grv_str_t result = grv_str_new(formatted_cstr);
        free(formatted_cstr);
        return result;
    } else {
        return grv_str_from_int(arg);
    }
}

grv_str_t _str_format_callback_int(va_list* args, grv_str_t specifier) {
    int arg = va_arg(*args, int);
    if (grv_str_eq(specifier, grv_str_ref("hex"))) {
        char* formatted_cstr = grv_cstr_new_with_format("%x", arg);
        grv_str_t result = grv_str_new(formatted_cstr);
        free(formatted_cstr);
        return result;
    } else if (!grv_str_empty(specifier)) {
        char* spec_str = grv_str_copy_to_cstr(specifier);
        char* fmt_str = grv_cstr_new_with_format("%%%sd", spec_str);
        char* formatted_cstr = grv_cstr_new_with_format(fmt_str, arg);
        grv_str_t result = grv_str_new(formatted_cstr);
        grv_free(spec_str);
        grv_free(fmt_str);
        grv_free(formatted_cstr);
        return result;
    } else {
        return grv_str_from_int(arg);
    }
}

grv_str_t _str_format_callback_i64(va_list* args, grv_str_t specifier) {
    if (grv_str_eq(specifier, grv_str_ref("hex"))) {
        i64 arg = va_arg(*args, i64);
        char* formatted_cstr = grv_cstr_new_with_format("%lx", arg);
        grv_str_t result = grv_str_new(formatted_cstr);
        free(formatted_cstr);
        return result;
    } else {
        i64 arg = va_arg(*args, i64);
        return grv_str_from_i64(arg);
    }
}

grv_str_t _str_format_callback_u64(va_list* args, grv_str_t specifier) {
    if (grv_str_eq(specifier, grv_str_ref("hex"))) {
        u64 arg = va_arg(*args, u64);
        char* formatted_cstr = grv_cstr_new_with_format("%llx", arg);
        grv_str_t result = grv_str_new(formatted_cstr);
        free(formatted_cstr);
        return result;
    } else {
        u64 arg = va_arg(*args, u64);
        return grv_str_from_u64(arg);
    }
}

grv_str_t _str_format_callback_f32(va_list* args, grv_str_t specifier) {
    f32 arg = (f32)va_arg(*args, double);
    char* formatted_cstr = 0;
    if (!grv_str_eq(specifier, grv_str_ref(""))) {
        grv_str_iter_t iter = grv_str_iter_begin(&specifier);
        int width = grv_str_iter_match_int(&iter);
        grv_str_iter_match_char(&iter, '.');
        int precision = grv_str_iter_match_int(&iter);
        char* fmt_cstr = grv_cstr_new_with_format("%%%d.%df", width, precision); 
        formatted_cstr = grv_cstr_new_with_format(fmt_cstr, arg);
        free(fmt_cstr);
    } else {
        formatted_cstr = grv_cstr_new_with_format("%.3g", arg);
    }   
    grv_str_t result = grv_str_new(formatted_cstr);
    free(formatted_cstr);
    return result;
}

void _str_format_register_pattern(grv_str_t key, grv_str_format_callback_t callback) {
    str_format_patterns.data[str_format_patterns.size++] = (str_format_pattern_t){ key, callback };
}

void _str_format_initialize_pattern_list(void) {
    static bool first = true;
    if (first) {
        first = false;
        str_format_patterns.capacity = 32;
        str_format_patterns.size = 0;
        str_format_patterns.data = calloc(str_format_patterns.capacity, sizeof(str_format_pattern_t));
        _str_format_register_pattern(grv_str_ref("str"), _str_format_callback_str);
        _str_format_register_pattern(grv_str_ref("cstr"), _str_format_callback_cstr);
        _str_format_register_pattern(grv_str_ref("int"), _str_format_callback_int);
        _str_format_register_pattern(grv_str_ref("i64"), _str_format_callback_i64);
        _str_format_register_pattern(grv_str_ref("u64"), _str_format_callback_u64);
        _str_format_register_pattern(grv_str_ref("f32"), _str_format_callback_f32);
    }
}

str_format_pattern_list_t* _str_format_get_patterns(void) {
    _str_format_initialize_pattern_list();
    return &str_format_patterns;
}

void _str_format_add_pattern(grv_str_t pattern, grv_str_format_callback_t callback) {
    assert(str_format_patterns.data != 0);
    if (str_format_patterns.capacity == str_format_patterns.size) {
        str_format_patterns.capacity *= 2;
        str_format_patterns.data = realloc(str_format_patterns.data, str_format_patterns.capacity);
    }
    _str_format_register_pattern(pattern, callback);
}

str_format_pattern_t* _str_format_get_pattern_for_key(grv_str_t key) {
    str_format_pattern_list_t* patterns = _str_format_get_patterns();
    for (grv_str_size_t i = 0; i < patterns->size; ++i) {
        str_format_pattern_t* pattern = patterns->data + i;
        if (grv_str_eq(key, pattern->key)) {
            return pattern;
        }
    }
    return 0;
}

void grv_str_format_register_pattern(grv_str_t pattern, grv_str_format_callback_t callback) {
    str_format_pattern_t* existing_pattern = _str_format_get_pattern_for_key(pattern);
    if (existing_pattern) {
        existing_pattern->callback = callback;
    } else {
        _str_format_add_pattern(pattern, callback);
    }
}

grv_str_t _str_format_pattern_to_lookup_key(grv_str_t pattern) {
    // turn str:xxx into str
    if (grv_str_contains_char(pattern, ':')) {
        grv_str_iter_t iter = grv_str_iter_begin(&pattern);
        grv_str_t key = grv_str_iter_match_up_to_char(&iter, ':');
        return key;
    } else {
        return pattern;
    }
}

grv_str_t _str_format_pattern_get_format_specifier(grv_str_t pattern) {
    // turn str:xxx into xxx
    if (grv_str_contains_char(pattern, ':')) {
        return grv_str_split_tail_at_char(pattern, ':');
    } else {
        return grv_str_ref("");
    }
}

grv_str_format_callback_t _str_format_get_callback_for_key(grv_str_t key) {
    str_format_pattern_list_t* patterns = _str_format_get_patterns();
    for (int i = 0; i < patterns->size; ++i) {
        str_format_pattern_t* pattern = patterns->data + i;
        if (grv_str_eq(key, pattern->key)) {
            return pattern->callback;
        }
    }
    grv_log_error(grv_str_format(grv_str_ref("Unsupported format specifier {str}"), key));
    grv_assert(false);
    return 0;
}

grv_str_t _str_vformat(grv_str_t fmt, va_list* args) {
    // find patterns of form {TYPE}, where type is str, int, i64, f32
    // replace with value of type
    grv_str_iter_t iter = grv_str_iter_begin(&fmt);
    grv_str_t result = grv_str_new("");

    while (!grv_str_iter_is_end(&iter)) {
        if (grv_str_iter_match_char(&iter, '{')) {
            grv_str_t pattern = grv_str_iter_match_up_to_char(&iter, '}');
            grv_str_iter_match_char(&iter, '}');
            grv_str_t key = _str_format_pattern_to_lookup_key(pattern);
            grv_str_t specifier = _str_format_pattern_get_format_specifier(pattern);
            grv_str_format_callback_t callback = _str_format_get_callback_for_key(key);
            if (callback) {
                grv_str_t value = callback(args, specifier);
                grv_str_append(&result, value);
                grv_str_free(&value);
            } else {
                grv_str_append_char(&result, '{');
                grv_str_append(&result, pattern);
                grv_str_append_char(&result, '}');
            }
        } else {
            grv_str_append_char(&result, grv_str_iter_get_char(&iter));
            grv_str_iter_inc(&iter, 1);
        }
    }

    return result;
}

grv_str_t grv_str_format(grv_str_t fmt, ...) {
    va_list args;
    va_start(args, fmt);
    grv_str_t res = _str_vformat(fmt, &args);
    va_end(args);
    return res;
}

void grv_str_print_format(grv_str_t fmt, ...) {
    va_list args;
    va_start(args, fmt);
    grv_str_t res = _str_vformat(fmt, &args);
    grv_str_print(res);
    grv_str_free(&res);
    va_end(args);
}
