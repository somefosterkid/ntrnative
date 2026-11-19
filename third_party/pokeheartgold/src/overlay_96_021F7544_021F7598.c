#include "global.h"

#include "overlay_96_unk_struct_ov96_021_f74a4.h"

void ov96_021F7620(UnkStruct_ov96_021F74A4 *param_1);

void ov96_021F7598(UnkStruct_ov96_021F74A4 *param_1);

void ov96_021F7598(UnkStruct_ov96_021F74A4 *param_1) {
    if (param_1->timer == 0) {
        ov96_021F7620(param_1);
        param_1->timer = 0xf0;
        return;
    }
    param_1->timer = param_1->timer + -1;
    return;
}

s32 ov96_021EB564(void *, s32);

void ov96_021F75BC(UnkStruct_ov96_021F74A4 *arg0, s32 arg1);

void ov96_021F75BC(UnkStruct_ov96_021F74A4 *arg0, s32 arg1) {
    arg0->entries[arg1].active = 0;
    ov96_021EB564(arg0->entries[arg1].unk_10, 8);
}

VecFx32 *ov96_021F75D4(UnkStruct_ov96_021F74A4 *param_1, s32 param_2);

VecFx32 *ov96_021F75D4(UnkStruct_ov96_021F74A4 *param_1, s32 param_2) {
    return &param_1->entries[param_2].pos;
}

u32 ov96_021F75E0(UnkStruct_ov96_021F74A4 *param_1, s32 param_2);

u32 ov96_021F75E0(UnkStruct_ov96_021F74A4 *param_1, s32 param_2) {
    return param_1->entries[param_2].active;
}

void ov96_021EB52C(void *param_1, s32 param_2, s32 param_3);

void ov96_021F75E8(UnkStruct_ov96_021F74A4 *param_1, s32 param_2, s32 param_3);

void ov96_021F75E8(UnkStruct_ov96_021F74A4 *param_1, s32 param_2, s32 param_3) {
    ov96_021EB564(param_1->entries[param_2].unk_18, 10);
    ov96_021EB52C(param_1->entries[param_2].unk_18, 1, 1);
    ov96_021EB564(param_1->entries[param_2].unk_14, param_3);
    ov96_021EB52C(param_1->entries[param_2].unk_14, 1, 1);
    return;
}
