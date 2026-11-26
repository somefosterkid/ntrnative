#include "global.h"
#include "pokeathlon/pokeathlon.h"

void ov96_021E8228(s32, u32, u32, s32, s32);

s32 ov96_022079B8(void *);

s32 ov96_02207A34(s32 arg0, void *arg1, void *arg2);

s32 ov96_02207A34(s32 arg0, void *arg1, void *arg2) {

    s32 temp_r0;
    s32 temp_r6;
    u8 temp_r1;
    u8 temp_r4;
    void *temp_r2;
    void *var_r0;
    void *var_r5;
    void *var_r6_2;
    u32 var_r1;
    temp_r6 = VEC_Mag(arg1 + 0x64);
    temp_r0 = VEC_Mag(arg2 + 0x64);
    if (temp_r6 > temp_r0) {
        var_r6_2 = arg1;
        var_r5 = arg2;
    } else if (temp_r6 < temp_r0) {
        var_r6_2 = arg2;
        var_r5 = arg1;
    } else {
        return 0;
    }
    if ((*((u8 *)(((u8 *)var_r6_2) + 0xB0))) != (*((u8 *)(((u8 *)var_r5) + 0xB0)))) {
        return 0;
    }
    if ((*((u8 *)(((u8 *)var_r6_2) + 0xB0))) != ov96_022079B8(var_r6_2 + 0x64)) {
        return 0;
    }
    if ((*((u8 *)(((u8 *)var_r6_2) + 0xA6))) == 1) {
        return 0;
    }
    temp_r1 = *((s8 *)(((u8 *)var_r5) + 0xAA));
    if (((u32)temp_r1) >= 2U) {
        *((u8 *)(((u8 *)var_r5) + 0xAA)) = (u8)((*((u8 *)(((u8 *)var_r5) + 0xAA))) - 2);
        var_r1 = (*((u8 *)(((u8 *)var_r6_2) + 0xAA))) + 2;
        var_r0 = var_r6_2;
    } else {
        *((u8 *)(((u8 *)var_r6_2) + 0xAA)) = (u8)((*((u8 *)(((u8 *)var_r6_2) + 0xAA))) + temp_r1);
        var_r1 = 0;
        var_r0 = var_r5;
    }
    *((s8 *)(((u8 *)var_r0) + 0xAA)) = var_r1;
    if (((u32)(*((u8 *)(((u8 *)var_r6_2) + 0xAA)))) > 9U) {
        *((u8 *)(((u8 *)var_r6_2) + 0xAA)) = 9U;
    }
    *((s8 *)(((u8 *)var_r5) + 0xA6)) = 1;
    ov96_021E8228(arg0, (u8)(*((s32 *)(((u8 *)var_r5) + 0x98))), *((u8 *)(((u8 *)var_r5) + 0xB1)), 1, 1);
    temp_r4 = *((u8 *)(((u8 *)var_r5) + 0xB1));
    *((s8 *)(((u8 *)var_r5) + 0xA2)) = (s8)((*((s32 *)(((u8 *)(var_r5 + (temp_r4 * 0x14))) + 0x18))) + (VEC_Mag(var_r6_2 + 0x64) / 4096));
    {
        VecFx32 sp4 = { 0 };
        VEC_MultAdd((s32)(4096.0f * (*((f32 *)(((u8 *)(var_r6_2 + (0x14 * (*((u8 *)(((u8 *)var_r6_2) + 0xB1)))))) + 0x14)))), var_r6_2 + 0x64, &sp4, var_r5 + 0x64);
        temp_r2 = var_r6_2 + 0x64;
        *((VecFx32 *)(((u8 *)var_r6_2) + 0x64)) = sp4;
    }
    *((s8 *)(((u8 *)var_r6_2) + 0xA4)) = 0x12;
    *((s8 *)(((u8 *)var_r5) + 0xA4)) = 6;
    return 1;
}
