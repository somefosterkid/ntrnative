#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

void ov96_021E8228(PokeathlonCourseData *, u32, u32, s32, s32);

s32 ov96_0220FCB0(s16 *, s32);

s32 ov96_0220F3BC(UnkStruct_ov96_0220E3F0_Record *);

void ov96_0220FD28(UnkStruct_ov96_0220E3F0_Record *arg0, UnkStruct_ov96_0220E3F0_Entry *arg1, UnkStruct_ov96_0220E3F0_Hit *arg2);

void ov96_0220FD28(UnkStruct_ov96_0220E3F0_Record *arg0, UnkStruct_ov96_0220E3F0_Entry *arg1, UnkStruct_ov96_0220E3F0_Hit *arg2) {
    s16 sp4[2];
    UnkStruct_ov96_0220E3F0_Entry *var_r6;
    s32 temp_r0;
    u32 temp_r1;
    u32 *new_var;
    PokeathlonCourseData *temp_r7;

    sp4[0] = arg2->x / 8;
    sp4[1] = arg2->y / 8;
    if (sp4[1] < 9) {
        arg0->unk_4 = (arg0->unk_4 & ~0x1C) | 0xC;
        if (ov96_0220FCB0(sp4, 9) != 0) {
            arg0->unk_4 = (arg0->unk_4 & ~0x1C) | 0x10;
        }
    } else if (sp4[1] < 0xF) {
        arg0->unk_4 = (arg0->unk_4 & ~0x1C) | 0x10;
        if (ov96_0220FCB0(sp4, 0xF) != 0) {
            arg0->unk_4 = (arg0->unk_4 & ~0x1C) | 0x14;
        }
    } else if (sp4[1] < 0x14) {
        arg0->unk_4 = (arg0->unk_4 & ~0x1C) | 0x14;
        if (ov96_0220FCB0(sp4, 0x14) != 0) {
            arg0->unk_4 = (arg0->unk_4 & ~0x1C) | 0x18;
        }
    } else {
        arg0->unk_4 = (arg0->unk_4 & ~0x1C) | 0x18;
    }
    var_r6 = &arg1[arg2->entry];
    temp_r7 = var_r6->units[0].course;
    temp_r0 = ov96_0220F3BC(arg0);
    temp_r1 = ((var_r6->unk_E0 << 14) >> 16) + temp_r0;
    if ((s32)temp_r1 > 200) {
        temp_r1 = 200;
    }
    new_var = &var_r6->unk_E0;
    *new_var = (*new_var & 0xFFFC0003) | ((u32)(u16)temp_r1 << 2);
    arg0->unk_4 = (arg0->unk_4 & ~3) | (arg2->entry & 3);
    ov96_021E8228(temp_r7, arg2->entry, arg2->unit, 3, 1);
}
