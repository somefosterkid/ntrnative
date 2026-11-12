// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_move.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_MOVE_H
#define BWPORT_BATTLE_BTL_MOVE_H

#include "types.h"
#include "struct_decls.h"

BOOL MoveEvent_AddItem(BattleMon *mon, u16 move, u32 speed);
void func_ov093_021e0394(BattleMon *mon, u16 move);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void RemoveForce(BattleMon *mon, u16 move);

#endif // BWPORT_BATTLE_BTL_MOVE_H
