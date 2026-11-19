#include "global.h"

#include "overlay_96_ov96hurdle_work.h"

s32 ov96_021EAB38(void *, s32, s32);

void ov96_02206D58(Ov96HurdleRunner *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4);

void ov96_02206D58(Ov96HurdleRunner *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4) {
    u8 temp_r0;
    temp_r0 = arg0->unk_9F;
    switch (temp_r0) {
    case 0:
        arg0->unk_A0[0] = 0U;
        ov96_021EAB38(arg0->unk_00[arg2], 1, arg2);
        arg0->unk_9F = (u8)((arg0->unk_9F) + 1);
        return;

    case 1:
        arg0->unk_70 = (s16)((arg0->unk_70) - 8);
        arg0->unk_A0[0] = (u8)((arg0->unk_A0[0]) + 1);
        if (((u32)(arg0->unk_A0[0])) >= 0x20U) {
            arg0->unk_70 = 0;
            arg0->unk_9F = (u8)((arg0->unk_9F) + 1);
            return;
        }

    default:
        return;

    case 3:

        break;
    case 2:
        if (arg4 != 0) {
            *((s8 *)(((u8 *)arg3) + 8)) = 2;
        }
        arg0->unk_9F = (u8)((arg0->unk_9F) + 1);
    }
}
