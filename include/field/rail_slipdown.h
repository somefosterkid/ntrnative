// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/rail_slipdown.h. pokebw2's
// RailSlipdown_Create, RailSlipdown_Delete and RailSlipdown_IsDone are its own names.
#ifndef BWPORT_FIELD_RAIL_SLIPDOWN_H
#define BWPORT_FIELD_RAIL_SLIPDOWN_H

#include "types.h"
#include "struct_decls.h"

// Sliding down a slope off a rail onto the rail below, overlay 63 (rail_slipdown.c, named from the ROM's string). The
// event that loads the overlay starts the slide, polls it until it is done, then deletes it.

// Starts the actor's slide in a task. stopPlayer is TRUE when the actor is the player, who is braked first and whom the
// camera follows down
void *func_ov063_021f36c0(GameSystem *gsys, Field *field, FieldActor *actor, BOOL stopPlayer);
void func_ov063_021f3758(void *work);
BOOL func_ov063_021f376c(void *work);

#endif // BWPORT_FIELD_RAIL_SLIPDOWN_H
