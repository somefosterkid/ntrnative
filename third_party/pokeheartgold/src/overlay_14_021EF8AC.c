#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "item.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F1F38(void *);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F6688(void *, s32);
s32 ov14_021EF920(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021EF8AC(UnkStruct_ov14_boxapp *arg0) {
    u16 itemId;
    void *v4;
    u8 v3;
    UnkStruct_ov14_boxsys *v0;
    UnkStruct_ov14_boxsys *v1;
    u32 v2;

    v0 = arg0->sys;
    v4 = v0->unk_C;
    ov14_021F34C8(v0, *(u16 *)v4, 0);
    v3 = arg0->unk21;
    v2 = *(u16 *)((u8 *)v4 + 2);
    arg0->unk21 = (u8) * (u16 *)v4;
    ov14_021F1F38(arg0);
    v1 = arg0->sys;
    itemId = v1->unk_88C8;
    v1->unk_88C8 = 0U;
    if ((ItemIdIsMail(itemId) == 1) && (v2 != v3)) {
        PlaySE(0x5F3U);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 0);
        ov14_021F6730(arg0, 0x25);
        *((s32 *)&arg0->unk30) = 0x87;
        return 6;
    }
    GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
    return 0x82;
}

s32 ov14_021EF920(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F6688(arg0->sys, 0x25);
    GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
    return 0x82;
}
