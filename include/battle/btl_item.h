// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_item.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_ITEM_H
#define BWPORT_BATTLE_BTL_ITEM_H

#include "types.h"
#include "struct_decls.h"

void ItemEvent_ItemRotationWake(BattleMon *mon);
BattleEventItem *ItemEvent_AddItem(BattleMon *mon);
void ItemEvent_RemoveItem(BattleMon *mon);
void ItemEvent_ItemRotationSleep(BattleMon *mon);

BattleEventItem *ItemEvent_TempAdd(BattleMon *mon, u16 item);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov093_021dcf9c(void *temp);

#endif // BWPORT_BATTLE_BTL_ITEM_H
