#include "global.h"

#include "bg_window.h"
#include "list_menu_items.h"
#include "touchscreen_list_menu.h"

typedef struct UnkStruct_ov65_0221EA74 {
    u8 unk_0[0x180];
    BgConfig *unk_180;
    void *unk_184;
    u8 unk_188[0x8];
    void *unk_190;
    u8 unk_194[0x400];
    u8 unk_594;
    u8 unk_595[0x1F];
    u8 unk_5B4;
    u8 unk_5B5[0xB3];
    ListMenuItem *unk_668;
    u8 unk_66C[0x1BB4];
    s32 *unk_2220;
    u8 unk_2224[0x14A4];
    TouchscreenListMenu *unk_36C8;
} UnkStruct_ov65_0221EA74;

s32 ov65_0221F4B0(void *);

u8 ov65_0221FB90(void *, s32, s32, MsgData *, MessageFormat *);

extern s32 ov65_0221E050;

extern s32 ov65_0221EB9C;

s32 ov65_0221EA74(UnkStruct_ov65_0221EA74 *arg0);

s32 ov65_0221EA74(UnkStruct_ov65_0221EA74 *arg0) {
    u32 v0;

    v0 = (u32)TouchscreenListMenu_HandleInput(arg0->unk_36C8);
    switch (v0) {
    case 0:
        FillBgTilemapRect(arg0->unk_180, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
        ov65_0221FB90((void *)&arg0->unk_594, 0xF, 1, arg0->unk_190, arg0->unk_184);
        ov65_0221F4B0(arg0);
        ListMenuItems_Delete(arg0->unk_668);
        arg0->unk_2220 = &ov65_0221E050;
        return 3;
    case 1:
        FillBgTilemapRect(arg0->unk_180, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
        ov65_0221FB90((void *)&arg0->unk_5B4, 0x14, 1, arg0->unk_190, arg0->unk_184);
        ov65_0221F4B0(arg0);
        ListMenuItems_Delete(arg0->unk_668);
        arg0->unk_2220 = &ov65_0221EB9C;
    default:
        break;
    case -2:
    case 2:
        FillBgTilemapRect(arg0->unk_180, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
        ov65_0221FB90((void *)&arg0->unk_594, 0xF, 1, arg0->unk_190, arg0->unk_184);
        ov65_0221F4B0(arg0);
        ListMenuItems_Delete(arg0->unk_668);
        arg0->unk_2220 = &ov65_0221E050;
        break;
    }
    return 0;
}
