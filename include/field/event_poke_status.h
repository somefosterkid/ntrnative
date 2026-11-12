// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_poke_status.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_EVENT_POKE_STATUS_H
#define BWPORT_FIELD_EVENT_POKE_STATUS_H

#include "types.h"
#include "gfl/overlay.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define OVERLAY_EVENT_POKE_STATUS OVERLAY_ID(20)

// The party list events: picked gets TRUE and index the slot of the Pokémon picked, or FALSE and 0
GameEvent *func_ov010_0215a07c(GameSystem *gsys, u16 *picked, u16 *index);
// The party list for a move tutor: learnMask the party slots, as bits, of the Pokémon that can learn move
GameEvent *func_ov010_0215a0e8(GameSystem *gsys, u16 *picked, u16 *index, u8 learnMask, u16 move);
// The party list for the musical, with a summary screen
GameEvent *func_ov010_0215a168(GameSystem *gsys, u16 *picked, u16 *index);
// The summary screen, to pick a move of the Pokémon at partyIndex to forget for move: forgot gets TRUE and slot the
// move's slot, or FALSE and 0
GameEvent *func_ov010_0215a2e4(GameSystem *gsys, Field *field, u16 partyIndex, u16 *forgot, u16 *slot, s32 forgetHm, u16 move);

#endif // BWPORT_FIELD_EVENT_POKE_STATUS_H
