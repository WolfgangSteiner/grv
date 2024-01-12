#ifndef GRV_BUILD_CONFIGURED
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* concatenate_arguments(int argc, char** argv) {
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

int main(int argc, char** argv) {
    int result = system("gcc -g -o bld -DGRV_BUILD_CONFIGURED -Iinclude src/bld.c src/grvbld.c");
    if (result != 0) {
        printf("[ERROR] Failed to build bld.c\n");
            return result;
    }
    system(concatenate_arguments(argc, argv));
}
#else

#include "grv/grvbld.h"

int main(int argc, char** argv) {
    GRV_CHECK_AND_REBUILD();    

    grvbld_config_t config;
    grvbld_config_init(&config);
    config.use_ccache = false;

    if (grvbld_args_contain(argc, argv, "--ccache")) config.use_ccache = true;
    if (grvbld_args_contain(argc, argv, "--debug")) config.debug = true;

    grvbld_target_t* libgrv = grvbld_target_create("grv", GRVBLD_STATIC_LIBRARY);
    grvbld_target_add_src(libgrv, "src/grv.c");
    grvbld_build_target(&config, libgrv);

    grvbld_run_tests(&config);
    return 0;
}

#endif