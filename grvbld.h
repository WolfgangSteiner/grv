#ifndef GRVBLD_H
#define GRVBLD_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <glob.h>

#ifndef NULL
#define NULL ((void*)0L);
#endif

#ifdef _WIN32
#define GRVBLD_PATH_SEP '\\'
#include <direct.h>
#else
#define GRVBLD_PATH_SEP '/'
#include <dirent.h>
#include <sys/stat.h>
#endif

#define GRVBLD_INLINE static inline 

//==============================================================================
// pass-through of cmd line arguments
//==============================================================================
GRVBLD_INLINE char* concatenate_arguments(int argc, char** argv) {
    int size = strlen(argv[0]) + 1;
    char* cmd_line = calloc(size, 1);
    strcat(cmd_line, argv[0]);  
    for (int i = 1; i < argc; ++i) {
        size += strlen(argv[i]) + 1;
        cmd_line = realloc(cmd_line, size);
        strcat(cmd_line, " ");
        strcat(cmd_line, argv[i]);
    }
    return cmd_line;
}

//==============================================================================
// grvbld_cstr
//==============================================================================
GRVBLD_INLINE char* grvbld_cstr_new(char* str) {
    size_t len = strlen(str);
    char* new_str = calloc(len + 1, 1);
    memcpy(new_str, str, len + 1);
    return new_str;
}

GRVBLD_INLINE char* grvbld_cstr_append(char* str, char* append_str) {
    size_t len = strlen(str);
    size_t append_len = strlen(append_str);
    str = realloc(str, len + append_len + 1);
    memcpy(str + len, append_str, append_len + 1);
    return str;
}

char* grvbld_cstr_prepend(char* str, char* prepend_str) {
    size_t str_len = strlen(str);
    size_t prepend_str_len = strlen(prepend_str);
    char* res = calloc(str_len + prepend_str_len + 1, 1);
    memcpy(res, prepend_str, prepend_str_len);
    memcpy(res + prepend_str_len, str, str_len + 1);
    return res;
}

GRVBLD_INLINE char* grvbld_cstr_cat(char* a, char* b) {
    char* res = grvbld_cstr_new(a);
    res = grvbld_cstr_append(res, b);
    return res;
}

GRVBLD_INLINE char* grvbld_cstr_cat3(char* a, char* b, char* c) {
    char* res = grvbld_cstr_new(a);
    res = grvbld_cstr_append(res, b);
    res = grvbld_cstr_append(res, c);
    return res;
}   

bool grvbld_cstr_starts_with(char* str, char* prefix) {
    size_t prefix_len = strlen(prefix);
    size_t str_len = strlen(str);
    if (prefix_len > str_len) return false;
    for (int i = 0; i < prefix_len; ++i) {
        if (*str++ != *prefix++) return false;
    }
    return true;
}

char* grvbld_cstr_remove_head(char* str, char* head) {
    assert(grvbld_cstr_starts_with(str, head));
    return grvbld_cstr_new(str + strlen(head));
}

char* grvbld_cstr_remove_path_head(char* str) {
    while (*str++ != '/');
    return grvbld_cstr_new(str);
}

//==============================================================================
// File system
//==============================================================================

GRVBLD_INLINE int grvbld_modtime(char* filename) {
    struct stat attr;
    stat(filename, &attr);
    return attr.st_mtime;
}

GRVBLD_INLINE bool grvbld_is_file_newer_than(char* filename, char* other_filename) {
    int mod_time_a = grvbld_modtime(filename);
    int mod_time_b = grvbld_modtime(other_filename);
    return mod_time_a > mod_time_b;
}

GRVBLD_INLINE bool is_source_file_newer(char* exe_filename) {
    char* src_filename = grvbld_cstr_cat(exe_filename, ".c");
    bool result = grvbld_is_file_newer_than(src_filename, exe_filename);
    result = result || grvbld_is_file_newer_than("grvbld.h", exe_filename);
    free(src_filename);
    return result;
}


//==============================================================================
// Logging
//==============================================================================
__attribute__((format(__printf__, 1, 2)))
GRVBLD_INLINE void log_info(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[INFO] ");
    vprintf(fmt, args);
    printf("\n");
}

__attribute__((format(__printf__, 1, 2)))
GRVBLD_INLINE void log_error(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[ERROR] ");
    vprintf(fmt, args);
    printf("\n");
}

GRVBLD_INLINE void log_newline() {
    printf("\n");
}
//==============================================================================
// Data types
//==============================================================================
typedef struct {
    char** data;
    size_t size;
    size_t capacity;
} grvbld_strarr_t;


