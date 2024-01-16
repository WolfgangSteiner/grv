#include "grv_gfx/grv_window.h"
#include "grv/grv_log.h"
#include "grv_gfx/grv_bitmap_font.h"

int main(int, char**) {
    grv_window_t* w = grv_window_new(800, 600, grv_str_ref("Hello World"));
    grv_window_show(w);

    while (true) {
        grv_window_poll_events();
        if (w->should_close) {
            grv_log_info("Window should close");
            break;
        }

        grv_frame_buffer_clear(&w->frame_buffer);
        grv_put_text_u8(&w->frame_buffer, "Hello World", (vec2i){0, 10}, NULL, 15);
        grv_window_present(w);
    }

    return 0;
}