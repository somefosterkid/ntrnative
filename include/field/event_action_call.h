// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_action_call.h, the lines in use
// so far.
#ifndef BWPORT_FIELD_EVENT_ACTION_CALL_H
#define BWPORT_FIELD_EVENT_ACTION_CALL_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "system/game_event.h"

// The work of CallEventPrepareResidentActorsForZoneChange's event, whose callback is func_ov010_02161724
struct PrepareResidentActorsWork {
    GameSystem *gameSystem;
    Field *field;
    GameData *gameData;
    // Which actors the event let move: 1 the player, 2 the actor with move code 0x30 and 4 a third, always NULL
    u8 movingActors;
};

// In event_save.c (pokebw2's func_ov012_0215c59c)
GameEventReturnCode func_ov010_02161724(GameEvent *event, u32 *state, void *data);

struct EventActionCallWork {
    GameSystem *gameSystem;
    Field *field;
    GameData *gameData;
    u16 actorId;
    u16 padE;
    const u32 *action;
    FieldAcmdTCB *tasks[8];
};

GameEvent *CallEventPrepareResidentActorsForZoneChange(GameSystem *gsys, Field *field);
GameEventReturnCode EventActionCall_Callback(GameEvent *event, u32 *state, void *data);
GameEvent *EventActionCall_Create(GameSystem *gsys, Field *field, u16 actorId, const u32 *action);
GameEvent *CallMoveOneTileFrontEvent(GameSystem *gsys, Field *field);
FieldActor *EventActionCall_FindActor(MMSys *mmSys, u16 actorId);
void EventActionCall_ClearTCBs(EventActionCallWork *work);
void EventActionCall_AddTCB(EventActionCallWork *work, FieldAcmdTCB *task);
BOOL EventActionCall_UpdateTCBs(EventActionCallWork *work);
// Walks the player one tile the way they face
FieldAcmdTCB *func_ov010_02161b1c(Field *field);
// Removes the task once it ended, and returns whether it had
BOOL func_ov010_02161b74(FieldAcmdTCB *task);

#endif // BWPORT_FIELD_EVENT_ACTION_CALL_H
