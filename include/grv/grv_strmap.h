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
} grv_strmap_entry;

typedef struct {
  grv_strmap_entry* arr;
  size_t size;
  size_t capacity;
} grv_strmap;

// hash function for grv_strmap
u64 grv_strmap_hash(grv_str_t *key);

// initialize a new grv_strmap
void grv_strmap_init(grv_strmap *map);

// free a grv_strmap
void grv_strmap_free(grv_strmap *map);

// insert a new entry into a grv_strmap
// ownership of the key and value is transferred to the grv_strmap
void grv_strmap_insert(grv_strmap *map, grv_str_t *key, grv_str_t* value);

// insert a new entry into a grv_strmap
// key and value are copied into the grv_strmap
void grv_strmap_insert_cstr(grv_strmap* map, char* key, char* value);

// find an entry in a grv_strmap
// returns NULL if the key is not found
grv_strmap_entry* grv_strmap_find(grv_strmap* map, grv_str_t* key);

// find an entry in a grv_strmap
// returns NULL if the key is not found
grv_strmap_entry* grv_strmap_find_cstr(grv_strmap* map, char* key);

// get the value for a key from a grv_strmap
// ownership of the returned grv_str_t is transferred to the caller
// returns NULL if the key is not found
grv_str_t* grv_strmap_get(grv_strmap *map, grv_str_t *key);

// get the value for a key from a grv_strmap
// ownership of the returned grv_str_t is transferred to the caller
// returns NULL if the key is not found
grv_str_t* grv_strmap_get_cstr(grv_strmap *map, char *key);

#endif