#include "grvbld.h"

static void build_exe(grvbld_config_t* config, char* src_file) {
    char* name = grvbld_cstr_filename(src_file);
    name = grvbld_cstr_remove_ext(name);
    grvbld_target_t* target = grvbld_target_create_executable(name);
    grvbld_target_add_src(target, src_file);
    grvbld_target_link_library(target, "grv");
    grvbld_build_target(config, target);
}

int main(int argc, char** argv) {
    GRV_CHECK_AND_REBUILD();    
    grvbld_config_t* config =  grvbld_config_new(argc, argv);
    //config->cc = "tcc";
    //config->use_ccache = false;

    grvbld_target_t* libgrv = grvbld_target_create_library("grv");
    grvbld_target_add_src(libgrv, "src/grv.c");
    grvbld_build_target(config, libgrv);

    if (config->run_tests) {
        int result = grvbld_run_tests(config);
        if (config->tests_only) return result;
    }

    grvbld_target_t* libgrvgfx = grvbld_target_create_library("grvgfx");
    grvbld_target_add_src(libgrvgfx, "src/grv_gfx/grv_gfx.c");
    grvbld_target_link_library(libgrvgfx, "SDL2");
    grvbld_target_add_data_file(libgrvgfx, "src/grv_gfx/cozette.psf");
    grvbld_build_target(config, libgrvgfx);

    build_exe(config, "tools/grvcc.c");
    build_exe(config, "tools/new_test.c");
    
    grvbld_target_t* grv_clock = grvbld_target_create_executable("grv_clock");
    grvbld_target_add_src(grv_clock, "apps/grv_clock.c");
    grvbld_target_link(grv_clock, libgrv);
    grvbld_target_link(grv_clock, libgrvgfx);
    grvbld_build_target(config, grv_clock);

    grvbld_target_t* grv_color_palette = grvbld_target_create_executable("grv_color_palette");
    grvbld_target_add_src(grv_color_palette, "apps/grv_color_palette.c");
    grvbld_target_link(grv_color_palette, libgrv);
    grvbld_target_link(grv_color_palette, libgrvgfx);
    grvbld_build_target(config, grv_color_palette);
    
    grvbld_target_t* test_window = grvbld_target_create_executable("test_window");
    grvbld_target_add_src(test_window, "src/grv_gfx/test_window.c");
    grvbld_target_link(test_window, libgrv);
    grvbld_target_link(test_window, libgrvgfx);
    //test_window->run_after_build = true;
    grvbld_build_target(config, test_window);

    return 0;
}

