#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E952C(UnkStruct_ov14_boxapp *app);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021F2A18(struct UnkStruct_ov14_boxsys *, u32, u32);

void ov14_021F0C0C(UnkStruct_ov14_boxapp *param_1) {
    s32 iVar1;

    param_1->unk21 = 0xff;
    ov14_021F2A18(param_1->sys, 9, 0);
    ov14_021E8020(param_1->sys->unk_2F0);
    iVar1 = ov14_021E8544(param_1->sys->unk_2F0);
    if (iVar1 == 1) {
        ov14_021E84A4(param_1->sys->unk_2F0);
    }
    ov14_021F0234(param_1, ov14_021E952C, 0x2d);
    return;
}
