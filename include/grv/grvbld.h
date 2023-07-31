#ifndef GRVBLD_H
#define GRVBLD_H

#include <stdbool.h>
#include <stdlib.h>

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