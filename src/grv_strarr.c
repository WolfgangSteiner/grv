#include "grv/str.h"
#include <stdlib.h>
#include <string.h>

// grv_strarr
grv_strarr_t grv_strarr_new() {
  grv_strarr_t r = { .size=0, .capacity=GRV_STRARR_DEFAULT_CAPACITY };
  r.arr = calloc(GRV_STRARR_DEFAULT_CAPACITY, sizeof(grv_str_t));
  return r;
}

grv_strarr_t grv_strarr_from_cstr_array(char** arr, size_t size) {
  grv_strarr_t r = { .size=size, .capacity=size };
  r.arr = calloc(size, sizeof(grv_str_t));
  for (size_t i = 0; i < size; i++) {
    r.arr[i] = grv_str_new(arr[i]);
  }
  return r;
} 

size_t grv_strarr_size(grv_strarr_t* a) {
  return a->size;
}

void grv_strarr_free(grv_strarr_t* a) {
  for (size_t i = 0; i < a->size; i++) {
    grv_str_free(&a->arr[i]);
  }
  free(a->arr);
  a->capacity = 0;
  a->size = 0;
}

void grv_strarr_clear(grv_strarr_t* a) {
  for (size_t i = 0; i < a->size; i++) {
    grv_str_free(&a->arr[i]);
  }
  a->size = 0;
} 

void grv_strarr_push_cstr(grv_strarr_t* a, char* s) {
  if (a->size == a->capacity) {
    a->capacity *= GRV_STRARR_GROW_FACTOR;
    a->arr = realloc(a->arr, a->capacity);
  }
  a->arr[a->size++] = grv_str_new(s);
}

// push a string on the list
// the array takes ownership of the string, in case of a reference, the string is copied
void grv_strarr_push(grv_strarr_t* a, grv_str_t* s) {
  if (a->size == a->capacity) {
    a->capacity *= GRV_STRARR_GROW_FACTOR;
    a->arr = realloc(a->arr, a->capacity);
  }
  if (s->descriptor & GRV_STR_FLAG_IS_REFERENCE) {
    a->arr[a->size++] = grv_str_copy(s);
  } else {
    a->arr[a->size++] = *s;
  }
}

void grv_strarr_push_front(grv_strarr_t* a, grv_str_t* s) {
  if (a->size == a->capacity) {
    a->capacity *= GRV_STRARR_GROW_FACTOR;
    a->arr = realloc(a->arr, a->capacity * sizeof(grv_str_t));
  }
  memmove(&a->arr[1], &a->arr[0], a->size * sizeof(grv_str_t));
  a->arr[0] = *s;
  a->size++;
}

void grv_strarr_append(grv_strarr_t* a, grv_strarr_t* b) {
  for (size_t i = 0; i < b->size; i++) {
    grv_str_t s = grv_str_copy(&b->arr[i]);
    grv_strarr_push(a, &s);
  }
}

grv_str_t* grv_strarr_at(grv_strarr_t* arr, size_t idx) {
  return &arr->arr[idx];
}

grv_str_t* grv_strarr_front(grv_strarr_t* arr) {
  return &arr->arr[0];
}

grv_str_t* grv_strarr_back(grv_strarr_t* arr) {
  return &arr->arr[arr->size - 1];
}

grv_str_t grv_strarr_get_copy(grv_strarr_t* arr, size_t idx) {
  return grv_str_copy(&arr->arr[idx]);
} 

grv_str_t grv_strarr_pop_front(grv_strarr_t* arr) {
  grv_str_t res = arr->arr[0];
  for (size_t i = 0; i < arr->size - 1; ++i) {
    arr->arr[i] = arr->arr[i + 1];
  }
  arr->size--;
  return res;
}

grv_str_t grv_strarr_pop_back(grv_strarr_t* arr) {
  return arr->arr[--arr->size];
}

void grv_strarr_remove_front(grv_strarr_t* arr) {
  if (arr->size == 0) {
    return;
  }

  grv_str_free(&arr->arr[0]);
  for (size_t i = 0; i < arr->size - 1; ++i) {
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


grv_str_t grv_strarr_join(grv_strarr_t* arr, char* join_str) {
  grv_str_t res;
  size_t join_len = strlen(join_str);
  size_t total_len = 0;
  for (size_t i = 0; i < arr->size; ++i) {
    total_len += grv_str_len(&arr->arr[i]);
  }
  total_len += (arr->size - 1) * join_len;
  grv_str_init_with_capacity(&res, total_len + 1);
  char* dst = grv_str_cstr(&res);
  for (size_t i = 0; i < arr->size; ++i) {
    grv_str_t* s = &arr->arr[i];
    char* src = grv_str_cstr(s);
    size_t len = grv_str_len(s);
    memcpy(dst, src, len);
    dst += len;
    if (i != arr->size - 1) {
      memcpy(dst, join_str, join_len);
      dst += join_len;
    }
  }
  *dst = 0;
  grv_str_set_size(&res, total_len);
  return res;
}

bool grv_strarr_contains_cstr(grv_strarr_t* arr, char* cstr) {
  for (size_t i = 0; i < arr->size; ++i) {
    if (grv_str_eq_cstr(&arr->arr[i], cstr)) {
      return true;
    }
  }
  return false;
}
