// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_cgear_shutdown.h.
#ifndef BWPORT_FIELD_EVENT_CGEAR_SHUTDOWN_H
#define BWPORT_FIELD_EVENT_CGEAR_SHUTDOWN_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "system/game_event.h"

struct CGearShutdownData {
    Field *field;
    FieldSubscreen *subscreen;
};

GameEvent *EventCGearShutdown_Create(GameSystem *gsys);

#endif // BWPORT_FIELD_EVENT_CGEAR_SHUTDOWN_H
