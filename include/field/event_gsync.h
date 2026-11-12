// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_gsync.h. pokebw2's
// EventGameSync_Create and EventGameSync_CreateFromArgs are its own names.
#ifndef BWPORT_FIELD_EVENT_GSYNC_H
#define BWPORT_FIELD_EVENT_GSYNC_H

// Game Sync, started by the C-Gear

#include "types.h"
#include "struct_decls.h"

GameEvent *func_ov000_02154670(GameSystem *gsys);
GameEvent *func_ov000_02154668(GameSystem *gsys, void *args);

#endif // BWPORT_FIELD_EVENT_GSYNC_H
