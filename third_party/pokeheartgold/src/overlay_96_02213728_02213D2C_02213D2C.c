#include "global.h"

#include "error_handling.h"
#include "unk_02020E80.h"

s32 ov96_02213D2C(VecFx32 *arg0, VecFx32 *arg1, void *arg2, void *arg3, VecFx32 *arg4);

s32 ov96_02213D2C(VecFx32 *arg0, VecFx32 *arg1, void *arg2, void *arg3, VecFx32 *arg4) {
    VecFx32 sp2C = { 0, 0, 0 };
    VecFx32 sp20;
    s32 sp10[4];
    s32 sp0[4];
    int new_var;
    s32 temp_r1;
    s32 temp_r1_10;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r1_4;
    s32 temp_r1_5;
    s32 temp_r1_6;
    s32 temp_r1_7;
    s32 temp_r1_8;
    s32 temp_r1_9;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r3;
    s32 var_r3_2;
    temp_r1 = arg0->x;
    sp10[2] = ((s32)(temp_r1 + (((u32)(temp_r1 >> 0xB)) >> 0x14))) >> 0xC;
    temp_r1_2 = arg0->y;
    sp10[3] = ((s32)(temp_r1_2 + (((u32)(temp_r1_2 >> 0xB)) >> 0x14))) >> 0xC;
    new_var = 0;
    temp_r1_3 = arg1->x;
    sp10[0] = ((s32)(temp_r1_3 + (((u32)(temp_r1_3 >> 0xB)) >> 0x14))) >> 0xC;
    temp_r1_4 = arg1->y;
    sp10[1] = ((s32)(temp_r1_4 + (((u32)(temp_r1_4 >> 0xB)) >> 0x14))) >> 0xC;
    temp_r1_5 = *((s32 *)arg2);
    sp0[2] = ((s32)(temp_r1_5 + (((u32)(temp_r1_5 >> 0xB)) >> 0x14))) >> 0xC;
    temp_r1_6 = *((s32 *)(((u8 *)arg2) + 4));
    sp0[3] = ((s32)(temp_r1_6 + (((u32)(temp_r1_6 >> 0xB)) >> 0x14))) >> 0xC;
    temp_r1_7 = *((s32 *)arg3);
    sp0[0] = ((s32)(temp_r1_7 + (((u32)(temp_r1_7 >> 0xB)) >> 0x14))) >> 0xC;
    temp_r1_8 = *((s32 *)(((u8 *)arg3) + 4));
    sp0[1] = ((s32)(temp_r1_8 + (((u32)(temp_r1_8 >> 0xB)) >> 0x14))) >> 0xC;
    if (sub_02020EB0((Vec2D *)(&sp10[2]), (Vec2D *)(&sp10[new_var]), (Vec2D *)(&sp0[2]), (Vec2D *)(&sp0[new_var])) == new_var) {
        return new_var;
    }
    VEC_Subtract(arg1, arg0, &sp20);
    VEC_Normalize(&sp20, &sp20);
    temp_r1_9 = arg0->y;
    if (temp_r1_9 == arg1->y) {
        var_r0 = temp_r1_9 - (*((s32 *)(((u8 *)arg2) + 4)));
        if (var_r0 < new_var) {
            var_r0 *= -1;
        }
        var_r3 = temp_r1_9 - (*((s32 *)(((u8 *)arg3) + 4)));
        if (var_r3 < 0) {
            var_r3 *= -1;
        }
        if (var_r3 == new_var) {
            return 0;
        }
        sp2C.x = *((s32 *)arg2);
        sp2C.y = temp_r1_9;
        VEC_MultAdd(FX_Div(var_r0, var_r0 + var_r3), &sp20, &sp2C, arg4);
        return 1;
    }
    temp_r1_10 = arg0->x;
    if (temp_r1_10 == arg1->x) {
        var_r0_2 = temp_r1_10 - (*((s32 *)arg2));
        if (var_r0_2 < new_var) {
            var_r0_2 *= -1;
        }
        var_r3_2 = temp_r1_10 - (*((s32 *)arg3));
        if (var_r3_2 < new_var) {
            var_r3_2 *= -1;
        }
        if (var_r3_2 == 0) {
            return 0;
        }
        sp2C.x = temp_r1_10;
        sp2C.y = *((s32 *)(((u8 *)arg2) + 4));
        VEC_MultAdd(FX_Div(var_r0_2, var_r0_2 + var_r3_2), &sp20, &sp2C, arg4);
        return 1;
    }
    GF_AssertFail();
    return new_var;
}
