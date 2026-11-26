#include "global.h"

#include "bg_window.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "party.h"
#include "pokemon.h"
struct TradeRoom;

void *ov65_0221E928(struct TradeRoom *, s32, s32);

u8 ov65_0221FB90(void *, s32, s32, MsgData *, MessageFormat *);

extern s32 ov65_0221EA74;

s32 ov65_0221E9A8(void *arg0);

s32 ov65_0221E9A8(void *arg0) {
    BufferBoxMonNickname(*(MessageFormat **)((u8 *)arg0 + 0x188), 0U, Mon_GetBoxMon(Party_GetMonByIndex(*(Party **)((u8 *)arg0 + 0x2224), *(s32 *)((u8 *)arg0 + 0x94))));
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x180), 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
    ov65_0221FB90(arg0 + 0x5A4, 0x10, 1, *(MsgData **)((u8 *)arg0 + 0x190), *(MessageFormat **)((u8 *)arg0 + 0x188));
    *(ListMenuItem **)((u8 *)arg0 + 0x668) = ListMenuItems_New(3U, HEAP_ID_26);
    ListMenuItems_AppendFromMsgData(*(ListMenuItem **)((u8 *)arg0 + 0x668), *(MsgData **)((u8 *)arg0 + 0x190), 0x11, 0);
    ListMenuItems_AppendFromMsgData(*(ListMenuItem **)((u8 *)arg0 + 0x668), *(MsgData **)((u8 *)arg0 + 0x190), 0x12, 1);
    ListMenuItems_AppendFromMsgData(*(ListMenuItem **)((u8 *)arg0 + 0x668), *(MsgData **)((u8 *)arg0 + 0x190), 0x13, 2);
    *(void **)((u8 *)arg0 + 0x36C8) = ov65_0221E928(arg0, 3, 9);
    *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221EA74;
    return 0;
}
