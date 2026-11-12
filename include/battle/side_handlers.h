// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/side_handlers.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_SIDE_HANDLERS_H
#define BWPORT_BATTLE_SIDE_HANDLERS_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "battle/btl_pokeparam.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's IsSideEffectSet
// Whether a side effect is set
BOOL data_ov095_06898cf5(u8 side, u32 effect);
// pokebw2's SideEffectsInit
// Clears the side effects of both sides
void data_ov095_06898bfd(void);
// pokebw2's SideEffectsEndTurn
// Counts a turn of every timed side effect, and removes the ones that run out, calling the function with each
void data_ov095_06898d55(void (*callback)(u8 side, u32 effect, BtlServerFlow *flow), BtlServerFlow *flow);
// pokebw2's GetSideEffectLayers
// How many layers of a side effect are set
u32 data_ov095_06898ce1(u8 side, u32 effect);
// pokebw2's SideEffectEventAdd
// Sets a side effect, or adds a layer to it (Spikes and Toxic Spikes), and returns its event item, or NULL when
// it is set and can't take another layer (name guessed from the table of EventAddSide* functions it searches)
BattleEventItem *data_ov095_06898c11(u8 side, u32 effect, BattleCondition cont);
// Removes a side effect, and returns whether it was set
BOOL ServerDisplay_RemoveSideEffect(u8 side, u32 effect);

#endif // BWPORT_BATTLE_SIDE_HANDLERS_H
