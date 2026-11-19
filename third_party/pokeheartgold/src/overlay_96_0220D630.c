#include "global.h"
#include "error_handling.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

u16 ov96_0220F3B4(UnkStruct_ov96_0220E3F0_Cell *arg0);

s32 ov96_021E8228(PokeathlonCourseData *, u32, u32, s32, s32);

UnkStruct_ov96_0220E3F0_Cell *ov96_0220F378(PokeathlonCourseData *, u32, u32);

void ov96_0220D630(UnkStruct_ov96_0220E3F0_Unit *arg0);

u32 ov96_0220D694(UnkStruct_ov96_0220E3F0_Unit *param_1);
void ov96_0220D6B0(UnkStruct_ov96_0220E3F0_Unit *param_1, s16 *param_2);

void ov96_0220D630(UnkStruct_ov96_0220E3F0_Unit *arg0) {
    if (ov96_0220F378(arg0->course, arg0->unk_45, arg0->unk_46)->unk_2 != 0) {
        ov96_021E8228(arg0->course, arg0->unk_45, arg0->unk_46, 2, 1);
        arg0->unk_40.raw = (s32)((u8)ov96_0220F3B4(arg0->cell) | (arg0->unk_40.raw & ~0xFF));
        arg0->state = 1;
    }
    arg0->unk_40.raw = (s32)(0xA00 | (arg0->unk_40.raw & 0xFFFF00FF));
}

u32 ov96_0220D694(UnkStruct_ov96_0220E3F0_Unit *param_1) {
    u32 uVar1;

    if (param_1 == NULL) {
        GF_AssertFail();
    }
    uVar1 = 1;
    if ((param_1->state != 2) && (param_1->state != 1)) {
        uVar1 = 0;
    }
    return uVar1;
}

void ov96_0220D6B0(UnkStruct_ov96_0220E3F0_Unit *param_1, s16 *param_2) {
    param_1->unk_10.x = (s32)*param_2 << 0xc;
    param_1->unk_10.y = (s32)param_2[1] << 0xc;
    param_1->unk_40.raw = param_1->unk_40.raw | 0x8000000;
    return;
}
