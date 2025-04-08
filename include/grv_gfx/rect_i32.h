#ifndef GRV_RECT_I32_H
#define GRV_RECT_I32_H

#include "grv/grv_base.h"
#include "grv/vec2i.h"
#include "grv/grv_common.h"
#include "grv_gfx/grv_alignment.h"

typedef struct {
   i32 x, y, w, h;
} rect_i32;

GRV_INLINE rect_i32 rect_i32_from_xy(i32 x1, i32 y1, i32 x2, i32 y2) {
   return (rect_i32){
      .x = x1,
      .y = y1,
      .w = x2 - x1 + 1,
      .h = y2 - y1 + 1,
   };
}

GRV_INLINE i32 rect_i32_xmin(rect_i32 a) { return a.x; }
GRV_INLINE i32 rect_i32_xmax(rect_i32 a) { return a.x + a.w - 1; }
GRV_INLINE i32 rect_i32_ymin(rect_i32 a) { return a.y; }
GRV_INLINE i32 rect_i32_ymax(rect_i32 a) { return a.y + a.h - 1; }

GRV_INLINE vec2_i32 rect_i32_pos(rect_i32 a) { return (vec2_i32){a.x, a.y}; }

GRV_INLINE vec2_i32 rect_i32_center(rect_i32 a) {
	return (vec2_i32) {a.x + a.w/2, a.y + a.h/2};
}

GRV_INLINE rect_i32 rect_i32_move_to(rect_i32 a, vec2_i32 p) {
	a.x = p.x; a.y = p.y; return a;
}

GRV_INLINE rect_i32 rect_i32_shift_x(rect_i32 r, i32 xo) {r.x += xo; return r;}
GRV_INLINE rect_i32 rect_i32_shift_y(rect_i32 r, i32 yo) {r.y += yo; return r;}

GRV_INLINE rect_i32 rect_i32_shrink(rect_i32 r, i32 x, i32 y) {
	r.x += x; r.y += y; r.w -= 2 * x; r.h -= 2 * y;
	return r;
}
GRV_INLINE bool rect_i32_point_inside(rect_i32 rect, vec2_i32 point) {
   return point.x >= rect_i32_xmin(rect) && point.x <= rect_i32_xmax(rect) &&
          point.y >= rect_i32_ymin(rect) && point.y <= rect_i32_ymax(rect);
}

GRV_INLINE rect_i32 rect_i32_intersect_rect(rect_i32 a, rect_i32 b) {
   rect_i32 result;
   result.x = grv_max_i32(a.x, b.x);
   result.y = grv_max_i32(a.y, b.y);
   i32 xmax = grv_min_i32(rect_i32_xmax(a), rect_i32_xmax(b));
   i32 ymax = grv_min_i32(rect_i32_ymax(a), rect_i32_ymax(b));
   result.w = xmax - result.x + 1;
   result.h = ymax - result.y + 1;
   return result;
}

GRV_INLINE rect_i32 rect_i32_center_in_rect(rect_i32 rect, rect_i32 container) {
   return (rect_i32){
      .x = container.x + (container.w - rect.w) / 2,
      .y = container.y + (container.h - rect.h) / 2,
      .w = rect.w,
      .h = rect.h,
   };
}

rect_i32 rect_i32_align_to_rect(rect_i32 rect, rect_i32 to_rect, grv_alignment_t alignment);
rect_i32 rect_i32_split_lower(rect_i32 rect, i32 upper_weight, i32 lower_weight);

#endif
