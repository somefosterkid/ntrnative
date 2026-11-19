#include "global.h"

#include "math_util.h"
#include "overlay_02_02245B80.h"


s32 _ll_mul(s32, s32, s32, s32);

s32 ov02_0224663C(LegendCameraShake *arg0) {
    f32 var_r0;
    s16 temp_r2_3;
    s16 temp_r2_4;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_4;
    s32 temp_r6;
    s32 var_r3;
    u16 temp_r2_2;

    if ((arg0->frame)++ != 0) {
        temp_r2_2 = arg0->frame;
        arg0->frame += 1;
        var_r0 = 0.5f + (f32)(temp_r2_2 << 0xC);
    } else {
        temp_r2_2 = arg0->frame;
        arg0->frame += 1;
        var_r0 = (f32)(temp_r2_2 << 0xC) - 0.5f;
    }
    temp_r0 = (s32)var_r0;
    temp_r0_2 = arg0->angleStep;
    temp_r0_4 = GF_SinDegFX32(FX_MUL(temp_r0_2, temp_r0));
    temp_r2_3 = arg0->amplitudeX;
    temp_r6 = temp_r0_4 >> 0x1F;
    arg0->target.x = (s32)(arg0->origin.x + FX_MUL(temp_r0_4, (s32)temp_r2_3));
    temp_r2_4 = arg0->amplitudeY;
    var_r3 = 0;
    arg0->target.y = (s32)(arg0->origin.y + FX_MUL(temp_r0_4, (s32)temp_r2_4));
    if ((u32) arg0->frame < (u32) arg0->frames) {
        return 0;
    }
    arg0->target.x = (s32) arg0->origin.x;
    arg0->target.y = (s32) arg0->origin.y;
    arg0->frame = 0U;
    arg0->repeat = (u16)(arg0->repeat - 1);
    if (arg0->repeat == 0) {
        var_r3 = 1;
        return var_r3;
    }
    return var_r3;
}
