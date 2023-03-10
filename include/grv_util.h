#ifndef GRV_UTIL_H
#define GRV_UTIL_H

#include "grv_defines.h"
#include "grv_str.h"

// Returns the width of the terminal
int grv_util_get_terminal_width();

// executes a command and returns the output as a grv_strarr
grv_strarr grv_system(grv_str* cmd);
grv_strarr grv_system_cstr(char* cmd);

#endif