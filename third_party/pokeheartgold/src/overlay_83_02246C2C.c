#include "global.h"
#include "list_menu_items.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "overlay_80_02237A70.h"

void ov83_022469E4(BattleCastleOpponentApp *app, u8 numWindows, u32 param_3, u32 param_4, u8 param_5);

void ov83_02246C2C(struct BattleCastleOpponentApp *arg0);

s32 ov83_0224773C(void *, s32, s32);
void ov83_02247CE8(struct TouchscreenListMenu *menu);
void ov83_02246C70(BattleCastleOpponentApp *arg0);

void ov83_02246C2C(BattleCastleOpponentApp *arg0) {
    s32 value;

    arg0->listMenuItems = ListMenuItems_New(3U, HEAP_ID_107);
    value = 0;
    do {
        ListMenuItems_AppendFromMsgData(arg0->listMenuItems, arg0->msgLoader, value + 0x16, value);
        value += 1;
    } while ((u32)value < 3U);
    ov83_022469E4(arg0, 3, 0x11, 8, 0xD);
}

void ov83_02246C70(BattleCastleOpponentApp *arg0) {
    s32 v0;

    v0 = ov80_02237B24(arg0->challengeType, 1);
    ov83_0224773C(arg0->monSprites, v0, 0);
    ov83_0224773C(arg0->ballSprites, v0, 0);
    ov83_02247CE8(arg0->touchMenu);
    ListMenuItems_Delete(arg0->listMenuItems);
    arg0->simpleMenuIsOpen = FALSE;
}
