#ifndef GRV_STRARR_H
#define GRV_STRARR_H

#include "grv_str.h"

#define GRV_STRARR_DEFAULT_CAPACITY 16
#define GRV_STRARR_GROWTH_FACTOR 2
typedef i32 grv_strarr_size_t;

typedef struct grv_strarr_t {
    grv_str_t* arr;
    grv_strarr_size_t size;
    grv_strarr_size_t capacity;
} grv_strarr_t;


grv_strarr_t grv_strarr_new(void);
grv_strarr_t grv_strarr_new_from_cstrarr(char** cstr_arr, grv_strarr_size_t size);
void grv_strarr_free(grv_strarr_t* strarr);
static inline grv_strarr_size_t grv_strarr_size(grv_strarr_t strarr) { return strarr.size; }

// copy a strarr. copy owns all of the strings.
grv_strarr_t grv_strarr_copy(grv_strarr_t arr);

// push a string on the list
// if the string owns its data, the array takes ownership of the string
// in case of a reference, the string caller is responsible to keep the underlying string alive
// if you want the string to be independent of the caller, use grv_str_copy before pushing it
grv_str_t* grv_strarr_push(grv_strarr_t* strarr, grv_str_t str);

GRV_INLINE grv_str_t* grv_strarr_push_str_ref(grv_strarr_t* strarr, char* cstr) {
    return grv_strarr_push(strarr, grv_str_ref(cstr));
}
grv_str_t* grv_strarr_push_front(grv_strarr_t* strarr, grv_str_t str);

// push a copy of a string to the array
grv_str_t* grv_strarr_push_copy(grv_strarr_t* strarr, grv_str_t str);

void grv_strarr_append(grv_strarr_t* strarr, grv_strarr_t other);

static inline grv_str_t* grv_strarr_at(grv_strarr_t strarr, grv_strarr_size_t index) {
    assert(index >= 0 && index < strarr.size);
    return &strarr.arr[index];
}

GRV_INLINE grv_str_t grv_strarr_get(grv_strarr_t strarr, grv_strarr_size_t index) { 
    assert(index >= 0 && index < strarr.size);
    return strarr.arr[index];
}

static inline grv_str_t* grv_strarr_front(grv_strarr_t strarr) {
    if (strarr.size == 0) return NULL;
    return grv_strarr_at(strarr, 0);
}

static inline grv_str_t* grv_strarr_back(grv_strarr_t strarr) {
    if (strarr.size == 0) return NULL;
    return grv_strarr_at(strarr, strarr.size - 1);
}

grv_str_t grv_strarr_pop_front(grv_strarr_t* strarr);
grv_str_t grv_strarr_pop_back(grv_strarr_t* strarr);

bool grv_strarr_any(grv_strarr_t strarr, bool(*predicate)(grv_str_t, void*), void* user_data); 

bool grv_strarr_contains_str(grv_strarr_t strarr, grv_str_t str);
GRV_INLINE bool grv_strarr_contains_cstr(grv_strarr_t strarr, char* cstr) {
    return grv_strarr_contains_str(strarr, grv_str_ref(cstr));
}
#define grv_strarr_contains(STRARR, STR) _Generic((STR), \
    grv_str_t: grv_strarr_contains_str, \
    char*: grv_strarr_contains_cstr \
)(STRARR, STR)

bool grv_strarr_any_contains_str(grv_strarr_t strarr, grv_str_t substr); 
GRV_INLINE bool grv_strarr_any_contains_cstr(grv_strarr_t strarr, char* cstr) {
    return grv_strarr_any_contains_str(strarr, grv_str_ref(cstr));
}

grv_str_t* grv_strarr_find_str_containing_str(grv_strarr_t strarr, grv_str_t substr);
GRV_INLINE grv_str_t* grv_strarr_find_str_containing_cstr(grv_strarr_t strarr, char* substr) {
    return grv_strarr_find_str_containing_str(strarr, grv_str_ref(substr));
}

i32 grv_strarr_index_of_str(grv_strarr_t strarr, grv_str_t str);

grv_str_t grv_strarr_join(grv_strarr_t strarr, grv_str_t separator);

grv_strarr_t grv_strarr_filter(grv_strarr_t arr, bool (*predicate)(grv_str_t));

void grv_strarr_remove_str(grv_strarr_t* arr, grv_str_t str); 

#endif
