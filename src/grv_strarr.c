#include "grv/grv_arr.h"
#include "grv/grv_strarr.h"
#include <stdlib.h>
#include <string.h>

// grv_strarr
grv_strarr_t grv_strarr_new(void) {
    grv_strarr_t arr = {0};
    return arr;
}

grv_strarr_t grv_strarr_new_from_cstrarr(char** arr, grv_str_size_t size) {
    grv_strarr_t r = { .size=size, .capacity=size, .arr=grv_alloc(size * sizeof(grv_str_t)) };
    for (grv_str_size_t i = 0; i < size; i++) {
        r.arr[i] = grv_str_new(arr[i]);
    }
    return r;
} 

void grv_strarr_free(grv_strarr_t* a) {
    for (grv_str_size_t i = 0; i < a->size; i++) {
        grv_str_free(&a->arr[i]);
    }
    grv_arr_free(a);
}

void grv_strarr_clear(grv_strarr_t* a) {
    for (grv_str_size_t i = 0; i < a->size; i++) {
        grv_str_free(&a->arr[i]);
    }
    a->size = 0;
} 

void grv_strarr_push_cstr(grv_strarr_t* arr, char* s) {
    grv_str_t str = grv_str_new(s);
    grv_arr_push(arr, str);
}

grv_str_t* grv_strarr_push(grv_strarr_t* arr, grv_str_t s) {
    grv_arr_check_and_grow(arr);
    grv_arr_push(arr, s);
    return &arr->arr[arr->size - 1];
}

grv_str_t* grv_strarr_push_copy(grv_strarr_t* arr, grv_str_t s) {
    return grv_strarr_push(arr, grv_str_copy(s));
}

grv_str_t* grv_strarr_push_front(grv_strarr_t* arr, grv_str_t str) {
    grv_arr_push_front(arr, str);
    return &arr->arr[0];
}

void grv_strarr_append(grv_strarr_t* a, grv_strarr_t b) {
    for (grv_str_size_t i = 0; i < b.size; i++) {
        grv_str_t* s = &b.arr[i];
        if (s->owns_data) {
            grv_strarr_push(a, grv_str_copy(*s));
        } else {
            grv_arr_push(a, *s);
        }
    }
}

grv_str_t grv_strarr_get_copy(grv_strarr_t* arr, size_t idx) {
    return grv_str_copy(arr->arr[idx]);
} 

grv_str_t grv_strarr_pop_front(grv_strarr_t* arr) {
    assert(arr->size > 0);
    grv_str_t res = arr->arr[0];
    memmove(&arr->arr[0], &arr->arr[1], (arr->size - 1) * sizeof(grv_str_t));
    arr->size--;
    return res;
}

grv_str_t grv_strarr_pop_back(grv_strarr_t* arr) {
    assert(arr->size > 0);
    return arr->arr[--arr->size];
}

void grv_strarr_remove_front(grv_strarr_t* arr) {
    if (arr->size == 0) {
        return;
    }

    grv_str_free(&arr->arr[0]);
    for (grv_str_size_t i = 0; i < arr->size - 1; ++i) {
        arr->arr[i] = arr->arr[i + 1];
    }
    arr->size--;
}

void grv_strarr_remove_back(grv_strarr_t* arr) {
    if (arr->size == 0) {
        return;
    }
    grv_str_free(&arr->arr[arr->size - 1]);
    arr->size--;
}

grv_str_t grv_strarr_join(grv_strarr_t arr, grv_str_t join_str) {
    grv_str_t res = grv_str_copy(arr.arr[0]);
    for (grv_str_size_t i = 1; i < arr.size; ++i) {
        grv_str_append(&res, join_str);
        grv_str_append(&res, arr.arr[i]);
    }
    return res;
}

bool grv_strarr_contains_str(grv_strarr_t arr, grv_str_t str) {
    for (grv_str_size_t i = 0; i < arr.size; ++i) {
        if (grv_str_eq(arr.arr[i], str)) {
            return true;
        }
    }
    return false;
}

grv_strarr_t grv_strarr_filter(grv_strarr_t arr, bool (*predicate)(grv_str_t)) {
    grv_strarr_t result = grv_strarr_new();
    for (grv_str_size_t i = 0; i < arr.size; ++i) {
        if (predicate(arr.arr[i])) {
            grv_strarr_push_copy(&result, arr.arr[i]);
        }
    }
    return result;
}


i32 grv_strarr_index_of_str(grv_strarr_t arr, grv_str_t str) {
    for (i32 idx = 0; idx < arr.size; ++idx) {
         if (grv_str_eq_str(arr.arr[idx], str)) return idx;
    }

    return -1;
}

void grv_strarr_remove_str(grv_strarr_t* arr, grv_str_t str) {
    grv_assert(grv_strarr_contains(*arr, str));
    i32 idx = grv_strarr_index_of_str(*arr, str);
    grv_str_t remove_str = arr->arr[idx];
    grv_str_free(&remove_str);
    if (idx != arr->size - 1) {
        memmove(&arr->arr[idx], &arr->arr[idx + 1], (arr->size - idx - 1) * sizeof(grv_str_t));
    }
    arr->size--;
}
