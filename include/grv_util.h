#ifndef GRV_UTIL_H
#define GRV_UTIL_H

#include "grv/common.h"
#include "grv_str.h"

// Returns the width of the terminal
int grv_util_get_terminal_width();

// executes a command and returns the output as a grv_strarr
grv_strarr grv_system(grv_str* cmd);
grv_strarr grv_system_cstr(char* cmd);

// Returns list of files given a pattern 
grv_strarr grv_util_glob(grv_str* pattern);


// generate a unique id
u64 grv_util_generate_uid();

// generate random bytes
void grv_util_random_bytes(void* buffer, u64 length);

// get the current timestamp
f32 grv_timestamp();

void* grv_alloc(u64 size);
void* grv_realloc(void* ptr, u64 size);
void grv_free(void* ptr);


#endif