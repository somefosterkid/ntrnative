// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_dive.h, without the rail
// slipdown events at the end of Black 2's file, which Black's does not have.
#ifndef BWPORT_FIELD_EVENT_DIVE_H
#define BWPORT_FIELD_EVENT_DIVE_H

#include "types.h"
#include "struct_decls.h"
#include "system/game_event.h"

struct DiveEventData {
    GameSystem *gsys;
    Field *field;
    u32 param;
    s32 timer;
};

GameEvent *EventDiveIn_Create(GameSystem *gsys, Field *field);
GameEvent *CreateDiveOutEvent(GameSystem *gsys, Field *field, u32 param);
GameEventReturnCode EventDiveIn_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventDiveOut_Callback(GameEvent *event, u32 *state, void *data);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEvent *func_ov021_021c474c(GameSystem *gsys, Field *field, FieldActor *actor);

#endif // BWPORT_FIELD_EVENT_DIVE_H