typedef enum {
    GRVBLD_EXECUTABLE,
    GRVBLD_LIBRARY, // build dynamic or static depending on config
    GRVBLD_STATIC_LIBRARY,
    GRVBLD_DYNAMIC_LIBRARY
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
    grvbld_strarr_t data_files;
    grvbld_strarr_t compile_options;
    grvbld_strarr_t link_options;
    grvbld_target_arr_t linked_targets; 
    bool run_after_build;
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
    bool run_tests;
    bool tests_only;
    bool dynamic; // build dynamic libraries
    int verbosity;
} grvbld_config_t;


//==============================================================================
// grvbld_strarr
//==============================================================================
GRVBLD_INLINE grvbld_strarr_t* grvbld_strarr_new(void) { 
    grvbld_strarr_t* res = calloc(1, sizeof(grvbld_strarr_t));
    return res;
}

GRVBLD_INLINE void grvbld_strarr_push(grvbld_strarr_t* arr, char* str) {
    if (arr->capacity == 0) {
        arr->capacity = 16;
        arr->size = 0;
        arr->data = calloc(arr->capacity, sizeof(char*));
    } else if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(char*));
    }
    arr->data[arr->size++] = grvbld_cstr_new(str);
}

void grvbld_strarr_append(grvbld_strarr_t* arr, grvbld_strarr_t* append_arr) {
    for (int i = 0; i < append_arr->size; ++i) {
         grvbld_strarr_push(arr, append_arr->data[i]);
    }
}

GRVBLD_INLINE void grvbld_strarr_dup(grvbld_strarr_t* dst, grvbld_strarr_t* src) {
    dst->size = src->size;
    dst->capacity = src->capacity;
    dst->data = calloc(dst->capacity, sizeof(char*));
    for (size_t i = 0; i < src->size; ++i) {
        dst->data[i] = grvbld_cstr_new(src->data[i]);
    }
}


//==============================================================================
// grvbld_str 
//==============================================================================

GRVBLD_INLINE size_t grvbld_cstr_len(char* str) {
    return strlen(str);
}

GRVBLD_INLINE char* grvbld_cstr_append_char(char* dst, char c) {
    size_t len = grvbld_cstr_len(dst);
    dst = realloc(dst, len + 2);
    dst[len] = c;
    dst[len + 1] = '\0';
    return dst;
}

GRVBLD_INLINE char* grvbld_cstr_append_path(char* dst, char* path) {
    dst = grvbld_cstr_append_char(dst, GRVBLD_PATH_SEP);
    dst = grvbld_cstr_append(dst, path);
    return dst;
}

GRVBLD_INLINE char* grvbld_cstr_append_arg(char* dst, char* arg) {
    dst = grvbld_cstr_append(dst, " ");
    dst = grvbld_cstr_append(dst, arg);
    return dst;
}

int grvbld_cstr_rfind_char(char* str, char c) {
    char* ptr = strrchr(str, GRVBLD_PATH_SEP);
    if (ptr) return (int)(ptr - str);
    else return -1;
}

char* grvbld_dirname(char* str) {
    char* res = grvbld_cstr_new(str);
    char* last_sep = strrchr(res, GRVBLD_PATH_SEP);
    int pos = grvbld_cstr_rfind_char(res, GRVBLD_PATH_SEP);
    if (pos > 0) {
        res[pos] = 0;
    } else {
        res[0] = 0;
    }
    return res;
}

GRVBLD_INLINE char* grvbld_cstr_filename(char* str) {
    char* last_sep = strrchr(str, GRVBLD_PATH_SEP);
    if (last_sep) {
        return grvbld_cstr_new(last_sep + 1);
    } else {
        return grvbld_cstr_new(str);
    } 
}

char* grvbld_cstr_remove_ext(char* str) {
    char* res = grvbld_cstr_new(str);
    char* dot = strrchr(res, '.');
    if (dot) {
        *dot = 0;
    }
    return res;
}

GRVBLD_INLINE char* grvbld_cstr_replace_ext(char* str, char* ext) {
    str = grvbld_cstr_remove_ext(str);
    str = grvbld_cstr_append(str, ext);
    return str;
 }

GRVBLD_INLINE char* grvbld_cstr_prepend_path(char* str, char* path) {
    char* result = grvbld_cstr_new(path);
    result = grvbld_cstr_append_path(result, str);
    return result;
}

