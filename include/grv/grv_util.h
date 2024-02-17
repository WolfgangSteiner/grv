#ifndef GRV_UTIL_H
#define GRV_UTIL_H

#include "grv_base.h"
#include "grv_str.h"
#include "grv_strarr.h"

// Returns the width of the terminal
int grv_util_get_terminal_width();

// executes a command and returns the output as a grv_strarr
grv_strarr_t grv_system(grv_str_t cmd);
grv_strarr_t grv_system_cstr(char* cmd);

// Returns list of files given a pattern 
grv_strarr_t grv_util_glob(grv_str_t pattern);


// generate a unique id
u64 grv_util_generate_uid();

// generate random bytes
void grv_util_random_bytes(void* buffer, u64 length);

// get the current timestamp
f32 grv_timestamp();

// check if a cmd is installed on the system
bool grv_cmd_available(grv_str_t cmd);

struct tm grv_local_time();
grv_str_t grv_local_time_str();
f32 grv_local_time_f32();

grv_strarr_t grv_readlines(grv_str_t path);


#endif
