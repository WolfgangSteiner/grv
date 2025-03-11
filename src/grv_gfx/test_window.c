#include "grv_gfx/grv_window.h"
#include "grv/grv_log.h"
#include "grv_gfx/grv_bitmap_font.h"
#include "grv/grv_cstr.h"
#include "grv/grv_util.h"
#include <time.h>

int main(int argc, char** argv) {
    GRV_UNUSED(argc);
    GRV_UNUSED(argv);
    grv_window_t* w = grv_window_new(480, 270, 2.0, grv_str_ref("Hello World"));
    grv_color_palette_init_with_type(&w->framebuffer.palette, GRV_COLOR_PALETTE_PICO8);
    //w->borderless = true;
    w->use_int_scaling = false;
    grv_window_show(w);

    while (true) {
        grv_window_poll_events();
        if (w->should_close) {
            break;
        }

        w->framebuffer.clear_color_u8 = 1;
        grv_framebuffer_clear(&w->framebuffer);

        // get current time
        grv_str_t time_str = grv_local_time_str();
        i32 scale = 7;
        int str_width = scale * grv_bitmap_font_calc_size(NULL, time_str).x;
        int x = (w->width - str_width) / 2;
        int y = (w->height - 8 * scale) / 2;
        grv_put_text_scaled_u8(&w->framebuffer, time_str, (vec2i){x, y}, NULL, scale, 11);
        grv_window_present(w);
        grv_str_free(&time_str);
        grv_sleep(1.0f/30.0f);
    }

    return 0;
}
