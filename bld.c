#include "grvbld.h"

int main(int argc, char** argv) {
    GRV_CHECK_AND_REBUILD();    
    grvbld_config_t* config =  grvbld_config_new(argc, argv);
    grvbld_strarr_push(&config->warnings, "-Wextra -Wpedantic");

    grvbld_target_t* libgrv = grvbld_target_create("grv", GRVBLD_STATIC_LIBRARY);
    grvbld_target_add_src(libgrv, "src/grv.c");
    grvbld_build_target(config, libgrv);

    grvbld_target_t* libgrvgfx = grvbld_target_create("grvgfx", GRVBLD_STATIC_LIBRARY);
    grvbld_target_add_src(libgrvgfx, "src/grv_gfx/grv_gfx.c");
    grvbld_target_link_library(libgrvgfx, "SDL2");
    grvbld_target_add_data_file(libgrvgfx, "src/grv_gfx/cozette.psf");
    grvbld_build_target(config, libgrvgfx);

    grvbld_run_tests(config);
    
    grvbld_target_t* grv_clock = grvbld_target_create("grv_clock", GRVBLD_EXECUTABLE);
    grvbld_target_add_src(grv_clock, "apps/grv_clock.c");
    grvbld_target_link(grv_clock, libgrv);
    grvbld_target_link(grv_clock, libgrvgfx);
    grvbld_build_target(config, grv_clock);

    grvbld_target_t* grv_color_palette = grvbld_target_create("grv_color_palette", GRVBLD_EXECUTABLE);
    grvbld_target_add_src(grv_color_palette, "apps/grv_color_palette.c");
    grvbld_target_link(grv_color_palette, libgrv);
    grvbld_target_link(grv_color_palette, libgrvgfx);
    grvbld_build_target(config, grv_color_palette);
    
    grvbld_target_t* test_window = grvbld_target_create("test_window", GRVBLD_EXECUTABLE);
    grvbld_target_add_src(test_window, "src/grv_gfx/test_window.c");
    grvbld_target_link(test_window, libgrv);
    grvbld_target_link(test_window, libgrvgfx);
    //test_window->run_after_build = true;
    grvbld_build_target(config, test_window);

    return 0;
}

