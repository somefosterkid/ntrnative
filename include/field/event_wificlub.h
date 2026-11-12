// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_wificlub.h, the lines in use
// so far. pokebw2's EventWifiClub_CreateFromArgs, in its overlay 8, is Black's func_ov006_02154c20.
#ifndef BWPORT_FIELD_EVENT_WIFICLUB_H
#define BWPORT_FIELD_EVENT_WIFICLUB_H

#include "types.h"
#include "struct_decls.h"

typedef struct {
    Field *field;
    BOOL useTransitions;
} EventWifiClubArgs;

GameEvent *func_ov006_02154c20(GameSystem *gsys, void *args);

// pokebw2's EventWifiClub_Create
GameEvent *func_ov006_02154bf8(GameSystem *gsys, Field *field, BOOL useTransitions);

#endif // BWPORT_FIELD_EVENT_WIFICLUB_H
