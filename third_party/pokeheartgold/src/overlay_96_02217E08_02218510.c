#include "global.h"

void *ov96_021E8BB0(s32);

u8 ov96_021E90FC();

s32 ov96_021EAA20();

s32 ov96_021EABF4(s32 *, s32 *);

void ov96_02218510(void *arg0, s32 arg1);

void ov96_02218510(void *arg0, s32 arg1) {
    s32 sp0[3];
    s32 temp_r4;
    s32 temp_r7;
    u32 temp_r1;
    u8 *new_var;
    u32 var_r0;
    temp_r7 = ov96_021EAA20();
    temp_r4 = ov96_021E90FC();
    new_var = (u8 *)(&sp0[0]);
    *((s32 *)new_var) = 0;
    *((s32 *)(new_var + 4)) = 0;
    *((s32 *)(new_var + 8)) = 0;
    if ((*((u16 *)(((u8 *)ov96_021E8BB0(temp_r7)) + 4))) != 0) {
        var_r0 = 0x40;
    } else {
        var_r0 = 0x20;
    }
    if (arg1 != 0) {
        sp0[0] = (((s32)(var_r0 + (var_r0 >> 0x1F))) >> 1) << 0xC;
        temp_r1 = var_r0 - temp_r4;
        sp0[1] = (temp_r4 + (((s32)(temp_r1 + (temp_r1 >> 0x1F))) >> 1)) << 0xC;
    } else {
        sp0[0] = (((s32)(var_r0 + (var_r0 >> 0x1F))) >> 1) << 0xC;
        sp0[1] = var_r0 << 0xC;
    }
    ov96_021EABF4(arg0, &sp0[0]);
}
