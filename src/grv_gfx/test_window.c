#include "grv_gfx/grv_window.h"
#include "grv/grv_log.h"
#include "grv_gfx/grv_bitmap_font.h"
#include "grv/grv_cstr.h"
#include "grv/grv_util.h"
#include <time.h>

int main(int, char**) {
    grv_window_t* w = grv_window_new(68, 12, 16.0, grv_str_ref("Hello World"));
    grv_color_palette_init_with_type(&w->frame_buffer.palette, GRV_COLOR_PALETTE_PICO8);
    w->borderless = true;
    grv_window_show(w);

    while (true) {
        grv_window_poll_events();
        if (w->should_close) {
            break;
        }

        grv_frame_buffer_clear(&w->frame_buffer);

        // get current time
        grv_str_t time_str = grv_local_time_str();
        grv_put_text_u8(&w->frame_buffer, time_str, (vec2i){2, 2}, NULL, 11);
        grv_window_present(w);
        grv_str_free(&time_str);
        grv_sleep(0.5);
    }

    return 0;
}