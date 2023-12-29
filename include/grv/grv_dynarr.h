#ifndef GRV_DYN_ARR_H
#define GRV_DYN_ARR_H

#include "grv_base.h"
#include <stdlib.h>

#define GRV_DYNARR_DEFAULT_CAPACITY 16
#define GRV_DYNARR_GROWTH_FACTOR 2

typedef struct {
    size_t size;
    size_t capacity;
    void* data;
} grv_dynarr_generic_t;

void grv_dynarr_generic_check_and_grow(grv_dynarr_generic_t* arr, size_t size);
void grv_dynarr_generic_free(grv_dynarr_generic_t* arr);
void grv_dynarr_generic_deep_free(grv_dynarr_generic_t* arr, void (*free_func)(void*));

#define DYNARR_DEFINE(TYPE, NAME) \
    typedef struct { \
        size_t size; \
        size_t capacity; \
        TYPE* data; \
    } grv_dynarr_##NAME##_t; \
    static inline void grv_dynarr_##NAME##_free(grv_dynarr_##NAME##_t* arr) { \
        if (arr->data) free(arr->data); \
        *dynarr = (grv_dynarr_##TYPE){0}; \
    } \
    static inline grv_dynarr_##NAME##_push(grv_dynarr_##NAME##_t* arr, TYPE data) { \
        grv_dynarr_check_and_grow((grv_dynarr_generic_t*)arr, sizeof(*arr->data)); \
        arr->data[arr->size++] = data; \
    } \
    static inline TYPE grv_dynarr_##NAME##_pop(grv_dynarr_##NAME##_t* arr) { \
        return arr->data[--arr->size]; \
    } \
    static inline void grv_dynarr_##TYPE##_clear(grv_dynarr_##NAME##_t* arr) { arr->size = 0; } \

#define grv_dynarr_push(arr, data) \
    grv_dynarr_check_and_grow((grv_dynarr_generic_t*)arr, sizeof(*arr->data)); \
    arr->data[arr->size++] = data; 

#define grv_dynarr_pop(arr) arr->data[--arr->size];
#define grv_dynarr_free(arr) grv_dynarr_generic_free((grv_dynarr_generic_t*)arr); 

#endif