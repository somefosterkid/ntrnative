#include "global.h"
#include "math_util.h"
#include "overlay_28.h"

void ov28_0225DE04(VecFx32 *vec, u32 arg1) {
    s32 temp_r0;
    s32 temp_r0_2;

    temp_r0 = GF_CosDeg(arg1);
    vec->x = (FX_MUL(temp_r0, 0x44000) + 0x54000);
    temp_r0_2 = GF_SinDeg(arg1);
    vec->y = (s32)(FX_MUL(temp_r0_2, 0x44000) + 0x164000);
}
