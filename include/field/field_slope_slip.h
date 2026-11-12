// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_slope_slip.h.
#ifndef BWPORT_FIELD_FIELD_SLOPE_SLIP_H
#define BWPORT_FIELD_FIELD_SLOPE_SLIP_H

#include "types.h"
#include "struct_decls.h"

// field_slope_slip.c, overlay 21 (overlay 36 in Black 2): the event that slides the player down a slope tile, which
// fieldmap_event.c starts after a step onto one. Function names from swan (https://github.com/ds-pokemon-hacking/swan,
// GPL-3.0)
GameEvent *EventSlopeSlip_Create(GameEvent *parent, GameSystem *gsys, Field *field);
BOOL IsTileSlopeSlip(u32 tileClass);

#endif // BWPORT_FIELD_FIELD_SLOPE_SLIP_H
