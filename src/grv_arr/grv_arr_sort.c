#include "grv/grv_arr.h"

#ifndef GRV_TEST_COMPILE

static inline void _grv_arr_sort_swap_u8(void* a, void* b, size_t element_size) {
    u8* a_ptr = (u8*)a;
    u8* b_ptr = (u8*)b;
    for (int i = 0; i < element_size; ++i) {
        u8 temp = *a_ptr;
        *a_ptr++ = *b_ptr;
        *b_ptr++ = temp;
    }
}

static inline void _grv_arr_sort_swap_u32(void* a, void* b, size_t element_size) {
    u32* a_ptr = (u32*)a;
    u32* b_ptr = (u32*)b;
    for (int i = 0; i < element_size / sizeof(u32); ++i) {
        u32 temp = *a_ptr;
        *a_ptr++ = *b_ptr;
        *b_ptr++ = temp;
    }
}

static inline void _grv_arr_sort_swap_u64(void* a, void* b, size_t element_size) {
    u64* a_ptr = (u64*)a;
    u64* b_ptr = (u64*)b;
    for (int i = 0; i < element_size / sizeof(u64); ++i) {
        u64 temp = *a_ptr;
        *a_ptr++ = *b_ptr;
        *b_ptr++ = temp;
    }
}
    
static void _grv_arr_sort_swap(void* a, void* b, size_t element_size) {
    if (element_size % sizeof(u64) == 0) {
        _grv_arr_sort_swap_u64(a, b, element_size);
    } else if (element_size % sizeof(u32) == 0) {
        _grv_arr_sort_swap_u32(a, b, element_size);
    } else {
        _grv_arr_sort_swap_u8(a, b, element_size);
    }
}
#include <stdio.h>
#include "grv/grv_str.h"
#include "grv/grv_log.h"


static size_t grv_arr_sort_partition(void* arr, size_t size, size_t element_size, int(*compare_func)(void*, void*)) {
    void* pivot_ptr = arr;
    size_t i = 0;
    for (size_t j = 1; j < size; ++j) {
        if (compare_func(arr + j * element_size, pivot_ptr) <= 0) {
            i++;
            _grv_arr_sort_swap(arr + j * element_size, arr + i * element_size, element_size);
        }
    }
    _grv_arr_sort_swap(arr + (i) * element_size, pivot_ptr, element_size);
    return i;
}

void grv_arr_sort_impl(void* arr, size_t size, size_t element_size, int(*compare_func)(void*, void*)) {
    if (size <= 1) {
        return;
    } else if (size == 2) {
        if (compare_func(arr, arr + element_size) > 0) _grv_arr_sort_swap(arr, arr + element_size,  element_size);
        return;
    }

    size_t pivot_idx = grv_arr_sort_partition(arr, size, element_size, compare_func);
    //
    // sort subarrays:
    if (pivot_idx > 0) {
        grv_arr_sort_impl(arr, pivot_idx, element_size, compare_func);
    }
    if (pivot_idx < size - 1) {
        size_t b_start = pivot_idx + 1;
        size_t b_size = size - b_start;
        grv_arr_sort_impl(arr + element_size * b_start, b_size, element_size, compare_func);
    }
}

#else
#include "grv/grv_test.h"
#include "grv/grv_str.h"
#include "grv/grv_strarr.h"

int char_compare(void* a, void* b) {
    char ca = *(char*)a;
    char cb = *(char*)b;
    return ca < cb ? -1 : ca > cb ? 1 : 0;
}

int str_compare(void* a, void* b) {
    grv_str_t* str_a = a;
    grv_str_t* str_b = b;
    return grv_str_compare(*str_a, *str_b);
}

GRV_TEST_BEGIN_NEW(grv_arr_sort)
grv_str_t s = grv_str_new("780150");
grv_arr_sort_impl(s.data, s.size, sizeof(char), char_compare);
GRV_TEST_EQUAL_STR(s, "001578");
s = grv_str_new("");
grv_arr_sort_impl(s.data, s.size, sizeof(char), char_compare);
GRV_TEST_EQUAL_STR(s, "");
s = grv_str_new("a");
grv_arr_sort_impl(s.data, s.size, sizeof(char), char_compare);
GRV_TEST_EQUAL_STR(s, "a");
s = grv_str_new("ba");
grv_arr_sort_impl(s.data, s.size, sizeof(char), char_compare);
GRV_TEST_EQUAL_STR(s, "ab");

grv_strarr_t arr = grv_str_split(grv_str_ref("gamma beta alpha"), grv_str_ref(" "));
grv_arr_sort(&arr, str_compare);
grv_str_t sorted = grv_strarr_join(arr, grv_str_ref(" "));
GRV_TEST_EQUAL_STR(sorted, "alpha beta gamma");

GRV_TEST_END_NEW()

#endif
