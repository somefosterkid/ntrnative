#include "global.h"
#include "bg_window.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "party.h"
#include "pokemon.h"
#include "touchscreen_list_menu.h"
struct TradeRoom;

typedef struct UnkStruct_ov65_0221F3B0 {
    u8 unk_0[0x4];
    void *unk_4;
    void *unk_8;
    u8 unk_C[0x88];
    s32 unk_94;
    s32 unk_98;
    u8 unk_9C[0x2184];
    s32 *unk_2220;
    void *unk_2224;
    void *unk_2228;
} UnkStruct_ov65_0221F3B0;

s32 ov65_0221F5A0(void *, void *, s32, s32, void *);

s32 sub_02034AEC(SaveData *, s32);

extern s32 ov65_0221E270;

s32 ov65_0221F3B0(UnkStruct_ov65_0221F3B0 *arg0);

TouchscreenListMenu *ov65_0221E928(struct TradeRoom *, s32, s32);
u8 ov65_0221FB90(void *, s32, s32, MsgData *, MessageFormat *);
extern s32 ov65_0221F4D4;
s32 ov65_0221F3F4(void *arg0);
s32 ov65_0221F8B4(void);
void ov65_0221F4B0(void *arg0);

s32 ov65_0221F3B0(UnkStruct_ov65_0221F3B0 *arg0) {
    sub_02034AEC(arg0->unk_4, 1);
    ov65_0221F5A0(arg0->unk_2224, arg0->unk_2228, arg0->unk_94, arg0->unk_98 - 6, arg0->unk_8);
    arg0->unk_2220 = &ov65_0221E270;
    return 2;
}

s32 ov65_0221F3F4(void *arg0) {
    BufferBoxMonNickname(*(MessageFormat **)((u8 *)arg0 + 0x188), 0U, Mon_GetBoxMon(Party_GetMonByIndex(*(Party **)((u8 *)arg0 + 0x2228), *(s32 *)((u8 *)arg0 + 0x94) - 6)));
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x180), 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
    ov65_0221FB90(arg0 + 0x5A4, 0x10, 1, *(MsgData **)((u8 *)arg0 + 0x190), *(MessageFormat **)((u8 *)arg0 + 0x188));
    *(ListMenuItem **)((u8 *)arg0 + 0x668) = ListMenuItems_New(2U, HEAP_ID_26);
    ListMenuItems_AppendFromMsgData(*(ListMenuItem **)((u8 *)arg0 + 0x668), *(MsgData **)((u8 *)arg0 + 0x190), 0x11, 0);
    ListMenuItems_AppendFromMsgData(*(ListMenuItem **)((u8 *)arg0 + 0x668), *(MsgData **)((u8 *)arg0 + 0x190), 0x13, 1);
    *(void **)((u8 *)arg0 + 0x36C8) = ov65_0221E928(arg0, 2, 0xC);
    *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221F4D4;
    return 0;
}

void ov65_0221F4B0(void *arg0) {
    TouchscreenListMenu_Destroy(*(TouchscreenListMenu **)((u8 *)arg0 + 0x36C8));
    TouchscreenListMenuSpawner_Destroy(*(TouchscreenListMenuSpawner **)((u8 *)arg0 + 0x36C4));
    ov65_0221F8B4();
}
