#ifndef GRV_RECT_FIXED32_H
#define GRV_RECT_FIXED32_H

#include "grv/grv_fixed32.h"
#include "grv/grv_vec2_fixed32.h"

typedef struct {
    grv_fixed32_t x,y,w,h;
} grv_rect_fixed32_t;


GRV_INLINE bool grv_rect_fixed32_point_inside(grv_rect_fixed32_t r, grv_vec2_fixed32_t p) {
    return p.x.val >= r.x.val
        && p.x.val <= r.x.val + r.w.val
        && p.y.val >= r.y.val
        && p.y.val <= r.y.val + r.h.val;
}

GRV_INLINE grv_rect_fixed32_t grv_rect_fixed32_move_to(grv_rect_fixed32_t r, grv_vec2_fixed32_t p) {
    return (grv_rect_fixed32_t) {.x=p.x, .y=p.y, .w=r.w, .h=r.h };
}

#endif
