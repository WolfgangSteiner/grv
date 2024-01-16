#ifndef GRV_RECTI_H
#define GRV_RECTI_H

#include "grv/grv_base.h"

typedef struct {
   s32 x, y, w, h;
} recti_t;

GRV_INLINE s32 recti_xmin(recti_t a) { return a.x; }
GRV_INLINE s32 recti_xmax(recti_t a) { return a.x + a.w - 1; }
GRV_INLINE s32 recti_ymin(recti_t a) { return a.y; }
GRV_INLINE s32 recti_ymax(recti_t a) { return a.y + a.h - 1; }

#endif
