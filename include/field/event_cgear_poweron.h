// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_cgear_poweron.h: the prototypes
// of Black's named functions and the types they need.
#ifndef BWPORT_FIELD_EVENT_CGEAR_POWERON_H
#define BWPORT_FIELD_EVENT_CGEAR_POWERON_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

GameEvent *EventCGearPowerOn_Create(GameSystem *gsys, BOOL bootComm);

typedef struct {
    // Set once the subscreen has changed to the C-Gear
    BOOL subscreenChanged;
    // Whether to start the game's communication
    BOOL bootComm;
    GameSystem *gsys;
} CGearPowerOnData;

#endif // BWPORT_FIELD_EVENT_CGEAR_POWERON_H
