#include "global.h"

#include "overlay_96_ov96_rower_work.h"

#include "sprite.h"
#include "unk_02005D10.h"

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB588(void *arg0, s32 *arg1);

extern s32 ov96_0221C5CC;

extern s32 ov96_0221C5D8;

extern s32 ov96_0221C5F0;

void ov96_021FFC34(Ov96RowerScore *arg0, s32 arg1);

void ov96_021FFC34(Ov96RowerScore *arg0, s32 arg1) {
    s32 sp18[3];
    s32 spC[3];
    s32 sp0[3];
    s32 var_r0;
    u8 temp_r5;
    u8 temp_r6;

    *(VecFx32 *)((u8 *)sp18) = *(VecFx32 *)((u8 *)&ov96_0221C5CC);
    *(VecFx32 *)((u8 *)spC) = *(VecFx32 *)((u8 *)&ov96_0221C5D8);
    *(VecFx32 *)((u8 *)sp0) = *(VecFx32 *)((u8 *)&ov96_0221C5F0);
    if ((s32) arg0->unk0E < arg1) {
        arg0->unk0E = (u16)arg1;
        var_r0 = 1;
    } else {
        var_r0 = 0;
    }
    if (var_r0 != 0) {
        temp_r6 = (u8)(arg1 / 10);
        temp_r5 = (u8)(arg1 % 10);
        if (temp_r6 != 0) {
            sp18[0] = 0x60000;
            spC[0] = 0x90000;
            sp0[0] = 0xB0000;
            Sprite_SetAnimCtrlSeq(arg0->digit0, temp_r6 + 1);
            Sprite_SetAnimCtrlSeq(arg0->digit1, temp_r5 + 1);
        } else {
            sp18[0] = 0x70000;
            spC[0] = 0xA0000;
            Sprite_SetAnimCtrlSeq(arg0->digit0, temp_r5 + 1);
            Sprite_SetAnimCtrlSeq(arg0->digit1, 0);
        }
        ov96_021EB588(arg0->unk08, sp18);
        Sprite_SetMatrix(arg0->digit0, (VecFx32 *)spC);
        Sprite_SetMatrix(arg0->digit1, (VecFx32 *)sp0);
        ov96_021EB52C(arg0->unk08, 1, 1);
        Sprite_SetDrawFlag(arg0->digit0, 1);
        Sprite_SetDrawFlag(arg0->digit1, 1);
        arg0->unk0C = 0x3CU;
        PlaySE(0x88FU);
        return;
    }
    arg0->unk0C = (u16)(arg0->unk0C - 1);
    if (arg0->unk0C == 0) {
        arg0->unk0C = 0U;
        ov96_021EB52C(arg0->unk08, 1, 0);
        Sprite_SetDrawFlag(arg0->digit0, 0);
        Sprite_SetDrawFlag(arg0->digit1, 0);
    }
}
