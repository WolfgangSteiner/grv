#ifndef GRV_STRMAP_H
#define GRV_STRMAP_H

#include "grv/grv_str.h"

#ifndef GRV_STRMAP_DEFAULT_CAPACITY
#define GRV_STRMAP_DEFAULT_CAPACITY 16
#endif

#ifndef GRV_STRMAP_GROWTH_FACTOR
#define GRV_STRMAP_GROWTH_FACTOR 2
#endif

typedef struct {
  grv_str_t key;
  grv_str_t value; 
  u64 hash;
} grv_strmap_entry_t;

typedef struct {
  grv_strmap_entry_t* arr;
  size_t size;
  size_t capacity;
} grv_strmap_t;

// hash function for grv_strmap_t
u64 grv_strmap_hash(grv_str_t key);

// initialize a new grv_strmap_t
void grv_strmap_init(grv_strmap_t *map);

// free a grv_strmap_t
void grv_strmap_free(grv_strmap_t *map);

// insert a new entry into a grv_strmap_t
// ownership of the key and value is transferred to the grv_strmap_t
void grv_strmap_insert_str(grv_strmap_t *map, grv_str_t key, grv_str_t value);
GRV_INLINE void grv_strmap_insert_cstr(grv_strmap_t *map, char* key, char* value) {
  grv_strmap_insert_str(map, grv_str_ref(key), grv_str_ref(value));
}
#define grv_strmap_insert(MAP, KEY, VALUE) _Generic((KEY), \
  grv_str_t: grv_strmap_insert_str, \
  char*: grv_strmap_insert_cstr \
)(MAP, KEY, VALUE)

// find an entry in a grv_strmap_t
// returns NULL if the key is not found
grv_strmap_entry_t* grv_strmap_find_str(grv_strmap_t* map, grv_str_t key);
GRV_INLINE grv_strmap_entry_t* grv_strmap_find_cstr(grv_strmap_t* map, char* key) {
  return grv_strmap_find_str(map, grv_str_ref(key));
}
#define grv_strmap_find(MAP, KEY) _Generic((KEY), \
  grv_str_t: grv_strmap_find_str, \
  char*: grv_strmap_find_cstr \
)(MAP, KEY)

// get the value for a key from a grv_strmap_t
// returns default_str if the key is not found
grv_str_t grv_strmap_get_str(grv_strmap_t *map, grv_str_t key, grv_str_t default_str);
GRV_INLINE grv_str_t grv_strmap_get_cstr(grv_strmap_t *map, char* key, char* default_str) {
  return grv_strmap_get_str(map, grv_str_ref(key), grv_str_ref(default_str));
}
#define grv_strmap_get(MAP, KEY, DEFAULT_STR) _Generic((KEY), \
  grv_str_t: grv_strmap_get_str, \
  char*: grv_strmap_get_cstr \
)(MAP, KEY, DEFAULT_STR)

#endif