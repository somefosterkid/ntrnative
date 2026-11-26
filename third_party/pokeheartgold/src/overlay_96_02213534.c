#include "global.h"
#include "pokeathlon/pokeathlon.h"

s32 ov96_02213514(VecFx32 *arg0, VecFx32 *arg1);

s32 ov96_02213534(void *arg0, void *arg1, u8 arg2);

void ov96_02213558(void *arg0);
u32 _s32_div_f(s32, u32);
void ov96_0221359C(PokeathlonCourseData *param_1, u8 *param_2, s32 param_3, s32 param_4, u8 *param_5);

s32 ov96_02213534(void *arg0, void *arg1, u8 arg2) {
    if (ov96_02213514(arg0, arg1) >= (s32)FX_SinCosTable_[((arg2 >> 4) * 2) + 1]) {
        return 1;
    }
    return 0;
}

void ov96_02213558(void *arg0) {
    u8 v0;

    *(u8 *)((u8 *)arg0 + 0x61) = (u8)(*(u8 *)((u8 *)arg0 + 0x61) + 1);
    if ((u32) * (u8 *)((u8 *)arg0 + 0x61) >= 0xAU) {
        *(u8 *)((u8 *)arg0 + 0x61) = 0U;
        *(u8 *)((u8 *)arg0 + 0x71) = (u8)(*(u8 *)((u8 *)arg0 + 0x71) + 2);
        v0 = *(u8 *)((u8 *)arg0 + 0x70);
        if ((u32) * (u8 *)((u8 *)arg0 + 0x71) > (u32)v0) {
            *(u8 *)((u8 *)arg0 + 0x71) = v0;
        }
    }
}

void ov96_0221359C(PokeathlonCourseData *param_1, u8 *param_2, s32 param_3, s32 param_4, u8 *param_5) {
    PokeathlonMonKinds *pbVar1;
    u32 uVar2;

    pbVar1 = ov96_021E60D8(param_1, param_3, param_4);
    *(s16 *)(param_5 + 0x6c) = (s16) * (u32 *)(param_2 + (u32)pbVar1->statIdx[4] * 4);
    *(s16 *)(param_5 + 0x6e) = (s16) * (u32 *)(param_2 + (u32)pbVar1->statIdx[4] * 4 + 0x14);
    *(s8 *)(param_5 + 0x70) = (s8) * (u32 *)(param_2 + (u32)pbVar1->statIdx[3] * 4 + 0x28);
    *(s8 *)(param_5 + 0x71) = (s8) * (u32 *)(param_2 + (u32)pbVar1->statIdx[3] * 4 + 0x28);
    uVar2 = _s32_div_f(*(s32 *)(param_2 + (u32)pbVar1->statIdx[1] * 4 + 0x3c) << 0xc, 10);
    *(u32 *)(param_5 + 100) = uVar2;
    uVar2 = _s32_div_f(*(s32 *)(param_2 + (u32)pbVar1->statIdx[1] * 4 + 0x50) << 0xc, 10);
    *(u32 *)(param_5 + 0x68) = uVar2;
    *(s8 *)(param_5 + 0x72) = (s8) * (u32 *)(param_2 + (u32)pbVar1->statIdx[0] * 4 + 100);
    *(u8 *)(param_5 + 0x73) = pbVar1->statIdx[4];
    *(u8 *)(param_5 + 0x75) = pbVar1->statIdx[0];
    *(u8 *)(param_5 + 0x74) = pbVar1->statIdx[3];
    return;
}
