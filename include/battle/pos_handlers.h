// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/pos_handlers.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_POS_HANDLERS_H
#define BWPORT_BATTLE_POS_HANDLERS_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's IsPosEffectFree
// Whether no event of the position effect is set yet (name guessed from PosEventAdd refusing to add a second one)
BOOL data_ov095_068982ad(u32 effect, u8 pos);
// The event item it adds for the effect, or NULL when the position has one of it already
BattleEventItem *PosEventAdd(u32 effect, u8 pos, u8 monId, const u32 *args, u8 argCount);

#endif // BWPORT_BATTLE_POS_HANDLERS_H
