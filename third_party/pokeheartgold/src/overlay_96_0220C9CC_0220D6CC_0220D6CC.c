#include "global.h"

#include "fx_util.h"

extern VecFx32 ov96_0221CF00;

s32 ov96_0220D6CC(VecFx32 *arg0);

s32 ov96_0220D6CC(VecFx32 *arg0) {
    VecFx32 spC = ov96_0221CF00;
    VecFx32 sp0 = { 0, 0, 0 };
    u32 temp_r0;

    sp0.x = arg0->y;
    sp0.z = arg0->x;
    sp0.y = 0;
    temp_r0 = VEC_AngleBetween(&spC, &sp0);
    if ((temp_r0 <= 0x2000U) || (temp_r0 >= 0xE000U)) {
        return 4;
    }
    if ((temp_r0 > 0x2000U) && (temp_r0 < 0x6000U)) {
        return 2;
    }
    if ((temp_r0 >= 0x6000U) && (temp_r0 <= 0xA000U)) {
        return 3;
    }
    return 1;
}