GRVBLD_INLINE char* grvbld_cstr_new_with_format(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    char* result = calloc(len + 1, sizeof(char));
    va_start(args, fmt);
    vsnprintf(result, len + 1, fmt, args);
    va_end(args);
    return result;
}

GRVBLD_INLINE char* grvbld_cstr_append_arg_format(char* dst, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    char* formatted_str = calloc(len + 1, sizeof(char));
    va_start(args, fmt);
    vsnprintf(formatted_str, len + 1, fmt, args);
    va_end(args);
    dst = grvbld_cstr_append_arg(dst, formatted_str);
    free(formatted_str);
    return dst;
}

char* grvbld_path_construct(char* first, ...) {
    assert(first != NULL);
    va_list args;
    va_start(args, first);
    size_t total_length = strlen(first) + 1;
    char* str = va_arg(args, char*);
    while (str != NULL) {
        total_length += strlen(str) + 1;
        str = va_arg(args, char*);
    }
    va_end(args);
    total_length *= 2; 
    va_start(args, first);
    char* res = calloc(total_length, 1);
    size_t cur_len = strlen(first);    
    memcpy(res, first, cur_len);
    str = va_arg(args, char*);
    while (str != NULL) {
        size_t str_len = strlen(str);
        res[cur_len++] = '/';
        memcpy(res + cur_len, str, str_len);
        cur_len += str_len;
        str = va_arg(args, char*);
    }
    va_end(args);
    return res;
}
    
GRVBLD_INLINE bool starts_with(char* str, char* prefix) {
    size_t prefix_len = strlen(prefix);
    return strncmp(str, prefix, prefix_len) == 0;
}

GRVBLD_INLINE bool ends_with(char* str, char* suffix) {
    size_t suffix_len = strlen(suffix);
    size_t str_len = strlen(str);
    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

GRVBLD_INLINE bool grvbld_cstr_eq(char* a, char* b) {
    return strcmp(a, b) == 0;
}

void grvbld_cstr_rstrip(char* str) {
    int len = strlen(str);
    while (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
        len--;
    }
}

GRVBLD_INLINE bool grvbld_args_contain(int argc, char** argv, char* arg) {
    for (int i = 0; i < argc; ++i) {
        if (grvbld_cstr_eq(argv[i], arg)) {
            return true;
        }
    }
    return false;
}

//==============================================================================
// grvbld_config_t
//==============================================================================
GRVBLD_INLINE grvbld_config_t* grvbld_config_new(int argc, char** argv) {
    grvbld_config_t* config = calloc(1, sizeof(grvbld_config_t));
    config->cc = "gcc";
    config->std = "gnu11";
    config->build_dir = "build";
    config->test_dir = "test";
    config->debug = true;
    config->treat_warnings_as_errors = false;
    config->verbosity = 0;
    config->use_ccache = true;
    config->run_tests = true;
    config->dynamic = false;

    grvbld_strarr_push(&config->inc, "include");
    grvbld_strarr_push(&config->warnings, "-Wall");
    grvbld_strarr_push(&config->warnings, "-Wextra");
    grvbld_strarr_push(&config->warnings, "-Wpedantic");
    grvbld_strarr_push(&config->warnings, "-Wno-cast-function-type");
    grvbld_strarr_push(&config->warnings, "-Werror=return-type");
    grvbld_strarr_push(&config->warnings, "-Werror=implicit-function-declaration");
    grvbld_strarr_push(&config->warnings, "-Werror=strict-prototypes"); 
    //grvbld_strarr_push(&config->libs, "-lm");

    if (grvbld_args_contain(argc, argv, "--debug")) config->debug = true;
    if (grvbld_args_contain(argc, argv, "--use-ccache")) config->use_ccache = true;
    if (grvbld_args_contain(argc, argv, "--no-use-ccache")) config->use_ccache = false;
    if (grvbld_args_contain(argc, argv, "--tests-only")) config->tests_only = true;
    if (grvbld_args_contain(argc, argv, "--no-tests")) config->run_tests = false;
    if (grvbld_args_contain(argc, argv, "--dynamic")) config->dynamic = true;
    if (grvbld_args_contain(argc, argv, "-vv")) {
        config->verbosity = 2;
    } else if (grvbld_args_contain(argc, argv, "-v")) {
        config->verbosity = 1;
    }
    printf("verbosity: %d\n", config->verbosity);
    return config;
}

GRVBLD_INLINE grvbld_config_t* grvbld_config_dup(grvbld_config_t* src) {
    grvbld_config_t* dst = calloc(1, sizeof(grvbld_config_t));
    memcpy(dst, src, sizeof(grvbld_config_t));
    dst->cc = grvbld_cstr_new(src->cc);
    dst->std = grvbld_cstr_new(src->std);
    grvbld_strarr_dup(&dst->inc, &src->inc); 
    grvbld_strarr_dup(&dst->warnings, &src->warnings);
    grvbld_strarr_dup(&dst->libs, &src->libs);
    grvbld_strarr_dup(&dst->defines, &src->defines);
    dst->build_dir = grvbld_cstr_new(src->build_dir);
    dst->test_dir = grvbld_cstr_new(src->test_dir);
    return dst;
}

GRVBLD_INLINE void grvbld_config_add_include_directory(grvbld_config_t* config, char* path) {
    grvbld_strarr_push(&config->inc, path);
}

GRVBLD_INLINE void grvbld_config_add_include_directories(grvbld_config_t* config, ...) {
    va_list args;
    va_start(args, config);
    char* dir = 0;
    int counter = 0;
    while ((dir = va_arg(args, char*)) != NULL) {
        assert(counter++ < 32);
        grvbld_strarr_push(&config->inc, dir);
    }
}
GRVBLD_INLINE void grvbld_config_add_library_directory(grvbld_config_t* config, char* path) {
    grvbld_strarr_push(&config->library_dirs, path);
}

//==============================================================================
// utility functions
//==============================================================================
GRVBLD_INLINE bool has_directory(char* path) {
#ifdef _WIN32
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY));
#else
    DIR* dir = opendir(path);
    if (dir) {
        closedir(dir);
        return true;
    }
    return false;
