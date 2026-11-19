#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

void ov96_0220D6B0(UnkStruct_ov96_0220E3F0_Unit *param_1, s16 *param_2);

void ov96_0220E818(UnkStruct_ov96_0220E3F0 *param_1, s16 *param_2, s32 param_3, s32 param_4);

u8 ov96_0220E850(UnkStruct_ov96_0220E3F0 *arg0, s32 arg1);

void ov96_0220E818(UnkStruct_ov96_0220E3F0 *param_1, s16 *param_2, s32 param_3, s32 param_4) {
    ov96_0220D6B0(&param_1->entries[param_3].units[param_4], param_2);
    return;
}

void ov96_0220D630(UnkStruct_ov96_0220E3F0_Unit *param_1);

void ov96_0220E834(UnkStruct_ov96_0220E3F0 *param_1, s32 param_2, s32 param_3);

void ov96_0220E834(UnkStruct_ov96_0220E3F0 *param_1, s32 param_2, s32 param_3) {
    ov96_0220D630(&param_1->entries[param_2].units[param_3]);
    return;
}

u8 ov96_0220E850(UnkStruct_ov96_0220E3F0 *arg0, s32 arg1) {
    return (u8)((u32)(arg0->entries[arg1].unk_E0 << 0xE) >> 0x10);
}
