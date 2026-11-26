#include "global.h"
#include "overlay_01_choose_starter_app.h"

s32 _ll_mul(s32, s32, s32, s32);

s32 ov96_021EB06C(s32, s32, s32, s32 *, s32 *);

extern s16 ov96_0221D6D4[][2];

s32 ov96_02218934(void *arg0);

s32 ov96_02218934(void *arg0) {
    s32 sp8;
    s32 sp4;
    s32 spC[12];
    f32 var_r0;
    s32 temp_r2;
    s32 temp_r3;
    s32 temp_r4;
    s32 temp_r7;
    s32 i;
    s32 lo;
    s32 hi;

    temp_r3 = *(s32 *)((u8 *)arg0 + 0x2C);
    if ((temp_r3 <= 0) || (temp_r2 = *(s32 *)((u8 *)arg0 + 0x30), (temp_r2 <= 0))) {
        return 1;
    }
    ov96_021EB06C(**(s32 **)((u8 *)arg0 + 4), temp_r3 >> 0xC, temp_r2 >> 0xC, &sp8, &sp4);
    if ((sp4 >= 0x40) && (sp4 <= 0xA0)) {
        if (sp8 > 0) {
            var_r0 = 0.5f + (f32)(sp8 << 0xC);
        } else {
            var_r0 = (f32)(sp8 << 0xC) - 0.5f;
        }
        temp_r4 = (s32)var_r0;
        if (sp4 > 0) {
            var_r0 = 0.5f + (f32)(sp4 << 0xC);
        } else {
            var_r0 = (f32)(sp4 << 0xC) - 0.5f;
        }
        temp_r7 = (s32)var_r0;
        for (i = 0; i < 4; i++) {
            spC[i * 3] = ov96_0221D6D4[i][0] << 0xC;
            spC[i * 3 + 1] = ov96_0221D6D4[i][1] << 0xC;
        }
        lo = FX_Div(FX_MUL(spC[4] - spC[1], temp_r4 - spC[0]), spC[3] - spC[0]);
        lo += spC[1];
        hi = FX_Div(FX_MUL(spC[10] - spC[7], temp_r4 - spC[6]), spC[9] - spC[6]);
        hi += spC[7];
        if ((temp_r7 >= lo) && (temp_r7 >= hi)) {
            return 0;
        }
    }
    return 1;
}
