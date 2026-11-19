#include "global.h"

#include "constants/heap.h"

#include "list_menu_items.h"
#include "msgdata.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_022469E4(void *, u8, s32, u32, u8);

u8 ov83_02247768(u8, u8);

u8 ov83_0224777C(SaveData *, u8, u8);

extern const u8 ov83_02248010[];

void ov83_02246AA4(void *arg0);

void ov83_02246AA4(void *arg0) {
    s32 temp_r0;
    s32 temp_r7;
    u16 n;
    u8 *p = arg0;
    n = 0;
    *((ListMenuItem **)&((BattleCastleOpponentApp *)p)->unk_5FC) = ListMenuItems_New(4U, HEAP_ID_107);
    temp_r7 = ov83_0224777C(*((void **)&((BattleCastleOpponentApp *)p)->saveData), ((BattleCastleOpponentApp *)p)->challengeType, 2);
    temp_r0 = ov83_02247768(((BattleCastleOpponentApp *)p)->numSlots, ((BattleCastleOpponentApp *)p)->selectedMonSlot);
    if ((*((u8 *)((((BattleCastleOpponentApp *)p)->identityUnlocked) + temp_r0))) == 0) {
        ListMenuItems_AppendFromMsgData(*((ListMenuItem **)&((BattleCastleOpponentApp *)p)->unk_5FC), ((MsgData *)((BattleCastleOpponentApp *)p)->msgLoader), 8, 0);
        n++;
    }
    ListMenuItems_AppendFromMsgData(*((ListMenuItem **)&((BattleCastleOpponentApp *)p)->unk_5FC), ((MsgData *)((BattleCastleOpponentApp *)p)->msgLoader), 9, 1);
    n++;
    if ((((*((u8 *)((((BattleCastleOpponentApp *)p)->statsUnlocked) + temp_r0))) == 0) || ((*((u8 *)((((BattleCastleOpponentApp *)p)->movesUnlocked) + temp_r0))) == 0)) || (temp_r7 != 2)) {
        ListMenuItems_AppendFromMsgData(*((ListMenuItem **)&((BattleCastleOpponentApp *)p)->unk_5FC), ((MsgData *)((BattleCastleOpponentApp *)p)->msgLoader), 0xA, 2);
        n++;
    }
    ListMenuItems_AppendFromMsgData(*((ListMenuItem **)&((BattleCastleOpponentApp *)p)->unk_5FC), ((MsgData *)((BattleCastleOpponentApp *)p)->msgLoader), 0xB, -2);
    n++;
    ov83_022469E4(p, n, 0x11, ov83_02248010[n], 0xD);
}

void ov83_02246B6C(void *arg0);

void ov83_02246B6C(void *arg0) {
    s32 var_r6;
    s32 var_r7;
    u16 var_r4;
    u8 *p = arg0;
    var_r4 = 0;
    *((ListMenuItem **)&((BattleCastleOpponentApp *)p)->unk_5FC) = ListMenuItems_New(4U, HEAP_ID_107);
    var_r6 = ov83_0224777C(*((void **)&((BattleCastleOpponentApp *)p)->saveData), ((BattleCastleOpponentApp *)p)->challengeType, 2);
    var_r7 = ov83_02247768(((BattleCastleOpponentApp *)p)->numSlots, ((BattleCastleOpponentApp *)p)->selectedMonSlot);
    if ((*((u8 *)((((BattleCastleOpponentApp *)p)->statsUnlocked) + var_r7))) == 0) {
        ListMenuItems_AppendFromMsgData(*((ListMenuItem **)&((BattleCastleOpponentApp *)p)->unk_5FC), ((MsgData *)((BattleCastleOpponentApp *)p)->msgLoader), 0x21, 3);
        var_r4++;
    }
    if ((var_r6 != 1) && ((*((u8 *)((((BattleCastleOpponentApp *)p)->movesUnlocked) + var_r7))) == 0)) {
        ListMenuItems_AppendFromMsgData(*((ListMenuItem **)&((BattleCastleOpponentApp *)p)->unk_5FC), ((MsgData *)((BattleCastleOpponentApp *)p)->msgLoader), 0x22, 4);
        var_r4++;
    }
    if (var_r6 != 2) {
        ListMenuItems_AppendFromMsgData(*((ListMenuItem **)&((BattleCastleOpponentApp *)p)->unk_5FC), ((MsgData *)((BattleCastleOpponentApp *)p)->msgLoader), 0x23, 5);
        var_r4++;
    }
    ListMenuItems_AppendFromMsgData(*((ListMenuItem **)&((BattleCastleOpponentApp *)p)->unk_5FC), ((MsgData *)((BattleCastleOpponentApp *)p)->msgLoader), 0x24, -2);
    var_r4++;
    ov83_022469E4(p, var_r4, 0x11, ov83_02248010[var_r4], 0xD);
}
