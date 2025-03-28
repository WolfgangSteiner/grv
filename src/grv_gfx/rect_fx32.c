#include "grv_gfx/rect_fx32.h"


rect_fx32 rect_fx32_align_to_rect(
    rect_fx32 rect,
    rect_fx32 to_rect,
    grv_alignment_t alignment)
{
    if (alignment & GRV_ALIGNMENT_HORIZONTAL_LEFT) {
        rect.x = to_rect.x;
    } else if (alignment & GRV_ALIGNMENT_HORIZONTAL_RIGHT) {
        rect.x.val = to_rect.x.val + to_rect.w.val - rect.w.val;
    } else if (alignment & GRV_ALIGNMENT_HORIZONTAL_CENTER) {
        rect.x.val = to_rect.x.val + (to_rect.w.val - rect.w.val) / 2;
    } else if (alignment & GRV_ALIGNMENT_HORIZONTAL_BLOCK) {
        rect.x = to_rect.x;
        rect.w = to_rect.w;
    } else if (alignment & GRV_ALIGNMENT_HORIZONTAL_LEFT_OF) {
        rect.x.val = to_rect.x.val - rect.w.val;
    } else if (alignment & GRV_ALIGNMENT_HORIZONTAL_RIGHT_OF) {
        rect.x.val = to_rect.x.val + rect.w.val;
    }

    if (alignment & GRV_ALIGNMENT_VERTICAL_TOP) {
        rect.y = to_rect.y;
    } else if (alignment & GRV_ALIGNMENT_VERTICAL_BOTTOM) {
        rect.y.val = to_rect.y.val + to_rect.h.val - rect.h.val;
    } else if (alignment & GRV_ALIGNMENT_VERTICAL_CENTER) {
        rect.y.val = to_rect.y.val + (to_rect.h.val - rect.h.val) / 2;
    } else if (alignment & GRV_ALIGNMENT_VERTICAL_BLOCK) {
        rect.y = to_rect.y;
        rect.h = to_rect.h;
    } else if (alignment & GRV_ALIGNMENT_VERTICAL_ABOVE) {
        rect.y.val = to_rect.y.val - rect.h.val;
    } else if (alignment & GRV_ALIGNMENT_VERTICAL_BELOW) {
        rect.y.val = to_rect.y.val + rect.h.val;
    }

    return rect;
}
