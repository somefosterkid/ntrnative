// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_result.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_RESULT_H
#define BWPORT_BATTLE_BTL_RESULT_H

#include "types.h"

BOOL IsBattleResultDefeat(u32 result, u32 battleType);
u32 GetWildBattleResultByCombined(u32 result);

#endif // BWPORT_BATTLE_BTL_RESULT_H
