#ifndef GRVBLD_H
#define GRVBLD_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>

static inline char* grvbld_cstr_new(char* str) {
    size_t len = strlen(str);
    char* new_str = malloc(len + 1);
    memcpy(new_str, str, len + 1);
    return new_str;
}

static inline char* grvbld_cstr_append(char* str, char* append_str) {
    size_t len = strlen(str);
    size_t append_len = strlen(append_str);
    str = realloc(str, len + append_len + 1);
    memcpy(str + len, append_str, append_len + 1);
    return str;
}

static inline char* grvbld_cstr_cat(char* a, char* b) {
    char* res = grvbld_cstr_new(a);
    res = grvbld_cstr_append(res, b);
    return res;
}

static inline char* grvbld_cstr_cat3(char* a, char* b, char* c) {
    char* res = grvbld_cstr_new(a);
    res = grvbld_cstr_append(res, b);
    res = grvbld_cstr_append(res, c);
    return res;
}   

static inline int grvbld_modtime(char* filename) {
    struct stat attr;
    stat(filename, &attr);
    return attr.st_mtime;
}

static inline bool grvbld_is_file_newer_than(char* filename, char* other_filename) {
    int mod_time_a = grvbld_modtime(filename);
    int mod_time_b = grvbld_modtime(other_filename);
    return mod_time_a > mod_time_b;
}

static inline bool is_source_file_newer(char* exe_filename) {
    char* src_filename = grvbld_cstr_cat(exe_filename, ".c");
    bool result = grvbld_is_file_newer_than(src_filename, exe_filename);
    result = result || grvbld_is_file_newer_than("src/grvbld.c", exe_filename);
    result = result || grvbld_is_file_newer_than("include/grv/grvbld.h", exe_filename);
    free(src_filename);
    return result;
}

__attribute__((format(__printf__, 1, 2)))
static inline void log_info(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[INFO] ");
    vprintf(fmt, args);
    printf("\n");
}

__attribute__((format(__printf__, 1, 2)))
static inline void log_error(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[ERROR] ");
    vprintf(fmt, args);
    printf("\n");
}

static inline int rebuild_file(char* filename) {
    char* src = grvbld_cstr_cat3("src/", filename, ".c");
    char* cmd = grvbld_cstr_cat("gcc -o ", filename);
    cmd = grvbld_cstr_append(cmd, " -DGRV_BUILD_CONFIGURED -Iinclude ");
    cmd = grvbld_cstr_append(cmd, src);
    cmd = grvbld_cstr_append(cmd, " src/grvbld.c");
    log_info("Rebuilding %s:", filename);
    log_info("%s", cmd);
    int result = system(cmd);
    free(src);
    free(cmd);
    return result;
}

#define GRV_CHECK_AND_REBUILD() \
    char* _executable_name = argv[0]; \
    if (is_source_file_newer(_executable_name)) { \
        int result = rebuild_file(_executable_name); \
        if (result == 0) { \
            log_info("Rebuilding of %s successful.", _executable_name); \
            system(_executable_name); \
            return 1; \
        } else { \
            log_error("Failed to rebuild %s", _executable_name); \
            return result; \
        } \
    }


typedef struct {
    char** data;
    size_t size;
    size_t capacity;
} grvbld_strarr_t;


typedef enum {
    GRVBLD_EXECUTABLE,
    GRVBLD_STATIC_LIBRARY
} grvbld_target_type_t;


struct grvbld_target_t;

typedef struct {
    struct grvbld_target_t** arr;
    size_t size;
    size_t capacity;
} grvbld_target_arr_t;

typedef struct grvbld_target_t{
    char* name;
    grvbld_target_type_t type;
    grvbld_strarr_t src_files;
    grvbld_strarr_t libs;
    grvbld_target_arr_t linked_targets; 
} grvbld_target_t;

typedef struct {
    char* cc;
    char* std;
    grvbld_strarr_t inc;
    grvbld_strarr_t warnings;
    grvbld_strarr_t libs;
    grvbld_strarr_t library_dirs;
    grvbld_strarr_t defines;
    char* build_dir;
    char* test_dir;
    bool debug;
    bool treat_warnings_as_errors;
    bool use_ccache;
    int verbosity;
} grvbld_config_t;

void grvbld_config_init(grvbld_config_t* config);
void grvbld_config_add_include_dir(grvbld_config_t* config, char* dir);
grvbld_target_t* grvbld_target_create(char* name, grvbld_target_type_t type);
void grvbld_target_add_src(grvbld_target_t* target, char* src);
void grvbld_target_link_library(grvbld_target_t* target, char* lib);
void grvbld_target_link(grvbld_target_t* target, grvbld_target_t* other);

int grvbld_build_test(grvbld_config_t* config, char* name);
int grvbld_test(grvbld_config_t*, char* name);
int grvbld_run_tests(grvbld_config_t*);
int grvbld_build_target(grvbld_config_t*, grvbld_target_t*);

bool grvbld_args_contain(int argc, char** argv, char* arg);

#endif