#endif
}

GRVBLD_INLINE bool create_directory(char* path) {
#ifdef _WIN32
    return CreateDirectoryA(path, NULL);
#else
    return mkdir(path, 0777) == 0;
#endif
}

GRVBLD_INLINE bool make_path(char* path) {
#if 0
    bool result = false;
    char* p = grvbld_cstr_new(path);
    char* s = p;
    char* e = strchr(s, GRVBLD_PATH_SEP);    
    while (e) {
        *e = 0;
        if (strlen(path) && !has_directory(path)) {
            if (!create_directory(path)) {
                goto end;
            }
        }
        s = e + 1;
        e = strchr(s, GRVBLD_PATH_SEP);
    }   

    if (strlen(path) && !has_directory(s)) {
        if (!create_directory(s)) {
            goto end;
        }
    }

    result = true;

end:
    free(p);
    return result;
#else 
    char* cmd = grvbld_cstr_new_with_format("mkdir -p %s", path);
    int result = system(cmd);
    free(cmd);
    return result == 0;
#endif
}

GRVBLD_INLINE grvbld_strarr_t* get_files_in_dir(char* path) {
    if (!has_directory(path)) {
        printf("[ERROR] path \"%s\" does not exist\n", path);
        exit(1);
    }

    grvbld_strarr_t* result = grvbld_strarr_new();

#ifdef _WIN32
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle = FindFirstFileA(path, &find_data);
    if (find_handle == INVALID_HANDLE_VALUE) {
        return result;
    }

    do {
        grvbld_strarr_push(result, find_data.cFileName);
    } while (FindNextFileA(find_handle, &find_data));

    FindClose(find_handle);

#else
    DIR* directory = opendir(path);
    if (directory == NULL) {
        return result;
    }

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        grvbld_strarr_push(result, entry->d_name);
    }

    closedir(directory);
#endif
    return result;
}

bool grvbld_cmd_available(char* cmd) {
    char* which_cmd = grvbld_cstr_new_with_format("which %s > /dev/null", cmd);
    bool result = false;
    result = (system(which_cmd) == 0);
    free(which_cmd);
    return result;
}

int grvbld_execute_build_cmd(grvbld_config_t* config, char* cmd) {
    if (config->verbosity > 0) log_info("%s", cmd);
    if (config->verbosity < 2) {
        cmd = grvbld_cstr_append_arg(cmd, "-fno-diagnostics-show-option");
        cmd = grvbld_cstr_append_arg(cmd, "-fno-diagnostics-show-caret");
    }
    return system(cmd);
}

int grvbld_execute_link_cmd(grvbld_config_t* config, char* cmd) {
    if (config->verbosity > 0) log_info("%s", cmd);
    return system(cmd);
}

