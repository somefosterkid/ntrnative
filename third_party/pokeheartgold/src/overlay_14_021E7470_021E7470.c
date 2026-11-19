#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void sub_02019A60(UnkStruct_0201956C *, u32, void *);
void ov14_021E895C(UnkStruct_ov14_boxapp *param_1, u32 param_2);
void ov14_021F2A18(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);

void ov14_021E7470(UnkStruct_ov14_boxapp *param_1, UnkStruct_ov14_mon *param_2) {
    u32 iVar1;
    u32 uVar2;

    iVar1 = ov14_021F5404(param_1, param_2);
    uVar2 = 0;
    do {
        sub_02019A60(param_1->sys->unk_2F0, 0x10, &param_1->sys->windows[iVar1 + uVar2]);
        uVar2 = uVar2 + 1;
    } while (uVar2 < 4);
    ov14_021E88A4(param_1->sys->unk_2F0);
    ov14_021F36DC(param_1, param_2, 2);
    ov14_021F2A18(param_1->sys, (param_1->sys->unk_88D0 ^ 1) + 2, 1);
    ov14_021F2A18(param_1->sys, param_1->sys->unk_88D0 + 2, 0);
    ov14_021F3D70(param_1->sys, param_2);
    ov14_021F5368(param_1, param_2);
    ov14_021E895C(param_1, (u32) param_2->unk_10);
    return;
}
