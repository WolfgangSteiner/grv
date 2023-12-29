#include "grv/grvbld.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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

//==============================================================================
// grvbld_strarr
//==============================================================================
static void grvbld_strarr_init(grvbld_strarr_t* arr) {
    arr->size = 0;
    arr->capacity = 16;
    arr->data = calloc(arr->capacity, sizeof(char*));
}

static void grvbld_strarr_push(grvbld_strarr_t* arr, char* str) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(char*));
    }
    arr->data[arr->size] = malloc(strlen(str) + 1);
    strcpy(arr->data[arr->size], str);
    arr->size++;
}

static void grvbld_strarr_dup(grvbld_strarr_t* dst, grvbld_strarr_t* src) {
    dst->size = src->size;
    dst->capacity = src->capacity;
    dst->data = calloc(dst->capacity, sizeof(char*));
    for (size_t i = 0; i < src->size; ++i) {
        dst->data[i] = malloc(strlen(src->data[i]) + 1);
        strcpy(dst->data[i], src->data[i]);
    }
}

//==============================================================================
// grvbld_str 
//==============================================================================
typedef struct {
    char* str;
    size_t size;
} grvbld_str_t;


static size_t grvbld_str_len(grvbld_str_t* str) {
    char* c = str->str;
    size_t len = 0;
    while (*c != '\0' && len < str->size) {
        ++c;
        ++len;
    }

    return len;
}

static void grvbld_str_init(grvbld_str_t* str, char* src) {
    str->size = strlen(src) + 1;
    str->str = malloc(str->size);
    memcpy(str->str, src, strlen(src) + 1);
}

static grvbld_str_t* grvbld_str_alloc(char* src) {
    grvbld_str_t* str = calloc(1, sizeof(grvbld_str_t));
    grvbld_str_init(str, src);
    return str;
}

static grvbld_str_t* grvbld_str_copy(grvbld_str_t* src) {
    grvbld_str_t* dst = grvbld_str_alloc(src->str);    
}

static void grvbld_str_resize(grvbld_str_t* str, size_t size) {
    if (str->size < size) {
        str->size = size;
        str->str = realloc(str->str, str->size);
    }
}

static void grvbld_str_append(grvbld_str_t* dst, char* src) {
    size_t src_len = strlen(src);
    size_t dst_len = grvbld_str_len(dst);
    grvbld_str_resize(dst, src_len + dst_len + 1);
    memcpy(dst->str + dst_len, src, src_len);
    dst->str[dst_len + src_len] = 0;
}

static void grvbld_str_append_char(grvbld_str_t* dst, char c) {
    size_t dst_len = grvbld_str_len(dst);
    grvbld_str_resize(dst, dst_len + 2);
    dst->str[dst_len] = c;
    dst->str[dst_len + 1] = 0;
}

static void grvbld_str_append_path(grvbld_str_t* dst, char* path) {
    grvbld_str_append_char(dst, GRVBLD_PATH_SEP);
    grvbld_str_append(dst, path);
}

static void grvbld_str_append_arg(grvbld_str_t* dst, char* arg) {
    grvbld_str_append(dst, " ");
    grvbld_str_append(dst, arg);
}

static grvbld_str_t* grvbld_str_filename(grvbld_str_t* str) {
    char* last_sep = strrchr(str->str, GRVBLD_PATH_SEP);
    if (last_sep) {
        return grvbld_str_alloc(last_sep + 1);
    } else {
        return grvbld_str_copy(str);
    } 
}

static void grvbld_str_remove_ext(grvbld_str_t* str) {
    char* dot = strrchr(str->str, '.');
    if (dot) {
        *dot = 0;
    }
}

static void grvbld_str_replace_ext(grvbld_str_t* str, char* ext) {
    grvbld_str_remove_ext(str);
    grvbld_str_append(str, ext);
}

static grvbld_str_t* grvbld_str_prepend_path(grvbld_str_t* str, char* path) {
    grvbld_str_t* result = grvbld_str_alloc(path);
    grvbld_str_append_path(result, str->str);
    return result;
}

static bool starts_with(char* str, char* prefix) {
    size_t prefix_len = strlen(prefix);
    return strncmp(str, prefix, prefix_len) == 0;
}

static bool ends_with(char* str, char* suffix) {
    size_t suffix_len = strlen(suffix);
    size_t grv_str_len = strlen(str);
    return strncmp(str + grv_str_len - suffix_len, suffix, suffix_len) == 0;
}

