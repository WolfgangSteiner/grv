#ifndef GRV_BUILD_CONFIGURED
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int result = system("gcc -g -o build -DGRV_BUILD_CONFIGURED -Iinclude src/build.c src/grvbld.c");
    if (result != 0) {
        printf("[ERROR] Failed to build build.c\n");
        return result;
    }
    system("./build");
}

#else

#include "grv/grvbld.h"

int main(int argc, char** argv) {
    GRV_CHECK_AND_REBUILD();    

    grvbld_config_t config;
    grvbld_config_init(&config);
    config.use_ccache = false;

    grvbld_target_t* libgrv = grvbld_target_create("grv", GRVBLD_STATIC_LIBRARY);
    grvbld_target_add_src(libgrv, "src/grv.c");
    grvbld_build_target(&config, libgrv);

    grvbld_run_tests(&config);
    return 0;
}

#endif