grvbld_strarr_t* grvbld_system(char* cmd) {
    grvbld_strarr_t* res = grvbld_strarr_new();
    size_t buffer_size = 4096;
    char* buffer = calloc(buffer_size, 1);
    FILE* fp = popen(cmd, "r");
    while (fgets(buffer, buffer_size, fp)) {
        grvbld_cstr_rstrip(buffer);
        grvbld_strarr_push(res, grvbld_cstr_new(buffer));
    }
    free(buffer);
    return res;
}

grvbld_strarr_t* grvbld_glob(char* pattern) {
    grvbld_strarr_t* res = grvbld_strarr_new();
    glob_t glob_result;
    glob(pattern, GLOB_TILDE, NULL, &glob_result);

    if (glob_result.gl_pathc == 0) {
        globfree(&glob_result);
        return res;
    }
    for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
        char* path = grvbld_cstr_new(glob_result.gl_pathv[i]);
        grvbld_strarr_push(res, path);
    }
    globfree(&glob_result);
    return res;
}

//==============================================================================
// rebuilding 
//==============================================================================
GRVBLD_INLINE int rebuild_file(char* filename) {
    char* src = grvbld_cstr_cat(filename, ".c");
    char* cmd = grvbld_cstr_cat("gcc -g -o ", filename);
    cmd = grvbld_cstr_append(cmd, " -Iinclude ");
    cmd = grvbld_cstr_append(cmd, src);
    //cmd = grvbld_cstr_append(cmd, " src/grvbld.c");
    log_info("Rebuilding %s:", filename);
    log_info("%s", cmd);
    int result = system(cmd);
    free(src);
    free(cmd);
    return result;
}

GRVBLD_INLINE char* grvbld_cat_argv(int argc, char** argv) {
    char* cmd = grvbld_cstr_new("");
    for (int i = 0; i < argc; ++i) {
         cmd = grvbld_cstr_append_arg(cmd, argv[i]);
    }
    return cmd;
}

#define GRV_CHECK_AND_REBUILD() \
    char* _executable_name = argv[0]; \
    if (is_source_file_newer(_executable_name)) { \
        int result = rebuild_file(_executable_name); \
        if (result == 0) { \
            log_info("Rebuilding of %s successful.", _executable_name); \
            system(grvbld_cat_argv(argc, argv)); \
            return 1; \
        } else { \
            log_error("Failed to rebuild %s", _executable_name); \
            return result; \
        } \
    }

    
//==============================================================================
// grvbld_target_t
//==============================================================================
GRVBLD_INLINE grvbld_target_t* grvbld_target_create(char* name, grvbld_target_type_t type) {
    grvbld_target_t* target = calloc(1, sizeof(grvbld_target_t));
    target->name = grvbld_cstr_new(name);
    target->type = type;
    target->linked_targets = (grvbld_target_arr_t){0};
    return target;
}

GRVBLD_INLINE grvbld_target_t* grvbld_target_create_executable(char* name) {
    return grvbld_target_create(name, GRVBLD_EXECUTABLE);
}

GRVBLD_INLINE grvbld_target_t* grvbld_target_create_static_library(char* name) {
    return grvbld_target_create(name, GRVBLD_STATIC_LIBRARY);
}

GRVBLD_INLINE grvbld_target_t* grvbld_target_create_dynamic_library(char* name) {
    return grvbld_target_create(name, GRVBLD_DYNAMIC_LIBRARY);
}

GRVBLD_INLINE grvbld_target_t* grvbld_target_create_library(char* name) {
    return grvbld_target_create(name, GRVBLD_LIBRARY);
}

GRVBLD_INLINE void grvbld_target_add_src(grvbld_target_t* target, char* src) {
    grvbld_strarr_push(&target->src_files, src);
}

GRVBLD_INLINE void grvbld_target_add_src_files(grvbld_target_t* target, ...) {
    va_list args;
    va_start(args, target);
    char* src = 0;
    int counter = 0;
    while ((src = va_arg(args, char*)) != NULL) {
        assert(counter++ < 32);
        grvbld_target_add_src(target, src); 
    }
}

GRVBLD_INLINE void grvbld_target_link_library(grvbld_target_t* target, char* lib) {
    grvbld_strarr_push(&target->libs, lib);
}

GRVBLD_INLINE void grvbld_target_add_compile_option(grvbld_target_t* target, char* option) {
    grvbld_strarr_push(&target->compile_options, option);
}

GRVBLD_INLINE void grvbld_target_add_link_option(grvbld_target_t* target, char* option) {
    grvbld_strarr_push(&target->link_options, option);
}