//==============================================================================
// grvbld_config_t
//==============================================================================
void grvbld_config_init(grvbld_config_t* config) {
    config->cc = "gcc";
    config->std = "gnu11";
    grvbld_strarr_init(&config->inc);
    grvbld_strarr_init(&config->warnings);
    grvbld_strarr_init(&config->libs);
    grvbld_strarr_init(&config->library_dirs);
    grvbld_strarr_init(&config->defines);
    config->build_dir = "_build";
    config->test_dir = "test";
    config->debug = true;
    config->treat_warnings_as_errors = false;
    config->verbosity = 0;
    config->use_ccache = true;

    grvbld_strarr_push(&config->inc, "include");
    grvbld_strarr_push(&config->warnings, "-Wall");
//    grvbld_strarr_push(&config->warnings, "-Wextra");
//    grvbld_strarr_push(&config->warnings, "-Wpedantic");

    grvbld_strarr_push(&config->libs, "-lm");
}


char* grvbld_strcpy(char* src) {
    char* dst = malloc(strlen(src) + 1);
    strcpy(dst, src);
    return dst;
}

grvbld_config_t* grvbld_config_dup(grvbld_config_t* src) {
    grvbld_config_t* dst = calloc(1, sizeof(grvbld_config_t));
    memcpy(dst, src, sizeof(grvbld_config_t));
    dst->cc = grvbld_strcpy(src->cc);
    dst->std = grvbld_strcpy(src->std);
    grvbld_strarr_dup(&dst->inc, &src->inc); 
    grvbld_strarr_dup(&dst->warnings, &src->warnings);
    grvbld_strarr_dup(&dst->libs, &src->libs);
    grvbld_strarr_dup(&dst->defines, &src->defines);
    dst->build_dir = grvbld_strcpy(src->build_dir);
    dst->test_dir = grvbld_strcpy(src->test_dir);
    return dst;
}

//==============================================================================
// utility functions
//==============================================================================

