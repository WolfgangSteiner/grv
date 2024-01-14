#include "grv_gfx/grv_window.h"

int main(int, char**) {
    grv_window_t* w = grv_window_new(800, 600, grv_str_ref("Hello World"));
    grv_window_show(w);

    while (!w->should_close) {
        grv_window_poll_events();
    }

    return 0;
}