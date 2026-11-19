#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "error_handling.h"

s16 ov96_0220E6DC(int, int);

void ov96_0220FBDC(UnkStruct_ov96_0220E3F0_Record *);

s32 ov96_0220FBEC(UnkStruct_ov96_0220E3F0_Record *, UnkStruct_ov96_0220E3F0_Entry *, UnkStruct_ov96_0220E3F0_Hit *);

void ov96_0220FD28(UnkStruct_ov96_0220E3F0_Record *, UnkStruct_ov96_0220E3F0_Entry *, UnkStruct_ov96_0220E3F0_Hit *);

void ov96_0220FE38(UnkStruct_ov96_0220E3F0_Record *arg0, u16 *arg1, UnkStruct_ov96_0220E3F0_Entry *arg2);

void ov96_0220FE38(UnkStruct_ov96_0220E3F0_Record *arg0, u16 *arg1, UnkStruct_ov96_0220E3F0_Entry *arg2) {
    UnkStruct_ov96_0220E3F0_Hit sp0;
    s16 temp_r2_2;
    s32 temp_r1_2;
    s32 temp_r2;
    u32 temp_r1;
    temp_r2 = arg0->unk_4;
    if ((((u32)(temp_r2 << 0x1A)) >> 0x1F) == 0) {
        return;
    }
    temp_r1 = ((u32)(temp_r2 << 0x1B)) >> 0x1D;
    switch (temp_r1) {
    case 1:
        if (ov96_0220FBEC(arg0, arg2, &sp0) != 0) {
            ov96_0220FD28(arg0, arg2, &sp0);
        } else {
            temp_r2_2 = arg0->unk_2;
            if (((s32)temp_r2_2) >= 0x1E0) {
                arg0->unk_2 = 0;
                arg0->unk_4 = (s32)(8 | (arg0->unk_4 & (~0x1C)));
            } else {
                arg0->unk_2 = (s16)(temp_r2_2 + arg0->unk_A);
                arg0->unk_0 = ov96_0220E6DC(arg0->unk_2, arg0->unk_9);
            }
        }
        break;

    case 2:
        arg0->unk_2 = (s16)(arg0->unk_2 + arg0->unk_A);
        if (((s32)arg0->unk_2) >= 0x30) {
            ov96_0220FBDC(arg0);
        }
        break;

    case 3:
    case 4:
    case 5:

    case 6:
        temp_r1_2 = (0xFFFFC03F & temp_r2) | (((u32)(((((u32)(temp_r2 << 0x12)) >> 0x18) + 1) << 0x18)) >> 0x12);
        arg0->unk_4 = temp_r1_2;
        if (((u32)(((u32)(temp_r1_2 << 0x12)) >> 0x18)) >= 0x14U) {
            ov96_0220FBDC(arg0);
        }
        break;

    default:
        GF_AssertFail();
    }

    *arg1 = ((*arg1) & ~0x7F) | (0x7F & (u16)(arg0->unk_2 * 2 / 8));
    *arg1 = ((*arg1) & 0xFFFFF87F) | (((u32)(arg0->unk_9 << 0x1C)) >> 0x15);
    *arg1 = (0xFFFFC7FF & (*arg1)) | (((u32)(((u16)(((u32)(arg0->unk_4 << 0x1B)) >> 0x1D)) << 0x1D)) >> 0x12);
    *arg1 = (0xFFFF3FFF & (*arg1)) | ((u16)(((u16)(((u32)(arg0->unk_4 << 0x1E)) >> 0x1E)) << 0xE));
}
