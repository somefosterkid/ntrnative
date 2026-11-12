// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_action_call.c, Black 2/White 2's
// overlay 12. Black's file has no EventActorLinearMove event, and ends with two helpers that overlay 21 calls in place
// of Black 2's FieldAcmdTCB_CreateWalkOneTile. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
#include "types.h"
#include "field/event_action_call.h"
#include "field/event_actor_move.h"
#include "field/field.h"
#include "field/field_acmd.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "system/game_event.h"
#include "system/game_system.h"

// The events that run action commands on the field's actors, and the jump of an actor

const u32 ACMD_QUEUE_WALK_N_8F[2] = { ACMD(0xc, 1), ACMD_END };
const u32 ACMD_QUEUE_WALK_S_8F[2] = { ACMD(0xd, 1), ACMD_END };
const u32 ACMD_QUEUE_WALK_W_8F[2] = { ACMD(0xe, 1), ACMD_END };
const u32 ACMD_QUEUE_WALK_E_8F[2] = { ACMD(0xf, 1), ACMD_END };

const fx32 JUMP_ANY_HEIGHT_TABLE[16] = {
    2 * FX32_ONE, 4 * FX32_ONE, 6 * FX32_ONE, 8 * FX32_ONE, 9 * FX32_ONE, 10 * FX32_ONE, 10 * FX32_ONE, 10 * FX32_ONE,
    9 * FX32_ONE, 8 * FX32_ONE, 6 * FX32_ONE, 5 * FX32_ONE, 3 * FX32_ONE, 2 * FX32_ONE, 0,            0,
};

GameEvent *CallEventPrepareResidentActorsForZoneChange(GameSystem *gsys, Field *field) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov010_02161724, sizeof(PrepareResidentActorsWork));
    PrepareResidentActorsWork *work = GameEvent_GetData(event);
    work->gameSystem = gsys;
    work->field = field;
    work->gameData = GSYS_GetGameData(gsys);
    return event;
}

GameEventReturnCode EventActionCall_Callback(GameEvent *event, u32 *state, void *data) {
    EventActionCallWork *work = data;
    MMSys *mmSys = Field_GetActorSystem(work->field);
    switch (*state) {
    case 0: {
        FieldActor *actor = EventActionCall_FindActor(mmSys, work->actorId);
        if (actor == NULL) {
            return GAMEEVENT_DONE;
        }
        FieldAcmdTCB *task = FieldAcmdTCB_Create(actor, work->action);
        EventActionCall_AddTCB(work, task);
        (*state)++;
        break;
    }
    case 1:
        if (!EventActionCall_UpdateTCBs(work)) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventActionCall_Create(GameSystem *gsys, Field *field, u16 actorId, const u32 *action) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventActionCall_Callback, sizeof(EventActionCallWork));
    EventActionCallWork *work = GameEvent_GetData(event);
    EventActionCall_ClearTCBs(work);
    work->gameSystem = gsys;
    work->field = field;
    work->gameData = GSYS_GetGameData(gsys);
    work->actorId = actorId;
    work->action = action;
    return event;
}

GameEvent *CallMoveOneTileFrontEvent(GameSystem *gsys, Field *field) {
    u32 direction = GetActorFaceDir(FieldPlayer_GetActor(Field_GetPlayer(field)));
    const u32 *queue;
    switch (direction) {
    case 0: queue = ACMD_QUEUE_WALK_N_8F; break;
    case 1: queue = ACMD_QUEUE_WALK_S_8F; break;
    case 2: queue = ACMD_QUEUE_WALK_W_8F; break;
    case 3: queue = ACMD_QUEUE_WALK_E_8F; break;
    }
    return EventActionCall_Create(gsys, field, 0xff, queue);
}

FieldActor *EventActionCall_FindActor(MMSys *mmSys, u16 actorId) {
    // BUG: left unset for 0xf1, which returns whatever the register holds: the actor system
#ifdef BUGFIX
    FieldActor *actor = NULL;
#else
    FieldActor *actor;
#endif

    if (actorId == 0xf2) {
        actor = FindActorByMoveCode(mmSys, 0x30);
    } else if (actorId != 0xf1) {
        actor = FindFieldActor(mmSys, actorId);
    }
    return actor;
}

