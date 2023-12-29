#ifndef GRVBLD_H
#define GRVBLD_H

#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdio.h>
#include "grv/cstr.h"

static inline int file_get_mod_time(char* filename) {
    struct stat attr;
    stat(filename, &attr);
    return attr.st_mtime;
}

static inline bool is_file_newer_than(char* filename, char* other_filename) {
    int mod_time_a = file_get_mod_time(filename);
    int mod_time_b = file_get_mod_time(other_filename);
    return mod_time_a > mod_time_b;
}

static inline bool is_source_file_newer(char* exe_filename) {
    char* src_filename = grv_cstr_cat(exe_filename, ".c");
    bool result = is_file_newer_than(src_filename, exe_filename);
    free(src_filename);
    return result;
}

__attribute__((format(__printf__, 1, 2)))
static inline void log_info(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[INFO] ");
    vprintf(fmt, args);
}

__attribute__((format(__printf__, 1, 2)))
static inline void log_error(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[ERROR] ");
    vprintf(fmt, args);
}

static inline int rebuild_file(char* filename) {
    char* cmd = grv_cstr_cat("gcc -o ", filename);
    cmd = grv_cstr_append(cmd, " -DGRV_BUILD_CONFIGURED -Iinclude ");
    cmd = grv_cstr_append(cmd, filename);
    cmd = grv_cstr_append(cmd, ".c")
    cmd = grv_cstr_append(cmd, " src/grvbld.c src/cstr.c");
    log_info("Rebuilding %s\n", filename);
    int result = system(cmd);
    free(cmd);
    return result;
}

#define GRV_CHECK_AND_REBUILD() \
    char* _executable_name = argv[0]; \
    if (is_source_file_newer(_executable_name)) { \
        int result = rebuild_file(_executable_name); \
        if (result == 0) { \
            system(_executable_name); \
            return 1; \
        } else { \
            log_error("Failed to rebuild %s\n", _executable_name); \
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


typedef struct {
    char* name;
    grvbld_target_type_t type;
    grvbld_strarr_t src_files;
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

int grvbld_build_test(grvbld_config_t* config, char* name);
int grvbld_test(grvbld_config_t*, char* name);
int grvbld_run_tests(grvbld_config_t*);
int grvbld_build_target(grvbld_config_t*, grvbld_target_t*);

#endif