GRVBLD_INLINE void grvbld_target_link_libraries(grvbld_target_t* target, ...) {
    va_list args;
    va_start(args, target);
    char* lib = 0;
    int counter = 0;
    while ((lib = va_arg(args, char*)) != NULL) {
        assert(counter++ < 32);
        grvbld_strarr_push(&target->libs, lib);
    }
}

GRVBLD_INLINE void grvbld_target_add_data_file(grvbld_target_t* target, char* data_file) {
    grvbld_strarr_push(&target->data_files, data_file);
}

GRVBLD_INLINE void grvbld_target_link(grvbld_target_t* target, grvbld_target_t* linked_target) {
    grvbld_target_arr_t* arr = &target->linked_targets;
    if (arr->capacity == 0) {
        arr->capacity = 16;
        arr->arr = calloc(arr->capacity, sizeof(grvbld_target_t*));
    } else if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->arr = realloc(arr->arr, arr->capacity * sizeof(grvbld_target_t*));
    }
    arr->arr[arr->size++] = linked_target;
}

//==============================================================================
// constructing the build command
//==============================================================================
GRVBLD_INLINE char* grvbld_build_cmd(grvbld_config_t* config) {
    char* cmd = grvbld_cstr_new("");
    bool ccache_available = grvbld_cmd_available("ccache");
    if (ccache_available && config->use_ccache) {
        cmd = grvbld_cstr_append(cmd, "ccache ");
    }
    cmd = grvbld_cstr_append(cmd, config->cc);
    cmd = grvbld_cstr_append_arg_format(cmd, "-std=%s", config->std);
    
    for (size_t i = 0; i < config->inc.size; ++i) {
        cmd = grvbld_cstr_append_arg_format(cmd, "-I%s", config->inc.data[i]);
    }

    for (size_t i = 0; i < config->library_dirs.size; ++i) {
        cmd = grvbld_cstr_append_arg_format(cmd, "-L%s", config->library_dirs.data[i]);
    }

    for (size_t i = 0; i < config->warnings.size; ++i) {
        cmd = grvbld_cstr_append_arg(cmd, config->warnings.data[i]);
    }

    if (config->debug) {
        cmd = grvbld_cstr_append_arg(cmd, "-g3");
    } else {
        cmd = grvbld_cstr_append_arg(cmd, "-O2");
    }

    if (config->treat_warnings_as_errors) {
        cmd = grvbld_cstr_append_arg(cmd, "-Werror");
    }

    for (size_t i = 0; i < config->libs.size; ++i) {
        cmd = grvbld_cstr_append_arg(cmd, config->libs.data[i]);
    }

    for (size_t i = 0; i < config->defines.size; ++i) {
        cmd = grvbld_cstr_append_arg_format(cmd, "-D%s", config->defines.data[i]);
    }

//    if (config->debug) {
//        cmd = grvbld_cstr_append_arg(cmd, "-DGRV_DEBUG_MEMORY");
//    }

    cmd = grvbld_cstr_append_arg_format(cmd, "-L%s", config->build_dir);

    for (size_t i = 0; i < config->library_dirs.size; ++i) {
        cmd = grvbld_cstr_append_arg_format(cmd, "-L%s", config->library_dirs.data[i]);
    }

    return cmd;
}

//==============================================================================
// testing code
//==============================================================================
int grvbld_test(grvbld_config_t* config, char* src_file) {
    config = grvbld_config_dup(config);    
    config->debug = true;
    //grvbld_strarr_push(&config->defines, "GRV_DEBUG_MEMORY");
    char* test_name = grvbld_cstr_remove_path_head(grvbld_cstr_remove_ext(src_file));
    char* src_path = grvbld_dirname(src_file);
    char* dst_path = grvbld_path_construct(config->build_dir, "test", grvbld_dirname(test_name), NULL);

    if (!has_directory(dst_path)) {
        bool make_path_ok = make_path(dst_path);
        if (!make_path_ok) {
            log_error("failed to create path \"%s\"", dst_path);
            exit(1);
        }
    }

    char* dst_file = grvbld_path_construct(config->build_dir, "test", test_name, NULL);
    char* cmd = grvbld_build_cmd(config);
    cmd = grvbld_cstr_append_arg(cmd, src_file);
    cmd = grvbld_cstr_append_arg_format(cmd, "-o %s", dst_file);
    cmd = grvbld_cstr_append_arg(cmd, "-lgrv");
    cmd = grvbld_cstr_append_arg(cmd, "-lm");

    bool is_src_file = grvbld_cstr_starts_with(src_path, "src/");
    if (is_src_file) cmd = grvbld_cstr_append_arg(cmd, "-DGRV_TEST_COMPILE");

    int result = grvbld_execute_build_cmd(config, cmd);
    if (result != 0) {
        log_error("failed to build test \"%s\"", test_name);
        exit(1);
    }

    return system(dst_file);
    
    #if 0
    if (grv_cmd_available(grv_str_ref("valgrind"))) {
        grvbld_cstr_t* valgrind_cmd = grvbld_str_alloc("valgrind --leak-check=full --show-leak-kinds=all ");
        grvbld_cstr_append_arg(valgrind_cmd, dst_file->str);
        printf("[INFO] %s\n", valgrind_cmd->str);
        system(valgrind_cmd->str);
    }
    #endif
}

