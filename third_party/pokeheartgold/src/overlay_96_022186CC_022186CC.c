#include "global.h"

#include "fx_util.h"

extern s32 ov96_0221D6B8;

s32 ov96_022186CC(void *arg0);

s32 ov96_022186CC(void *arg0) {
    VecFx32 spC;
    VecFx32 sp0;
    s32 *p;
    u32 temp_r0;

    spC = *(VecFx32 *)((u8 *)&ov96_0221D6B8);
    p = (s32 *)&sp0;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    *(s32 *)((u8 *)&sp0) = ((s32 *)arg0)[1];
    *(s32 *)((u8 *)&sp0 + 8) = ((s32 *)arg0)[0];
    *(s32 *)((u8 *)&sp0 + 4) = 0;
    temp_r0 = (u32)VEC_AngleBetween(&spC, &sp0);
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
