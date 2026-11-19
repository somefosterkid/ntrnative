#include "global.h"

#include "math_util.h"

void _s32_div_f(u16, u32);

void ov96_021E860C(u32 param_1, s8 param_2, s32 param_3, s32 param_4);

void ov96_021E860C(u32 param_1, s8 param_2, s32 param_3, s32 param_4) {
    u8 cVar1;
    u16 uVar2;
    u32 uVar3;
    u32 remainder;
    u32 remainder_00;
    u32 uVar4;
    u8 acStack_38[5];
    u8 acStack_33[27];
    uVar3 = 0;
    do {
        *((u8 *)(param_4 + uVar3)) = 0;
        uVar3 = (uVar3 + 1) & 0xff;
    } while (uVar3 < 3);
    uVar4 = 0;
    if (param_2 == 10) {
        uVar3 = 0x19;
        do {
            acStack_33[uVar4] = (s8)uVar4;
            uVar4 = (uVar4 + 1) & 0xff;
        } while (uVar4 < 0x19);
        uVar4 = 0;
        if (uVar4 < param_1) {
            do {
                uVar2 = LCRandom();
                remainder = ((s32)uVar2) % ((s32)uVar3);
                cVar1 = acStack_33[remainder & 0xff];
                *((s8 *)(param_4 + uVar4)) = cVar1;
                *((s8 *)(param_4 + uVar4)) = cVar1 + 0x19;
                uVar3 = (uVar3 - 1) & 0xff;
                acStack_33[remainder & 0xff] = acStack_33[uVar3];
                uVar4 = (uVar4 + 1) & 0xff;
            } while (uVar4 < param_1);
        }
    } else {
        uVar3 = 5;
        do {
            acStack_38[uVar4] = (s8)uVar4;
            uVar4 = (uVar4 + 1) & 0xff;
        } while (uVar4 < 5);
        uVar4 = 0;
        if (uVar4 < param_1) {
            do {
                uVar2 = LCRandom();
                remainder_00 = ((s32)uVar2) % ((s32)uVar3);
                cVar1 = acStack_38[remainder_00 & 0xff];
                *((s8 *)(param_4 + uVar4)) = cVar1;
                *((s8 *)(param_4 + uVar4)) = cVar1 + (((s8)param_2) * 5);
                uVar3 = (uVar3 - 1) & 0xff;
                acStack_38[remainder_00 & 0xff] = acStack_38[uVar3];
                uVar4 = (uVar4 + 1) & 0xff;
            } while (uVar4 < param_1);
        }
    }
    if ((param_3 != 0) && (uVar3 = 0, uVar3 < param_1)) {
        do {
            *((u8 *)(param_4 + uVar3)) = (*((u8 *)(param_4 + uVar3))) + 0x32;
            uVar3 = (uVar3 + 1) & 0xff;
        } while (uVar3 < param_1);
    }
    uVar3 = 0;
    if (uVar3 < param_1) {
        do {
            *((u8 *)(param_4 + uVar3)) = (*((u8 *)(param_4 + uVar3))) + 1;
            uVar3 = (uVar3 + 1) & 0xff;
        } while (uVar3 < param_1);
    }
    return;
}