void EventActionCall_ClearTCBs(EventActionCallWork *work) {
    int i;
    for (i = 0; i < 8; i++) {
        work->tasks[i] = NULL;
    }
}

void EventActionCall_AddTCB(EventActionCallWork *work, FieldAcmdTCB *task) {
    int i;
    for (i = 0; i < 8; i++) {
        if (work->tasks[i] == NULL) {
            work->tasks[i] = task;
            return;
        }
    }
}

BOOL EventActionCall_UpdateTCBs(EventActionCallWork *work) {
    int i;
    BOOL active = 0;
    for (i = 0; i < 8; i++) {
        if (work->tasks[i] != NULL) {
            if (FieldAcmdTCB_CheckEnded(work->tasks[i]) == TRUE) {
                FieldAcmdTCB_Remove(work->tasks[i]);
                work->tasks[i] = NULL;
            } else {
                active = 1;
            }
        }
    }
    return active;
}

GameEventReturnCode EventActorJump_Callback(GameEvent *event, u32 *state, void *data) {
    EventActorJumpWork *work = data;
    VecFx32 position;
    fx32 landingY;
    fx32 y;
    BOOL done = FALSE;

    switch (*state) {
    case 0:
        position = work->start;
        position.y += JUMP_ANY_HEIGHT_TABLE[work->frame];
        work->frame += 2;
        position.x += FX_Div(FX_Mul(work->displacement.x, work->frame * FX32_ONE), 15 * FX32_ONE);
        position.z += FX_Div(FX_Mul(work->displacement.z, work->frame * FX32_ONE), 15 * FX32_ONE);
        SetActorWPosValue(work->actor, &position);
        if (work->frame > NELEMS(JUMP_ANY_HEIGHT_TABLE) - 1) {
            (*state)++;
        }
        break;
    case 1:
        CopyActorWPos(work->actor, &position);
        landingY = work->start.y + work->displacement.y;
        y = position.y - 8 * FX32_ONE;
        if (y > landingY) {
            position.y = y;
            SetActorWPosValue(work->actor, &position);
        } else {
            position.y = landingY;
            done = TRUE;
            position.x = work->start.x + work->displacement.x;
            position.z = work->start.z + work->displacement.z;
            SetActorWPosAll(work->actor, &position, GetActorFaceDir(work->actor));
            GFL_SndSEPlay(0x67b);
            func_ov021_021ac528(work->actor, work->effects);
        }
        break;
    }
    return done;
}

GameEvent *EventActorJump_Create(GameSystem *gsys, FieldActor *actor, const VecFx32 *start, const VecFx32 *end) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventActorJump_Callback, sizeof(EventActorJumpWork));
    EventActorJumpWork *work = GameEvent_GetData(event);
    work->actor = actor;
    work->effects = Field_GetFieldEffects(GSYS_GetField(gsys));
    work->start = *start;
    VEC_Subtract(end, start, &work->displacement);
    GFL_SndSEPlay(0x55e);
    return event;
}

FieldAcmdTCB *func_ov010_02161b1c(Field *field) {
    FieldActor *actor = FieldPlayer_GetActor(Field_GetPlayer(field));
    const u32 *queue;
    switch (GetActorFaceDir(actor)) {
    case 0: queue = ACMD_QUEUE_WALK_N_8F; break;
    case 1: queue = ACMD_QUEUE_WALK_S_8F; break;
    case 2: queue = ACMD_QUEUE_WALK_W_8F; break;
    case 3: queue = ACMD_QUEUE_WALK_E_8F; break;
    }
    func_ov010_0216d4b4(actor);
    return FieldAcmdTCB_Create(actor, queue);
}

BOOL func_ov010_02161b74(FieldAcmdTCB *task) {
    if (FieldAcmdTCB_CheckEnded(task) == TRUE) {
        FieldAcmdTCB_Remove(task);
        return TRUE;
    }
    return FALSE;
}
