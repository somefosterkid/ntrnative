// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_wifi_bsubway.h. pokebw2's
// EventWifiBSubway_Create and EventWifiBSubway_CreateFromArgs are its own names.
#ifndef BWPORT_FIELD_EVENT_WIFI_BSUBWAY_H
#define BWPORT_FIELD_EVENT_WIFI_BSUBWAY_H

// The Wi-Fi Battle Subway, started by a script command

#include "types.h"
#include "struct_decls.h"

typedef struct {
    u32 mode;
    u16 *result;
} EventWifiBSubwayArgs;

GameEvent *func_ov007_021542cc(GameSystem *gsys, u32 mode, u16 *result);
GameEvent *func_ov007_021542c0(GameSystem *gsys, void *args);

#endif // BWPORT_FIELD_EVENT_WIFI_BSUBWAY_H
