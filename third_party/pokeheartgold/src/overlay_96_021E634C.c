#include "global.h"

void ov96_021E62AC(void *param_1, s32 param_2, u32 param_3, u32 param_4, u32 param_5, u16 param_6, void *param_7);

u32 ov96_021EB5E8(s32 param_1);

void ov96_021E634C(void *param_1, s32 param_2, u32 param_3, s32 param_4, u32 param_5, u8 param_6, void *param_7);

void ov96_021E634C(void *param_1, s32 param_2, u32 param_3, s32 param_4, u32 param_5, u8 param_6, void *param_7) {
    u32 uVar1;

    uVar1 = ov96_021EB5E8(param_4);
    ov96_021E62AC(param_1, param_2, param_3, uVar1, param_5, (u32)param_6, param_7);
    return;
}
