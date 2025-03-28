#include "grv_gfx/grv_window.h"
#include "grv/grv_log.h"
#include "grv_gfx/grv_bitmap_font.h"
#include <time.h>


int main(int argc, char** argv) {
    GRV_UNUSED(argc);
    GRV_UNUSED(argv);
    int window_width = 128;
    int window_height = 128;
    grv_window_t* w = grv_window_new(window_width, window_height, 4, grv_str_ref("grv_color_palette"));
    w->use_int_scaling = false;
    grv_color_palette_init_with_type(&w->framebuffer.palette, GRV_COLOR_PALETTE_PICO8);
    grv_window_show(w);

    while (!w->should_close) {
        grv_window_poll_events();
        grv_framebuffer_clear(&w->framebuffer);
        
        for (i32 y = 0; y < 4; y++) {
            for (i32 x = 0; x < 4; x++) {
                u8 color = y * 4 + x;
                u32 size = 32;
                rect_i32 rect = {x*size, y*size, size, size};
                grv_framebuffer_fill_rect_u8(&w->framebuffer, rect, color);
                grv_str_t index_str = grv_str_from_int(color);
                vec2i text_size = grv_bitmap_font_calc_size(NULL, index_str);
                rect_i32 text_rect = {0, 0, text_size.x, text_size.y};
                text_rect = rect_i32_center_in_rect(text_rect, rect);
                grv_put_text_u8(&w->framebuffer, index_str, rect_i32_pos(text_rect), NULL, color | 0x80);
            }
        }

        grv_window_present(w);
        grv_sleep(1/30.0f);
    }

    return 0;
}
