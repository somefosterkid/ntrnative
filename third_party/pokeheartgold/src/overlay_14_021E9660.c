#include "global.h"
#include "bg_window.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E9618(void *arg0);
s32 ov14_021E9660(UnkStruct_ov14_boxapp *app);
s32 ov14_021E96A8(UnkStruct_ov14_boxapp *app);
s32 ov14_021E96C8(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(void *, s32);
s32 ov14_021E9660(UnkStruct_ov14_boxapp *arg0) {
    u16 v0;
    UnkStruct_ov14_boxsys *v1;

    v1 = arg0->sys;
    v0 = v1->unk_10;
    switch (v0) {
    case 0:
        if (ov14_021E9618(arg0) == 0) {
            v1->unk_10 = (u16)(v1->unk_10 + 1);
        }
    default:
        break;
    case 1:
        ov14_021F44B4(v1, 8);
        if (sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xD) == 0) {
            v1->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
s32 ov14_021E96A8(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F44B4(arg0->sys, -8);
    return sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xD);
}
s32 ov14_021E96C8(UnkStruct_ov14_boxapp *arg0) {
    u16 *buffer;
    u16 v0;
    u16 v1;
    UnkStruct_ov14_boxsys *v2;

    v2 = arg0->sys;
    buffer = sub_02019B08(v2->unk_2F0, 0);
    v0 = v2->unk_10;
    switch ((u32)v0) {
    case 0:
        CopyToBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 9U, 0x20U, 1U, buffer, 0U, 3U, 0x20U, 7U);
        break;
    case 1:
        CopyToBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 8U, 0x20U, 1U, buffer, 0U, 0U, 0x20U, 7U);
        CopyToBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 0xAU, 0x20U, 1U, buffer, 0U, 6U, 0x20U, 7U);
        break;
    case 2:
        CopyToBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 7U, 0x20U, 2U, buffer, 0U, 0U, 0x20U, 7U);
        CopyToBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 0xAU, 0x20U, 2U, buffer, 0U, 5U, 0x20U, 7U);
        break;
    case 10:
        FillBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 0U, 4U, 0x20U, 0xBU, 0x10U);
    case 3:
        CopyToBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 6U, 0x20U, 7U, buffer, 0U, 0U, 0x20U, 7U);
        break;
    case 4:
        CopyToBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 5U, 0x20U, 2U, buffer, 0U, 0U, 0x20U, 7U);
        CopyToBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 0xCU, 0x20U, 2U, buffer, 0U, 5U, 0x20U, 7U);
        break;
    case 5:
        CopyToBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 4U, 0x20U, 2U, buffer, 0U, 0U, 0x20U, 7U);
        CopyToBgTilemapRect(arg0->sys->unk_14, 0U, 0U, 0xDU, 0x20U, 2U, buffer, 0U, 5U, 0x20U, 7U);
        break;
    }
    ScheduleBgTilemapBufferTransfer(arg0->sys->unk_14, 0U);
    v1 = v2->unk_10;
    if (v1 == 0xA) {
        v2->unk_10 = 0U;
        return 0;
    }
    v2->unk_10 = (u16)(v1 + 1);
    return 1;
}
