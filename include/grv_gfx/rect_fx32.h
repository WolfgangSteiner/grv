#ifndef RECT_FX32_H
#define RECT_FX32_H

#include "grv/fx32.h"
#include "grv/vec2_fx32.h"
#include "grv_gfx/grv_alignment.h"

typedef struct {
    fx32 x,y,w,h;
} rect_fx32;

GRV_INLINE rect_fx32 rect_fx32_from_i32(i32 x, i32 y, i32 w, i32 h) {
    return (rect_fx32) {
        .x=fx32_from_i32(x),
        .y=fx32_from_i32(y),
        .w=fx32_from_i32(w),
        .h=fx32_from_i32(h),
    };
}

GRV_INLINE rect_i32 rect_fx32_round(rect_fx32 rect) {
	return (rect_i32){
		.x=fx32_round(rect.x),
		.y=fx32_round(rect.y),
		.w=fx32_round(rect.w),
		.h=fx32_round(rect.h)
	};
}

rect_fx32 rect_fx32_align_to_rect(rect_fx32 rect_a, rect_fx32 rect_b, grv_alignment_t alignment);

GRV_INLINE bool rect_fx32_point_inside(rect_fx32 r, vec2_fx32 p) {
    return p.x.val >= r.x.val
        && p.x.val <= r.x.val + r.w.val
        && p.y.val >= r.y.val
        && p.y.val <= r.y.val + r.h.val;
}

GRV_INLINE rect_fx32 rect_fx32_move_to(rect_fx32 r, vec2_fx32 p) {
    return (rect_fx32) {.x=p.x, .y=p.y, .w=r.w, .h=r.h };
}

GRV_INLINE bool rect_fx32_intersect(rect_fx32 a, rect_fx32 b) {
    i32 a_x1 = a.x.val;
    i32 a_x2 = a.x.val + a.w.val;
    i32 b_x1 = b.x.val;
    i32 b_x2 = b.x.val + b.w.val;
    i32 a_y1 = a.y.val;
    i32 a_y2 = a.y.val + a.h.val;
    i32 b_y1 = b.y.val;
    i32 b_y2 = b.y.val + b.h.val;
    return !(a_x2 < b_x1 || a_x1 > b_x2 || a_y2 < b_y1 || a_y1 > b_y2);
}

#endif
