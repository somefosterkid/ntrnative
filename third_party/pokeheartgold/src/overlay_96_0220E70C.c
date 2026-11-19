#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "error_handling.h"

s32 ov96_0220E70C(UnkStruct_ov96_0220E3F0 *arg0, u8 arg1, u32 arg2);

UnkStruct_ov96_0220E3F0_Record *ov96_0220E74C(UnkStruct_ov96_0220E3F0 *arg0, u32 arg1);

u32 ov96_021EAF8C(void *param_1);
void ov96_0220E770(UnkStruct_ov96_0220E3F0 *param_1, u32 param_2, u32 param_3, u16 *param_4, u32 *param_5);

s32 ov96_0220E70C(UnkStruct_ov96_0220E3F0 *arg0, u8 arg1, u32 arg2) {
    s32 v0;
    s32 v1;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg1 >= 4U) {
        GF_AssertFail();
    }
    if (arg2 >= 3U) {
        GF_AssertFail();
    }
    v1 = 1;
    v0 = arg0->entries[arg1].units[arg2].state;
    if ((v0 != 2) && (v0 != 1)) {
        v1 = 0;
    }
    return v1;
}

UnkStruct_ov96_0220E3F0_Record *ov96_0220E74C(UnkStruct_ov96_0220E3F0 *arg0, u32 arg1) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg1 >= 2U) {
        GF_AssertFail();
    }
    return &arg0->score.records[arg1];
}

void ov96_0220E770(UnkStruct_ov96_0220E3F0 *param_1, u32 param_2, u32 param_3, u16 *param_4, u32 *param_5) {
    u32 uVar1;
    UnkStruct_ov96_0220E3F0_Unit *iVar2;
    if (param_1 == NULL) {
        GF_AssertFail();
    }
    if ((3 + 1) <= param_2) {
        GF_AssertFail();
    }
    if ((2 + 1) <= param_3) {
        GF_AssertFail();
    }
    iVar2 = &param_1->entries[param_2].units[param_3];
    *param_4 = (s16)(iVar2->unk_1C.x >> 0xc);
    param_4[1] = (s16)(iVar2->unk_1C.y >> 0xc);
    uVar1 = ov96_021EAF8C(iVar2->unk_4);
    *param_5 = uVar1;
    return;
}
