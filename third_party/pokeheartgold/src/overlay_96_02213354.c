#include "global.h"

void ov96_02213354(s32 x, s32 y, VecFx32 *out);

void ov96_02213354(s32 x, s32 y, VecFx32 *out) {
    out->x = x << 0xc;
    out->y = y << 0xc;
    out->z = 0;
    return;
}
