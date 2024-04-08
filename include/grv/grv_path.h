#ifndef GRV_PATH_H
#define GRV_PATH_H

#include "grv/grv_str.h"
#include "grv/grv_strarr.h"

#ifdef _WIN32
    #define GRV_PATH_SEPARATOR '\\'
#else
    #define GRV_PATH_SEPARATOR '/'
    #define GRV_PATH_SEPARATOR_STR "/"
#endif

grv_str_t grv_path_basename(grv_str_t path);
grv_str_t grv_path_dirname(grv_str_t path);
grv_str_t grv_path_stem(grv_str_t path);

// split a path into its components
grv_strarr_t grv_path_split(grv_str_t path);

// join a path from its components
grv_str_t grv_path_join(grv_strarr_t path_components); 

// expand the tilde to full home path 
grv_str_t grv_path_expand_tilde(grv_str_t path);

// append a path component
void grv_path_append(grv_str_t* path_a, grv_str_t path_b);

// prepend a path component
void grv_path_prepend(grv_str_t* path, grv_str_t prefix_path);

#ifdef _WIN32
char* grv_win_path_to_posix_path(wchar_t* win_path);
wchar_t* grv_posix_path_to_win_path(char* posix_path);
#endif

#endif

