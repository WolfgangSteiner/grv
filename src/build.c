#ifndef GRV_BUILD_CONFIGURED
#include <stdlib.h>

int main(void) {
    system("gcc -o build -DGRV_BUILD_CONFIGURED -I include src/build.c src/grvbld.c src/cstr.c");
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