static bool has_directory(char* path) {
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

static bool create_directory(char* path) {
#ifdef _WIN32
    return CreateDirectoryA(path, NULL);
#else
    return mkdir(path, 0777) == 0;
#endif
}

static bool make_path(char* path) {
    bool result = false;
    char* p = malloc(strlen(path) + 1);
    strcpy(p, path);
    char* s = p;
    char* e = strchr(s, GRVBLD_PATH_SEP);    
    while (e) {
        *e = 0;
        if (!has_directory(path)) {
            if (!create_directory(path)) {
                goto end;
            }
        }
        s = e + 1;
        e = strchr(s, GRVBLD_PATH_SEP);
    }   

    if (!has_directory(s)) {
        if (!create_directory(s)) {
            goto end;
        }
    }

    result = true;

end:
    free(p);
    return result;
}

static grvbld_strarr_t* get_files_in_dir(char* path) {
    if (!has_directory(path)) {
        printf("[ERROR] path \"%s\" does not exist\n", path);
        exit(1);
    }

    grvbld_strarr_t* result = calloc(1, sizeof(grvbld_strarr_t));
    grvbld_strarr_init(result);


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

static void remove_ext(char* str) {
    char* dot = strrchr(str, '.');
    if (dot) {
        *dot = 0;
    }
}

//==============================================================================
// grvbld_target_t
//==============================================================================
grvbld_target_t* grvbld_target_create(char* name, grvbld_target_type_t type) {
    grvbld_target_t* target = calloc(1, sizeof(grvbld_target_t));
    target->name = grvbld_strcpy(name);
    target->type = type;
    grvbld_strarr_init(&target->src_files);
    return target;
}

void grvbld_target_add_src(grvbld_target_t* target, char* src) {
    grvbld_strarr_push(&target->src_files, src);
}


//==============================================================================
// grvbld main functions
//==============================================================================
static void create_build_path(grvbld_config_t* config) {
    bool make_path_ok = make_path(config->build_dir);
    if (!make_path_ok) {
        printf("[ERROR] failed to create path \"%s\"\n", config->build_dir);
        exit(1);
    }
}

static grvbld_str_t* grvbld_build_cmd(grvbld_config_t* config) {
    grvbld_str_t* cmd = grvbld_str_alloc("");
    if (config->use_ccache) {
        grvbld_str_append(cmd, "ccache ");
        grvbld_str_append_arg(cmd, config->cc);
    } else {
        grvbld_str_append(cmd, config->cc);
    }
    grvbld_str_append_arg(cmd, "-std=");
    grvbld_str_append(cmd, config->std);
    
    for (size_t i = 0; i < config->inc.size; ++i) {
        grvbld_str_append_arg(cmd, "-I");
        grvbld_str_append(cmd, config->inc.data[i]);
    }

    for (size_t i = 0; i < config->warnings.size; ++i) {
        grvbld_str_append_arg(cmd, config->warnings.data[i]);
    }

    if (config->debug) {
        grvbld_str_append_arg(cmd, "-g");
    }

    if (config->treat_warnings_as_errors) {
        grvbld_str_append_arg(cmd, "-Werror");
    }

    for (size_t i = 0; i < config->libs.size; ++i) {
        grvbld_str_append_arg(cmd, config->libs.data[i]);
    }

    for (size_t i = 0; i < config->defines.size; ++i) {
        grvbld_str_append_arg(cmd, "-D");
        grvbld_str_append(cmd, config->defines.data[i]);
    }

    if (config->debug) {
        grvbld_str_append_arg(cmd, "-D");
        grvbld_str_append(cmd, "GRV_DEBUG_MEMORY");
    }

    grvbld_str_append_arg(cmd, "-L");
    grvbld_str_append(cmd, config->build_dir);

    return cmd;
}

int grvbld_test(grvbld_config_t* config, char* name) {
    config = grvbld_config_dup(config);    
    config->debug = true;
    grvbld_strarr_push(&config->defines, "GRV_DEBUG_MEMORY");

    grvbld_str_t* dst_dir = grvbld_str_alloc(config->build_dir);
    grvbld_str_append_path(dst_dir, "test");
    bool make_path_ok = make_path(dst_dir->str);
    if (!make_path_ok) {
        printf("[ERROR] failed to create path \"%s\"\n", dst_dir->str);
        exit(1);
    }

    grvbld_str_t* cmd = grvbld_build_cmd(config);
    
    grvbld_str_t* src_file = grvbld_str_alloc(config->test_dir);
    grvbld_str_append_path(src_file, name);
    grvbld_str_append(src_file, ".c"); 

    grvbld_str_t* dst_file = grvbld_str_alloc(config->build_dir);
    grvbld_str_append_path(dst_file, "test");
    grvbld_str_append_path(dst_file, name);

    grvbld_str_append_arg(cmd, "-o");
    grvbld_str_append_arg(cmd, dst_file->str);

    grvbld_str_append_arg(cmd, src_file->str);

    grvbld_str_append_arg(cmd, "-lgrv");
    // grvbld_str_append_arg(cmd, "src/grv.c");

    if (config->verbosity > 0) {
        printf("[INFO] %s\n", cmd->str);
    }
    
    int result = system(cmd->str);
    if (result != 0) {
        printf("[ERROR] failed to build test \"%s\"\n", name);
        exit(1);
    }

    system(dst_file->str);
}

int grvbld_run_tests(grvbld_config_t* config) {
    grvbld_strarr_t* test_files = get_files_in_dir(config->test_dir);
    for (size_t i = 0; i < test_files->size; ++i) {
        char* test_file = test_files->data[i];
        if (starts_with(test_file, "test_")
            && ends_with(test_file, ".c")) {
            remove_ext(test_file);
            grvbld_test(config, test_file);
        }
    }
}


static int grvbld_build_static_library(grvbld_config_t* config, grvbld_target_t* target) {
    grvbld_str_t* lib_file = grvbld_str_alloc(config->build_dir);
    grvbld_str_append_path(lib_file, "lib");
    grvbld_str_append(lib_file, target->name);
    grvbld_str_append(lib_file, ".a");

    grvbld_str_t* ar_cmd = grvbld_str_alloc("ar rcs");
    grvbld_str_append_arg(ar_cmd, lib_file->str);

    grvbld_str_t* cmd = grvbld_build_cmd(config);
    grvbld_str_append_arg(cmd, "-c");

    // compile all of the source files
    for (size_t i = 0; i < target->src_files.size; ++i) {
        grvbld_str_t* src_file = grvbld_str_alloc(target->src_files.data[i]);
        grvbld_str_t* obj_file = grvbld_str_filename(src_file);
        obj_file = grvbld_str_prepend_path(obj_file, config->build_dir);
        grvbld_str_replace_ext(obj_file, ".o");
        grvbld_str_append_arg(cmd, src_file->str);
        grvbld_str_append_arg(cmd, "-o");
        grvbld_str_append_arg(cmd, obj_file->str);
        grvbld_str_append_arg(ar_cmd, obj_file->str);

        printf("[INFO] %s\n", cmd->str);
        system(cmd->str);
    }

    printf("[INFO] %s\n", ar_cmd->str);
    system(ar_cmd->str);
}

int grvbld_build_target(grvbld_config_t* config, grvbld_target_t* target) {
    create_build_path(config);

    grvbld_str_t* cmd = grvbld_build_cmd(config);
    
    if (target->type == GRVBLD_STATIC_LIBRARY) {
        grvbld_build_static_library(config, target);
    } else if (target->type == GRVBLD_EXECUTABLE) {
        grvbld_str_t* dst_file = grvbld_str_alloc(config->build_dir);
        grvbld_str_append_path(dst_file, target->name);

        grvbld_str_append_arg(cmd, "-o");
        grvbld_str_append_arg(cmd, dst_file->str);

        for (size_t i = 0; i < target->src_files.size; ++i) {
            grvbld_str_append_arg(cmd, target->src_files.data[i]);
        }
    
        system(cmd->str);
    }
}