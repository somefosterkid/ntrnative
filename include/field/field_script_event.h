// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_script_event.h: the prototypes
// of Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_SCRIPT_EVENT_H
#define BWPORT_FIELD_FIELD_SCRIPT_EVENT_H

#include "types.h"
#include "gfl/heap.h"
#include "system/game_event.h"
#include "struct_decls.h"

GameEvent *EventScriptCall_Create(GameSystem *gsys, u16 scriptId, FieldActor *actor, HeapID heapId);
GameEventReturnCode EventScriptCall_Callback(GameEvent *event, u32 *state, void *data);
GameEvent *EventScriptCall_CreateCore(GameSystem *gsys, HeapID heapId, u16 scriptId, FieldActor *actor, u32 param);

#endif // BWPORT_FIELD_FIELD_SCRIPT_EVENT_H
