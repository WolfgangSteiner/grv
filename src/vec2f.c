#include "grv/vec2f.h"
#include "grv/vec2i.h"
#include "grv/grv_common.h"
#include "grv/grv_math.h"


vec2i vec2f_round(vec2f a) { return (vec2i){grv_round_f32(a.x), grv_round_f32(a.y)}; }

vec2f vec2f_rot(vec2f a, f32 rot) {
    f32 sinv = unit_sin_f32(rot);
    f32 cosv = unit_cos_f32(rot);
    return (vec2f){ cosv * a.x - sinv * a.y, sinv * a.x + cosv * a.y };
}

vec2f vec2f_lerp(vec2f a, vec2f b, f32 t) {
    return vec2f_add(a, vec2f_smul(vec2f_sub(b, a), t));
    //return (vec2f){a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}
