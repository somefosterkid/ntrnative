#include "global.h"

#include "bg_window.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E98AC(UnkStruct_ov14_boxapp *app);
s32 ov14_021E85BC(void *);
s32 ov14_021E9920(UnkStruct_ov14_boxapp *app);
s32 ov14_021E9970(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(void *, s32);
s32 ov14_021E98AC(UnkStruct_ov14_boxapp *arg0) {
    u16 v0;
    UnkStruct_ov14_boxsys *v1;

    v1 = arg0->sys;
    FillBgTilemapRect(v1->unk_14, 0U, 0U, 0U, (u8)(u8)(v1->unk_10 + 6), 0x20U, 1U, 0x10U);
    FillBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 0U, (u8)(u8)(0xC - v1->unk_10), 0x20U, 1U, 0x10U);
    ScheduleBgTilemapBufferTransfer(arg0->sys->unk_14, 0U);
    v0 = v1->unk_10;
    if (v0 == 3) {
        v1->unk_10 = 0U;
        return 0;
    }
    v1->unk_10 = (u16)(v0 + 1);
    return 1;
}
s32 ov14_021E9920(UnkStruct_ov14_boxapp *arg0) {
    u16 v0;
    UnkStruct_ov14_boxsys *v1;

    v1 = arg0->sys;
    v0 = v1->unk_10;
    switch (v0) {
    case 0:
        if (sub_02019978(v1->unk_2F0, 0xF) == 0) {
            ov14_021E85BC(((u8 *)arg0->sys->unk_2F0));
            v1->unk_10 = (u16)(v1->unk_10 + 1);
        }
    default:
        break;
    case 1:
        if (sub_02019978(v1->unk_2F0, 0xE) == 0) {
            v1->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
s32 ov14_021E9970(UnkStruct_ov14_boxapp *arg0) {
    if ((sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xF) == 0) && (sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xE) == 0)) {
        return 0;
    }
    return 1;
}
