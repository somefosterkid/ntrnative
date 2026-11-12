// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_save.c, Black 2/White 2's overlay
// 12. Black's save event takes no subscreen to go back to, Black has no twin of pokebw2's func_ov012_0215c574, and its
// func_ov010_02161724 doesn't let the actor with move code 0x30 move. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
#include "types.h"
#include "field/event_action_call.h"
#include "field/event_save.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

// The event that saves the game from the field menu

GameEvent *EventSave_Create(GameSystem *gsys, Field *field, u16 heapId, void *msgBgSys, u32 *result) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventSave_Callback, sizeof(EventSaveWork));
    EventSaveWork *work = GameEvent_GetData(event);
    GameData *gameData;

    MI_CpuFill8(work, 0, sizeof(EventSaveWork));
    work->msgBgSys = msgBgSys;
    work->heapId = heapId;
    work->gameSystem = gsys;
    work->field = field;
    gameData = GSYS_GetGameData(gsys);
    work->save = GameData_GetSaveControl(gameData);
    work->result = result;
    return event;
}

GameEventReturnCode EventSave_Callback(GameEvent *event, u32 *state, void *data) {
    EventSaveWork *work = data;
    u32 result = EventSave_Update(work, state);

    switch (result) {
    case 0:
        *work->result = 1;
        return GAMEEVENT_DONE;
    case 1:
        *work->result = 0;
        return GAMEEVENT_DONE;
    default:
        return GAMEEVENT_CONTINUE;
    }
}

void func_ov010_0216171c(void *param) {
    GFL_HeapFree(param);
}

// Doesn't match: Black keeps state 0's test of the third actor (movs r6, #0; beq), which this C folds away
GameEventReturnCode func_ov010_02161724(GameEvent *event, u32 *state, void *data) {
    PrepareResidentActorsWork *work = data;
    Field *field = work->field;
    MMSys *actorSystem;
    FieldActor *player;
    FieldActor *actor;
    FieldActor *third;

    switch (*state) {
    case 0:
        actorSystem = Field_GetActorSystem(field);
        player = FindPlayerFieldActor(actorSystem);
        actor = FindActorByMoveCode(actorSystem, 0x30);
        work->movingActors = 0;
        DisableAllActorsMovement(actorSystem);
        if (!func_ov010_0216d454(player)) {
            work->movingActors |= 1;
            EnableActorMovement(player);
        }
        // Black has no third actor here: a lookup that yields NULL either way, which MWCC doesn't fold
        third = actor != NULL ? NULL : NULL;
        if (third != NULL) {
            if (IsActorFlag16(third) == TRUE) {
                work->movingActors |= 4;
                EnableActorMovement(third);
            }
        }
        (*state)++;
        break;
    case 1:
        actorSystem = Field_GetActorSystem(field);
        player = FindPlayerFieldActor(actorSystem);
        actor = FindActorByMoveCode(actorSystem, 0x30);
        if ((work->movingActors & 1) && func_ov010_0216d454(player) == TRUE) {
            DisableActorMovement(player);
            work->movingActors &= ~1;
        }
        if (work->movingActors & 4) {
            third = NULL;
            if (!IsActorFlag16(third)) {
                DisableActorMovement(third);
                work->movingActors &= ~4;
            }
        }
        if (work->movingActors & 2) {
            actor = FindActorByMoveCode(actorSystem, 0x30);
            if (!IsActorFlag16(actor)) {
                DisableActorMovement(actor);
                work->movingActors &= ~2;
            }
        }
        if (work->movingActors == 0) {
            (*state)++;
        }
        break;
    case 2:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}
