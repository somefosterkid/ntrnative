#include "global.h"

#include "constants/heap.h"

#include "list_menu_items.h"
#include "msgdata.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_022469E4(BattleCastleOpponentApp *app, u8 numWindows, u32 param_3, u32 param_4, u8 param_5);

u8 ov83_02247768(u8, u8);

u8 ov83_0224777C(SaveData *, u8, u8);

extern const u8 ov83_02248010[];

void ov83_02246AA4(BattleCastleOpponentApp *app);

void ov83_02246AA4(BattleCastleOpponentApp *app) {
    s32 temp_r0;
    s32 temp_r7;
    u16 n;
    n = 0;
    app->listMenuItems = ListMenuItems_New(4U, HEAP_ID_107);
    temp_r7 = ov83_0224777C(app->saveData, app->challengeType, 2);
    temp_r0 = ov83_02247768(app->numSlots, app->selectedMonSlot);
    if (app->identityUnlocked[temp_r0] == 0) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgLoader, 8, 0);
        n++;
    }
    ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgLoader, 9, 1);
    n++;
    if (((app->statsUnlocked[temp_r0] == 0) || (app->movesUnlocked[temp_r0] == 0)) || (temp_r7 != 2)) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgLoader, 0xA, 2);
        n++;
    }
    ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgLoader, 0xB, -2);
    n++;
    ov83_022469E4(app, n, 0x11, ov83_02248010[n], 0xD);
}

void ov83_02246B6C(BattleCastleOpponentApp *app);

void ov83_02246B6C(BattleCastleOpponentApp *app) {
    s32 var_r6;
    s32 var_r7;
    u16 var_r4;
    var_r4 = 0;
    app->listMenuItems = ListMenuItems_New(4U, HEAP_ID_107);
    var_r6 = ov83_0224777C(app->saveData, app->challengeType, 2);
    var_r7 = ov83_02247768(app->numSlots, app->selectedMonSlot);
    if (app->statsUnlocked[var_r7] == 0) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgLoader, 0x21, 3);
        var_r4++;
    }
    if ((var_r6 != 1) && (app->movesUnlocked[var_r7] == 0)) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgLoader, 0x22, 4);
        var_r4++;
    }
    if (var_r6 != 2) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgLoader, 0x23, 5);
        var_r4++;
    }
    ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgLoader, 0x24, -2);
    var_r4++;
    ov83_022469E4(app, var_r4, 0x11, ov83_02248010[var_r4], 0xD);
}
