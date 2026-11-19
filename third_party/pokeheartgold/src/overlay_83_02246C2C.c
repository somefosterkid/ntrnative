#include "global.h"
struct BattleCastleOpponentApp;

#include "list_menu_items.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "overlay_80_02237A70.h"

s32 ov83_022469E4(void *, u8, s32, s32, u8);

void ov83_02246C2C(struct BattleCastleOpponentApp *arg0);

s32 ov83_0224773C(void *, s32, s32);
SysTask *ov83_02247CE8(void *);
void ov83_02246C70(void *arg0);

void ov83_02246C2C(BattleCastleOpponentApp *arg0) {
    s32 value;

    *((ListMenuItem * *)&arg0->unk_5FC) = ListMenuItems_New(3U, HEAP_ID_107);
    value = 0;
    do {
        ListMenuItems_AppendFromMsgData(*((ListMenuItem * *)&arg0->unk_5FC), *((MsgData * *)&arg0->msgLoader), value + 0x16, value);
        value += 1;
    } while ((u32)value < 3U);
    ov83_022469E4(arg0, 3, 0x11, 8, 0xD);
}

void ov83_02246C70(void *arg0) {
    s32 v0;

    v0 = ov80_02237B24(((BattleCastleOpponentApp *)arg0)->challengeType, 1);
    ov83_0224773C(arg0 + 0x4E4, v0, 0);
    ov83_0224773C(arg0 + 0x4F4, v0, 0);
    ov83_02247CE8(*((void **)&((BattleCastleOpponentApp *)arg0)->unk_5F8));
    ListMenuItems_Delete(*((ListMenuItem **)&((BattleCastleOpponentApp *)arg0)->unk_5FC));
    *((u8 *)((u8 *)&((BattleCastleOpponentApp *)arg0)->levelAdjustment + 0x1)) = (u8)(*((u8 *)((u8 *)&((BattleCastleOpponentApp *)arg0)->levelAdjustment + 0x1)) & ~4);
}