int grvbld_run_tests(grvbld_config_t* config) {
    grvbld_strarr_t* test_files = grvbld_system("find test -name \"*.c\" | sort");
    grvbld_strarr_t* src_test_files = grvbld_system("ag -l GRV_TEST_COMPILE src | sort"); 
    grvbld_strarr_append(test_files, src_test_files);
    bool success = true;
    for (size_t i = 0; i < test_files->size; ++i) {
        char* test_file = test_files->data[i];
        int test_result = grvbld_test(config, test_file);
        if (test_result > 0) success = false;
    }
    return success ? 0 : 1;
}

grvbld_strarr_t grvbld_build_data_files(grvbld_config_t* config, grvbld_target_t* target) {
    grvbld_strarr_t data_obj_files = {0};
    for (size_t i = 0; i < target->data_files.size; ++i) {
        char* data_src = target->data_files.data[i];
        char* data_dst = grvbld_cstr_filename(data_src);
        char* data_obj_dir = grvbld_cstr_cat(config->build_dir, "/data");
        make_path(data_obj_dir);
        data_dst = grvbld_cstr_prepend_path(data_dst, data_obj_dir);
        data_dst = grvbld_cstr_append(data_dst, ".o");
        char* ld_cmd = grvbld_cstr_new_with_format("ld -r -b binary -o %s -z noexecstack %s", data_dst, data_src);
        log_info("%s", ld_cmd);
        system(ld_cmd);
        grvbld_strarr_push(&data_obj_files, data_dst);
    }
    return data_obj_files;
}

//==============================================================================
// grvbld main functions
//==============================================================================
GRVBLD_INLINE void create_build_path(grvbld_config_t* config) {
    bool make_path_ok = make_path(config->build_dir);
    if (!make_path_ok) {
        printf("[ERROR] failed to create path \"%s\"\n", config->build_dir);
        exit(1);
    }
}

GRVBLD_INLINE char* grvbld_obj_file_for_src_file(grvbld_config_t* config, char* src) {
    char* obj_file = grvbld_cstr_filename(src);
    obj_file = grvbld_cstr_prepend_path(obj_file, config->build_dir);
    obj_file = grvbld_cstr_replace_ext(obj_file, ".o");
    return obj_file;
}

GRVBLD_INLINE int grvbld_build_static_library(grvbld_config_t* config, grvbld_target_t* target) {
    char* lib_file = grvbld_cstr_new_with_format("%s/lib%s.a", config->build_dir, target->name);
    char* ar_cmd = grvbld_cstr_new_with_format("rm -f %s && ar rcs %s", lib_file, lib_file);

    log_info("BUILDING STATIC LIBRARY %s", target->name);

    // compile all of the source files
    for (size_t i = 0; i < target->src_files.size; ++i) {
        char* src_file = grvbld_cstr_new(target->src_files.data[i]);
        char* obj_file = grvbld_obj_file_for_src_file(config, src_file);
        char* cmd = grvbld_build_cmd(config);
        cmd = grvbld_cstr_append_arg(cmd, "-c");
        cmd = grvbld_cstr_append_arg(cmd, src_file);
        cmd = grvbld_cstr_append_arg_format(cmd, "-o %s", obj_file);
        ar_cmd = grvbld_cstr_append_arg(ar_cmd, obj_file);

        int result = grvbld_execute_build_cmd(config, cmd);
        if (result != 0) {
            log_error("failed to build %s", target->name);
            exit(1);
        }
    }

    grvbld_strarr_t data_obj_files = grvbld_build_data_files(config, target);
    for (size_t i = 0; i < data_obj_files.size; ++i) {
        ar_cmd = grvbld_cstr_append_arg(ar_cmd, data_obj_files.data[i]);
    }

    log_info("%s", ar_cmd);
    int result = system(ar_cmd);
    if (result != 0) {
        log_error("failed to build %s", target->name);
        exit(1);
    }
    return result;
}

