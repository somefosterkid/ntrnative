// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/b_bag_item.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_B_BAG_ITEM_H
#define BWPORT_BATTLE_B_BAG_ITEM_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's BBagItem_CheckLastItem
BOOL func_ov197_0220b9a4(BBagWork *work);
// pokebw2's BBagItem_SetCursorToLastItem
void func_ov197_0220b9d8(BBagWork *work);
// pokebw2's BBagItem_MakeShooterList
void func_ov197_0220bb1c(BBagWork *work);
// pokebw2's BBagItem_MakeDemoList
void func_ov197_0220bb78(BBagWork *work);
// pokebw2's BBagItem_GetSlotItem
u16 func_ov197_0220bb98(BBagWork *work, int slot);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BBagItem_MakePocketLists
void func_ov197_0220ba2c(BBagWork *work);
// pokebw2's BBagItem_GetShooterCost
u16 func_ov197_0220bbd8(u16 item);

#endif // BWPORT_BATTLE_B_BAG_ITEM_H
