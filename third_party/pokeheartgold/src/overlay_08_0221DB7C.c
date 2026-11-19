#include "global.h"

#include "battle/battle_system.h"

#include "bag.h"
#include "bag_cursor.h"
#include "bg_window.h"
#include "sprite_system.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct MessageFormat;

void ov08_0221DB7C(BattleParty *battleParty);

void ov08_0221DB7C(BattleParty *battleParty) {
    ManagedSprite_SetDrawFlag(battleParty->sprites[26], FALSE);
    ClearWindowTilemapAndScheduleTransfer(&battleParty->windows[10]);
    ClearWindowTilemapAndScheduleTransfer(&battleParty->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&battleParty->windows[7]);
}

s32 ov08_0221D81C(BattleParty *battleParty);

void ov08_0221DBB4(BattleParty *arg0);

void ov08_0221DBB4(BattleParty *arg0) {
    ov08_0221D81C(arg0);
    ScheduleBgTilemapBufferTransfer(arg0->background, 7U);
}

void ov08_0221DBCC(BattleSystem *arg0, u16 arg1, u16 arg2, enum HeapID arg3);

void ov08_0221DBCC(BattleSystem *battleSystem, u16 itemId, u16 pocket, enum HeapID heapID) {
    if ((itemId != 0x41) && (itemId != 0x43) && (itemId != 0x42)) {
        Bag_TakeItem(BattleSystem_GetBag(battleSystem), itemId, 1U, heapID);
    }
    BagCursor_Battle_SetLastUsedItem(BattleSystem_GetBagCursor(battleSystem), itemId, pocket);
}
