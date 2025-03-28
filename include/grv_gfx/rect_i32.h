#ifndef GRV_RECT_I32_H
#define GRV_RECT_I32_H

#include "grv/grv_base.h"
#include "grv/vec2i.h"
#include "grv/grv_common.h"

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

GRV_INLINE vec2i rect_i32_pos(rect_i32 a) { return (vec2i){a.x, a.y}; }

GRV_INLINE bool rect_i32_contains_point(rect_i32 rect, vec2i point) {
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

#endif
