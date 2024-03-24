#ifndef GRV_UTIL_H
#define GRV_UTIL_H

#include "grv_base.h"
#include "grv_str.h"
#include "grv_strarr.h"
#include <unistd.h>

// Returns the width of the terminal
int grv_util_get_terminal_width(void);

// executes a command without capturing the output
// returns result of subcommand
int grv_system(grv_str_t cmd);

// executes a command, capturing the output using popen
// returns the output as a grv_strarr
grv_strarr_t grv_system_with_capture(grv_str_t cmd);
grv_strarr_t grv_system_with_capture_cstr(char* cmd);

// fork process
pid_t grv_fork(grv_str_t cmd, grv_strarr_t* args);

// Returns list of files given a pattern 
grv_strarr_t grv_util_glob(grv_str_t pattern);

// generate a unique id
u64 grv_util_generate_uid(void);

// generate random bytes
void grv_random_bytes(void* buffer, u64 length);

// generate one random byte
u8 grv_random_u8(void);

// generate random u32 
u32 grv_random_u32(void);

// generate random u64
u64 grv_random_u64(void);

// get the current timestamp
f32 grv_timestamp(void);

// check if a cmd is installed on the system
bool grv_cmd_available(grv_str_t cmd);

struct tm grv_local_time(void);
grv_str_t grv_local_time_str(void);
f32 grv_local_time_f32(void);
grv_str_t grv_local_datetime_str(void);

grv_strarr_t grv_readlines(grv_str_t path);

char grv_query_user(grv_str_t msg, grv_str_t choices);

#endif
