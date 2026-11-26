#include "global.h"

#include "battle/battle_system.h"

#include "bag.h"
#include "bag_cursor.h"
#include "bg_window.h"
#include "overlay_08_battle_bag.h"

void ov08_02223390(BattleSystem *arg0, u16 arg1, u16 arg2, enum HeapID arg3);

void ov08_02223390(BattleSystem *battleSystem, u16 itemId, u16 pocket, enum HeapID heapID) {
    Bag_TakeItem(BattleSystem_GetBag(battleSystem), itemId, 1U, heapID);
    BagCursor_Battle_SetLastUsedItem(BattleSystem_GetBagCursor(battleSystem), itemId, pocket);
}

void ov08_022233DC(BattleBag *, int);

extern WindowTemplate ov08_02225B90;

void ov08_022233B8(BattleBag *battleBag);

void ov08_022233B8(BattleBag *battleBag) {
    AddWindow(battleBag->background, &battleBag->messageBoxWindow, &ov08_02225B90);
    ov08_022233DC(battleBag, battleBag->currentScreen);
}
