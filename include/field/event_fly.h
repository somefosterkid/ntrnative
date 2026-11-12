// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_fly.h.
#ifndef BWPORT_FIELD_EVENT_FLY_H
#define BWPORT_FIELD_EVENT_FLY_H

#include "types.h"
#include "struct_decls.h"
#include "system/game_event.h"

GameEvent *func_ov021_021c8268(GameSystem *gsys, Field *field, u32 zoneId);
GameEventReturnCode EventFly_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021c85cc(GameEvent *event, u32 *state, void *data);

#endif // BWPORT_FIELD_EVENT_FLY_H
