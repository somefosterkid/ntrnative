#include "global.h"

s32 ov96_021E8BB0(s32 param_1);

u8 ov96_021E90FC(s32 param_1);

u32 ov96_021EAA20(u32 *param_1, s32, s32);

void ov96_021EABE0(s32 *param_1, u8 param_2);

void ov96_021EABF4(s32 *param_1, VecFx32 *param_2);

void ov96_0220D554(s32 *param_1, s32 param_2, s32 arg2);

void ov96_0220D554(s32 *param_1, s32 param_2, s32 arg2) {
    s32 iVar1;
    u32 uVar2;
    s8 uVar3;

    iVar1 = ov96_021EAA20(param_1, param_2, arg2);
    uVar2 = ov96_021E90FC(iVar1);
    VecFx32 VStack_20 = { 0, 0, 0 };
    iVar1 = ov96_021E8BB0(iVar1);
    if (*(u16 *)(iVar1 + 4) != 0) {
        uVar3 = 0x40;
        ov96_021EABE0(param_1, 1);
    } else {
        uVar3 = 0x20;
        ov96_021EABE0(param_1, 2);
    }
    if (param_2 != 0) {
        VStack_20.x = uVar3 / 2 << 0xc;
        iVar1 = uVar2 + (s32)(uVar3 - uVar2) / 2;
        VStack_20.y = iVar1 * 0x1000;
    } else {
        VStack_20.x = uVar3 / 2 << 0xc;
        VStack_20.y = (uVar3 - 1) << 0xc;
    }
    ov96_021EABF4(param_1, &VStack_20);
}
