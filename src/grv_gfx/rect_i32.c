#include "grv_gfx/rect_i32.h"

rect_i32 rect_i32_align_to_rect(rect_i32 rect, rect_i32 to_rect, grv_alignment_t alignment) {
    if (alignment & GRV_ALIGNMENT_HORIZONTAL_LEFT) {
        rect.x = to_rect.x;
    } else if (alignment & GRV_ALIGNMENT_HORIZONTAL_RIGHT) {
        rect.x = to_rect.x + to_rect.w - rect.w;
    } else if (alignment & GRV_ALIGNMENT_HORIZONTAL_CENTER) {
        rect.x = to_rect.x + (to_rect.w - rect.w) / 2;
    } else if (alignment & GRV_ALIGNMENT_HORIZONTAL_BLOCK) {
        rect.x = to_rect.x;
        rect.w = to_rect.w;
    } else if (alignment & GRV_ALIGNMENT_HORIZONTAL_LEFT_OF) {
        rect.x = to_rect.x - rect.w;
    } else if (alignment & GRV_ALIGNMENT_HORIZONTAL_RIGHT_OF) {
        rect.x = to_rect.x + to_rect.w;
    }

    if (alignment & GRV_ALIGNMENT_VERTICAL_TOP) {
        rect.y = to_rect.y;
    } else if (alignment & GRV_ALIGNMENT_VERTICAL_BOTTOM) {
        rect.y = to_rect.y + to_rect.h - rect.h;
    } else if (alignment & GRV_ALIGNMENT_VERTICAL_CENTER) {
        rect.y = to_rect.y + (to_rect.h - rect.h) / 2;
    } else if (alignment & GRV_ALIGNMENT_VERTICAL_BLOCK) {
        rect.y = to_rect.y;
        rect.h = to_rect.h;
    } else if (alignment & GRV_ALIGNMENT_VERTICAL_ABOVE) {
        rect.y = to_rect.y - rect.h;
    } else if (alignment & GRV_ALIGNMENT_VERTICAL_BELOW) {
        rect.y = to_rect.y + to_rect.h;
    }

    return rect;
}

rect_i32 rect_i32_split_lower(rect_i32 rect, i32 upper_weight, i32 lower_weight) {
	i32 h = rect.h * lower_weight / (upper_weight + lower_weight);
	rect.y += rect.h - h;
	rect.h = h;
	return rect;
}
