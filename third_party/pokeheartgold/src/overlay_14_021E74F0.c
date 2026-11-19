#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void sub_02019A60(UnkStruct_0201956C *, u32, void *);
void ov14_021E8944(UnkStruct_0201956C *param_1);
void ov14_021E895C(UnkStruct_ov14_boxapp *param_1, u32 param_2);
void ov14_021F2A18(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);
void ov14_021F38B0(UnkStruct_ov14_boxsys *param_1, u16 param_2);
s32 ov14_021F5564(UnkStruct_ov14_boxapp *param_1, s32 param_2);

void ov14_021E74F0(UnkStruct_ov14_boxapp *param_1, UnkStruct_ov14_mon *param_2) {
    s32 iVar1;

    if (param_2->item != 0) {
        iVar1 = ov14_021F5564(param_1, (u32) param_2->item);
        sub_02019A60(param_1->sys->unk_2F0, 0x10, &param_1->sys->windows[iVar1]);
        sub_02019A60(param_1->sys->unk_2F0, 0x10, &param_1->sys->windows[iVar1 + 1]);
        ov14_021F38B0(param_1->sys, param_2->item);
    }
    ov14_021E8944(param_1->sys->unk_2F0);
    ov14_021F36DC(param_1, param_2, 2);
    ov14_021F2A18(param_1->sys, (param_1->sys->unk_88D0 ^ 1) + 2, 1);
    ov14_021F2A18(param_1->sys, param_1->sys->unk_88D0 + 2, 0);
    ov14_021F3D70(param_1->sys, param_2);
    ov14_021F5368(param_1, param_2);
    ov14_021E895C(param_1, (u32) param_2->unk_10);
    return;
}
