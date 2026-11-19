#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F1F38(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021EF1EC(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021F40DC(void *);

s32 ov14_021EF190(UnkStruct_ov14_boxapp *arg0) {
    u16 *v1;
    UnkStruct_ov14_boxsys *v0;

    v0 = arg0->sys;
    v1 = v0->unk_C;
    ov14_021F34C8(v0, *v1, 0);
    PlaySE(0x5EAU);
    ov14_021F40DC(arg0);
    ov14_021F391C(arg0->sys, 1);
    ov14_021F29E4(arg0->sys, 0xB, 2);
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32) arg0->unk21);
    GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
    arg0->unk21 = (u8)*v1;
    ov14_021F1F38(arg0);
    return 0x81;
}

s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);

s32 ov14_021EF1EC(UnkStruct_ov14_boxapp *arg0) {
    if (ov14_021F2A04(arg0->sys, 0xB) == 1) {
        return 0x81;
    }
    ov14_021F391C(arg0->sys, 0);
    ov14_021F2A18(arg0->sys, 0xB, 0);
    ov14_021E8248(((u8 *)arg0->sys->unk_2F0));
    ov14_021E82A8(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8328(arg0->sys->unk_2F0);
    return ov14_021F0234(arg0, ov14_021E94BC, 0x75);
}
