#include "global.h"

#include "item.h"
#include "msgdata.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021F0910(UnkStruct_ov14_boxapp *param_1);

u32 ov14_021E6070(UnkStruct_ov14_boxapp *param_1, u32 param_2, s32 param_3, void *param_4);
void ov14_021E83F4(UnkStruct_0201956C *param_1);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F0254(UnkStruct_ov14_boxapp *app, s16 param_2);
void ov14_021F027C(UnkStruct_ov14_boxapp *app, s8 param_2);
void ov14_021F2A18(struct UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);
void ov14_021F6704(void *param_1, u32 param_2, s32 param_3);
s32 ov14_021E9450(UnkStruct_ov14_boxapp *app);

void ov14_021F0910(UnkStruct_ov14_boxapp *param_1) {
    u32 uVar1;
    BOOL BVar2;

    uVar1 = ov14_021E6070(param_1, (u32) param_1->unk21, 0x4c, (void *)0x0);
    if (uVar1 != 0) {
        ov14_021F2A18(param_1->sys, 9, 0);
        ov14_021E84A4(param_1->sys->unk_2F0);
        ov14_021F0234(param_1, ov14_021E9450, 0x4f);
        return;
    }
    uVar1 = ov14_021E6070(param_1, (u32) param_1->unk21, 6, (void *)0x0);
    if (uVar1 == 0) {
        param_1->unk2C = 0x26;
        ov14_021F027C(param_1, 1);
        return;
    }
    ov14_021F2A18(param_1->sys, 9, 0);
    BVar2 = ItemIdIsMail((u16)uVar1);
    if (BVar2 == 1) {
        ov14_021E84A4(param_1->sys->unk_2F0);
        ov14_021F0234(param_1, ov14_021E9450, 0x50);
        return;
    }
    ov14_021E83F4(param_1->sys->unk_2F0);
    ov14_021F6704(param_1, uVar1, 0x25);
    ov14_021F0254(param_1, 0);
    return;
}
