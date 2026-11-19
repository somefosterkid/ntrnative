#include "global.h"

void ov18_021F4EB0(s32 arg0, s16 *arg1, s16 *arg2);

void ov18_021F4EB0(s32 arg0, s16 *arg1, s16 *arg2) {
    f32 var_r0;
    f32 var_r0_2;
    s16 temp_r6_2;
    s16 temp_r7;
    int temp_r6;
    s32 var_r0_i;
    s32 var_r0_2_i;

    temp_r6 = (arg0 >> 4) * 2;
    temp_r7 = FX_SinCosTable_[temp_r6];
    if (((f32)temp_r7 / 4096.0f) > 0.0f) {
        var_r0 = 0.5f + (4096.0f * ((f32)temp_r7 / 4096.0f));
    } else {
        var_r0 = (4096.0f * ((f32)temp_r7 / 4096.0f)) - 0.5f;
    }
    var_r0_i = (s32)var_r0;
    temp_r6_2 = FX_SinCosTable_[temp_r6 + 1];
    if (((f32)temp_r6_2 / 4096.0f) > 0.0f) {
        var_r0_2 = 0.5f + (4096.0f * ((f32)temp_r6_2 / 4096.0f));
    } else {
        var_r0_2 = (4096.0f * ((f32)temp_r6_2 / 4096.0f)) - 0.5f;
    }
    var_r0_2_i = (s32)var_r0_2;
    *arg1 += (s32)(var_r0_i * 0x38) >> 0xC;
    *arg2 += (s32)(var_r0_2_i * 0x38) >> 0xC;
}
