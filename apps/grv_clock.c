#include "grv_gfx/grv_window.h"
#include "grv/grv_log.h"
#include "grv_gfx/grv_bitmap_font.h"
#include "grv/grv_cstr.h"
#include "grv/grv_util.h"
#include <time.h>


u8 color_for_time(void) {
    f32 time = grv_local_time_f32();
    if (time < 6.0f || time > 23.0f) {
        return 8;
    } else if (time > 22.0f) {
        return 9;
    } else {
        return 11;
    }
}

void draw_seconds(grv_framebuffer_t* fb) {
    struct tm tm = grv_local_time();
    recti_t rect = {0, 0, tm.tm_sec, fb->height};
    grv_framebuffer_fill_rect_u8(fb, rect, 1);
}

int main(int argc, char** argv) {
    GRV_UNUSED(argc);
    GRV_UNUSED(argv);
    int window_width = 60;
    int window_height = 12;
    grv_window_t* w = grv_window_new(window_width, window_height, 16.0, grv_str_ref("Hello World"));
    grv_color_palette_init_with_type(&w->framebuffer.palette, GRV_COLOR_PALETTE_PICO8);
    w->borderless = true;
    grv_window_show(w);
    grv_bitmap_font_t* font = grv_get_cozette_font();

    while (true) {
        grv_window_poll_events();
        if (w->should_close) {
            break;
        }

        grv_framebuffer_clear(&w->framebuffer);
        draw_seconds(&w->framebuffer);
        struct tm tm = grv_local_time();
        grv_str_t time_str = grv_str_new_with_format("%02d:%02d", tm.tm_hour, tm.tm_min);
        int str_width = grv_bitmap_font_calc_size(font, time_str).x;
        int x = (window_width - str_width) / 2;
        grv_put_text_u8(&w->framebuffer, time_str, (vec2i){x, 2}, font, color_for_time());
        grv_window_present(w);
        grv_str_free(&time_str);
        grv_sleep(0.5);
    }

    return 0;
}
