// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/scrcmd_actor_move.c, which is the same
// code in Black 2/White 2's overlay 12, except for the second command's event: Black 2's holds the actor in place for
// a few frames, Black's circles it around its place. The name is pokebw2's, a descriptive one.
#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_script.h"
#include "field/scrcmd_actor_move.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "system/game_event.h"
#include "system/game_system.h"

// The actor's movement flag that keeps its movement codes from moving it
#define ACTOR_MOVEMENT_FLAG_HELD 0x8000

// The radius of the actor's circle, and the frames of its three parts: two turns spiralling out to the radius, one
// turn at the radius, and one spiralling back in
#define CIRCLE_RADIUS (48 * FX32_ONE)
#define CIRCLE_OUT_FRAMES 110
#define CIRCLE_FRAMES 80
#define CIRCLE_IN_FRAMES 80

typedef struct {
    fx32 distance;
    u16 frames;
    u16 frame;
    FieldActor *actor;
    VecFx32 pos;
} ActorMoveHeightData;

typedef struct {
    u32 frame;
    FieldActor *actor;
    VecFx32 pos;
} ActorCircleData;

static GameEventReturnCode func_ov010_0216a838(GameEvent *event, u32 *state, void *work);
static GameEventReturnCode func_ov010_0216a8f0(GameEvent *event, u32 *state, void *work);

BOOL func_ov010_0216a7a0(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    GameSystem *gsys;
    u16 actorId;
    u16 height;
    u16 frames;
    GameEvent *event;
    ActorMoveHeightData *data;
    FieldActor *actor;

    FieldScriptEnv_GetGameData(env);
    work = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    actorId = ScriptReadAny(vm, env);
    height = ScriptReadAny(vm, env);
    frames = ScriptReadAny(vm, env);
    event = GameEvent_Create(gsys, NULL, func_ov010_0216a838, sizeof(ActorMoveHeightData));
    data = GameEvent_GetData(event);
    data->frames = frames;
    data->frame = 0;
    actor = FindFieldActor(Field_GetActorSystem(GSYS_GetField(gsys)), actorId);
    CopyActorWPos(actor, &data->pos);
    data->actor = actor;
    SetActorMovementFlag(actor, ACTOR_MOVEMENT_FLAG_HELD);
    // The height is in half tiles
    data->distance = height * (8 * FX32_ONE) - data->pos.y;
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

static GameEventReturnCode func_ov010_0216a838(GameEvent *event, u32 *state, void *work) {
    ActorMoveHeightData *data = work;
    VecFx32 pos;

    data->frame++;
    pos = data->pos;
    pos.y += data->distance * data->frame / data->frames;
    SetActorWPosValue(data->actor, &pos);
    if (data->frame >= data->frames) {
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

BOOL func_ov010_0216a884(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    GameSystem *gsys;
    u16 actorId;
    GameEvent *event;
    ActorCircleData *data;
    FieldActor *actor;

    FieldScriptEnv_GetGameData(env);
    work = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    actorId = ScriptReadAny(vm, env);
    event = GameEvent_Create(gsys, NULL, func_ov010_0216a8f0, sizeof(ActorCircleData));
    data = GameEvent_GetData(event);
    data->frame = 0;
    actor = FindFieldActor(Field_GetActorSystem(GSYS_GetField(gsys)), actorId);
    CopyActorWPos(actor, &data->pos);
    data->actor = actor;
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

static GameEventReturnCode func_ov010_0216a8f0(GameEvent *event, u32 *state, void *work) {
    ActorCircleData *data = work;
    VecFx32 pos = data->pos;
    int angle;
    fx16 sin;
    fx16 cos;
    fx32 radius;

    data->frame++;
    switch (*state) {
    case 0:
        angle = data->frame * 0x20000 / CIRCLE_OUT_FRAMES;
        if (angle >= 0x10000) {
            angle %= 0x10000;
        }
        sin = FX_SinIdx(angle);
        cos = FX_CosIdx(angle);
        radius = data->frame * CIRCLE_RADIUS / CIRCLE_OUT_FRAMES;
        pos.x += FX_Mul(cos, radius);
        pos.z += FX_Mul(sin, radius);
        SetActorWPosValue(data->actor, &pos);
        if (data->frame >= CIRCLE_OUT_FRAMES) {
            data->frame = 0;
            *state = 1;
        }
        break;
    case 1:
        angle = data->frame * 0x10000 / CIRCLE_FRAMES;
        if (angle >= 0x10000) {
            angle %= 0x10000;
        }
        sin = FX_SinIdx(angle);
        cos = FX_CosIdx(angle);
        pos.x += FX_Mul(cos, CIRCLE_RADIUS);
        pos.z += FX_Mul(sin, CIRCLE_RADIUS);
        SetActorWPosValue(data->actor, &pos);
        if (data->frame >= CIRCLE_FRAMES) {
            data->frame = 0;
            *state = 2;
        }
        break;
    case 2:
        angle = data->frame * 0x10000 / CIRCLE_IN_FRAMES;
        if (angle >= 0x10000) {
            angle %= 0x10000;
        }
        sin = FX_SinIdx(angle);
        cos = FX_CosIdx(angle);
        radius = (CIRCLE_IN_FRAMES - data->frame) * CIRCLE_RADIUS / CIRCLE_IN_FRAMES;
        pos.x += FX_Mul(cos, radius);
        pos.z += FX_Mul(sin, radius);
        SetActorWPosValue(data->actor, &pos);
        if (data->frame >= CIRCLE_IN_FRAMES) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}
