#include "global.h"

#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02242508(s32, s32);

s32 ov70_022434C0(void *, s32, s32);

void ov70_02243F00(UnkStruct_0201956C *, MessagePrinter *, Window *, s32, u32);

void ov70_022427C4(UnkStruct_ov70_02242E58 *arg0, s32 arg1);

void ov70_022427C4(UnkStruct_ov70_02242E58 *arg0, s32 arg1) {
    s16 v0;
    s16 v1;

    if (arg1 < 0) {
        PlaySE(0x5E0U);
        Sprite_SetAnimActiveFlag(arg0->unk_14, 1);
        Sprite_SetAnimCtrlSeq(arg0->unk_14, 0x27);
        v0 = arg0->unk_5A;
        if (v0 != 0) {
            arg0->unk_5A = (s16)(v0 - 1);
        } else {
            arg0->unk_5A = 2;
        }
    } else {
        PlaySE(0x5E0U);
        Sprite_SetAnimActiveFlag(arg0->unk_10, 1);
        Sprite_SetAnimCtrlSeq(arg0->unk_10, 0x26);
        v1 = arg0->unk_5A;
        if ((s32)v1 < 2) {
            arg0->unk_5A = (s16)(v1 + 1);
        } else {
            arg0->unk_5A = 0;
        }
    }
    ov70_022434C0(arg0, arg0->unk_5A, arg0->unk_5C);
    ov70_02243F00(arg0->unk_1C, arg0->unk_38, &arg0->unk_4[4], arg0->unk_5A, ov70_02242508(arg0->unk_5C, 4));
}

s32 ov70_02242860(UnkStruct_ov70_02242E58 *arg0, s32 arg1);

s32 ov70_02242860(UnkStruct_ov70_02242E58 *arg0, s32 arg1) {
    switch (arg1) {
    case 4:
        PlaySE(0x5DCU);
        return -2;
    case 5:
        ov70_022427C4(arg0, -1);
        return -1;
    case 6:
        ov70_022427C4(arg0, 1);
        return -1;
    default:
        if ((s32)(arg1 + (arg0->unk_5A * 4)) < (s32) arg0->unk_5C) {
            PlaySE(0x5DCU);
            return arg0->unk_34[arg1].value;
        }
        return -1;
    }
}