GRVBLD_INLINE int grvbld_build_dynamic_library(grvbld_config_t* config, grvbld_target_t* target) {
    grvbld_strarr_t obj_files = {0};
    log_info("BUILDING DYNAMIC LIBRARY %s", target->name);

    for (size_t i = 0; i < target->src_files.size; i++) {
        char* src = target->src_files.data[i];
        char* obj = grvbld_obj_file_for_src_file(config, src);
        char* cmd = grvbld_build_cmd(config);
        cmd = grvbld_cstr_append_arg(cmd, "-fPIC -c");
        cmd = grvbld_cstr_append_arg(cmd, "-o");
        cmd = grvbld_cstr_append_arg(cmd, obj);
        cmd = grvbld_cstr_append_arg(cmd, src);
        grvbld_strarr_push(&obj_files, obj);
        int result = grvbld_execute_build_cmd(config, cmd);
        if (result != 0) return result;
    }

    char* link_cmd = grvbld_cstr_new_with_format(
        "ld -shared -o %s/lib%s.so", config->build_dir, target->name);

    for (size_t i = 0; i < obj_files.size; i++) {
        link_cmd = grvbld_cstr_append_arg(link_cmd, obj_files.data[i]);
    }

    grvbld_strarr_t data_obj_files = grvbld_build_data_files(config, target);
    for (size_t i = 0; i < data_obj_files.size; ++i) {
        link_cmd = grvbld_cstr_append_arg(link_cmd, data_obj_files.data[i]);
    }

    for (size_t i = 0; i < target->libs.size; i++) {
        link_cmd = grvbld_cstr_append_arg(link_cmd, "-l");
        link_cmd = grvbld_cstr_append(link_cmd, target->libs.data[i]);
    }
    int result = grvbld_execute_link_cmd(config, link_cmd);
    return result;
}

GRVBLD_INLINE int grvbld_build_target(grvbld_config_t* config, grvbld_target_t* target) {
    create_build_path(config);

    char* cmd = grvbld_build_cmd(config);
    
    if (target->type == GRVBLD_STATIC_LIBRARY 
        || (target->type == GRVBLD_LIBRARY && !config->dynamic)) {
        return grvbld_build_static_library(config, target);
    } else if (target->type == GRVBLD_DYNAMIC_LIBRARY
        || (target->type == GRVBLD_LIBRARY && config->dynamic)) {
        return grvbld_build_dynamic_library(config, target);
    } else if (target->type == GRVBLD_EXECUTABLE) {
        log_info("BUILDING EXECUTABLE %s", target->name);
        char* dst_file = grvbld_cstr_new_with_format("%s/%s", config->build_dir, target->name);
        cmd = grvbld_cstr_append_arg_format(cmd, "-o %s", dst_file);

        for (size_t i = 0; i < target->compile_options.size; i++) {
            cmd = grvbld_cstr_append_arg(cmd, target->compile_options.data[i]);
        }

        for (size_t i = 0; i < target->src_files.size; ++i) {
            cmd = grvbld_cstr_append_arg(cmd, target->src_files.data[i]);
        }
    
        for (size_t i = 0; i < target->libs.size; ++i) {
            cmd = grvbld_cstr_append_arg_format(cmd, "-l%s", target->libs.data[i]);
        }

        for (size_t i = 0; i < target->linked_targets.size; ++i) {
            grvbld_target_t* linked_target = target->linked_targets.arr[i];
            if (linked_target->type == GRVBLD_STATIC_LIBRARY || linked_target->type == GRVBLD_LIBRARY) {
                cmd = grvbld_cstr_append_arg_format(cmd, "-l%s", linked_target->name);
                for (size_t j = 0; j < linked_target->libs.size; ++j) {
                    cmd = grvbld_cstr_append_arg_format(cmd, "-l%s", linked_target->libs.data[j]);
                }
            }
        }        

        cmd = grvbld_cstr_append_arg(cmd, "-lm");

        for (size_t i = 0; i < target->link_options.size; i++) {
            cmd = grvbld_cstr_append_arg(cmd, target->link_options.data[i]);
        }
            
        int result = grvbld_execute_build_cmd(config, cmd);

        if (result == 0 && target->run_after_build) {
            char* debug_cmd = grvbld_cstr_new_with_format("gdb -tui -q -ex run %s", dst_file);
            system(debug_cmd);
        }
        return result;
    } else {
        log_error("Unknown target type");
        return 1;
    }   
}



#endif
