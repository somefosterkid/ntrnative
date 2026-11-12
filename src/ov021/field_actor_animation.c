// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov033/field_actor_animation.c, ported by tools/port_file.py.
#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_actor_animation.h"
#include "field/field_script.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "system/vm.h"
#include "constants/directions.h"
#include "field/field_async_proc.h"
#include "nitro/fx/fx_vec.h"

void FieldActorAnmProc_Update(FieldAsyncProc *asyncProc, Field *field, void *data);
void func_ov021_021d1984(FieldAsyncProc *asyncProc, Field *field, void *data);
void FieldActorAnmProc_CommitTransform(G3DCurve *curve, FieldActor *actor, VecFx32 *pos);
void ChangeActorDirection(FieldActor *actor, u16 dir);
BOOL FieldSubscreen_IsReady(FieldSubscreen *subscreen);
void FieldSubscreen_ReqChange(FieldSubscreen *subscreen, u32 mode);

struct EventActorAnmProcWaitWork {
    FieldActorAnmProc *proc;
};

// Whether the actor can move again when each animation ends
static const u32 FIELD_ACTOR_ANM_IS_ALLOW_MOVE_ON_END[4] = { 0, 0, 0, 1 };

static const FieldAsyncProcDef FIELD_ACTOR_ANM_ASYNC_PROC_TEMPLATE = {
    0, sizeof(FieldActorAnmProc), NULL, NULL, FieldActorAnmProc_Update, func_ov021_021d1984,
};

FieldActorAnmProc *FieldActorAnmProc_Create(Field *field, u16 actorId, const VecFx32 *pos, HeapID heapId) {
    MMSys *actors = Field_GetActorSystem(field);
    FieldAsyncProc *asyncProc =
        FieldAsyncProcManager_AddProc(OVERLAY_NONE, Field_GetAsyncProcMgr(field), &FIELD_ACTOR_ANM_ASYNC_PROC_TEMPLATE);
    FieldActorAnmProc *work = FieldAsyncProc_GetData(asyncProc);

    work->asyncProc = asyncProc;
    work->heapId = heapId;
    work->pos = *pos;
    work->actor = FindFieldActor(actors, actorId);
    return work;
}

void FieldActorAnmProc_Free(FieldActorAnmProc *proc) {
    FieldAsyncProc_End(proc->asyncProc);
}

void FieldActorAnmProc_Play(FieldActorAnmProc *proc, u16 animation) {
    proc->curve = GFL_G3DCurveCreateToLoadBuffer(proc->heapId, 0xcb, animation, 0xa, proc->unk4, sizeof(proc->unk4));
    proc->finished = TRUE;
    proc->animation = animation;
    FieldActorAnmProc_CommitTransform(proc->curve, proc->actor, &proc->pos);
    SetActorMovementFlag(proc->actor, 0x8000);
}

BOOL FieldActorAnmProc_IsPlaying(FieldActorAnmProc *proc) {
    return proc->finished == 0;
}

void FieldActorAnmProc_Update(FieldAsyncProc *asyncProc, Field *field, void *data) {
    FieldActorAnmProc *work = data;

    if (work->finished != 0 && work->finished == 1) {
        if (GFL_G3DCurveFrameStepLoop(work->curve, FX32_ONE) == 0) {
            FieldActorAnmProc_CommitTransform(work->curve, work->actor, &work->pos);
        } else {
            VecFx32 pos;

            GFL_G3DCurveFree(work->curve);
            work->curve = NULL;
            work->finished = 0;
            CopyActorWPos(work->actor, &pos);
            SetActorWPosAll(work->actor, &pos, GetActorFaceDir(work->actor));
            if (FIELD_ACTOR_ANM_IS_ALLOW_MOVE_ON_END[work->animation] != 0) {
                ClearActorMovementFlag(work->actor, 0x8000);
            }
        }
        FldAct_InvokeUpdateCallback(work->actor);
    }
}

void func_ov021_021d1984(FieldAsyncProc *asyncProc, Field *field, void *data) {
}

void FieldActorAnmProc_CommitTransform(G3DCurve *curve, FieldActor *actor, VecFx32 *pos) {
    VecFx32 translation;
    VecFx32 rotation;

    GFL_G3DCurveGetNowTranslation(curve, &translation);
    GFL_G3DCurveGetNowRotation(curve, &rotation);
    VEC_Add(&translation, pos, &translation);
    SetActorWPosValue(actor, &translation);

    if (rotation.y == 0) {
        ChangeActorDirection(actor, DIR_DOWN);
    } else if (rotation.y == 90 * FX32_ONE) {
        ChangeActorDirection(actor, DIR_RIGHT);
    } else if (rotation.y == 180 * FX32_ONE) {
        ChangeActorDirection(actor, DIR_UP);
    } else if (rotation.y == 270 * FX32_ONE) {
        ChangeActorDirection(actor, DIR_LEFT);
    }
}

BOOL s0157_ActorAnimationInit(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    HeapID heapId = Field_GetHeapID(fieldWork->field);
    u16 actorId = ScriptReadAny(vm, env);
    VecFx32 pos;

    FieldPlayer_GetWPos(Field_GetPlayer(fieldWork->field), &pos);
    ScriptWork_SetActorAnmProc(work, FieldActorAnmProc_Create(fieldWork->field, actorId, &pos, heapId));
    return FALSE;
}

BOOL s0158_ActorAnimationFree(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);

    FieldActorAnmProc_Free(ScriptWork_GetActorAnmProc(work));
    ScriptWork_SetActorAnmProc(work, NULL);
    return FALSE;
}

BOOL s0159_ActorAnimationPlay(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 animation = ScriptReadAny(vm, env);

    FieldActorAnmProc_Play(ScriptWork_GetActorAnmProc(work), animation);
    return FALSE;
}

GameEventReturnCode EventActorAnmProcWait_Callback(GameEvent *event, u32 *state, void *data) {
    EventActorAnmProcWaitWork *work = data;

    return FieldActorAnmProc_IsPlaying(work->proc) == TRUE ? GAMEEVENT_DONE : GAMEEVENT_CONTINUE;
}

BOOL s015A_ActorAnimationWait(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameEvent *event = GameEvent_Create(gsys, NULL, EventActorAnmProcWait_Callback, sizeof(EventActorAnmProcWaitWork));
    EventActorAnmProcWaitWork *eventWork = GameEvent_GetData(event);

    eventWork->proc = ScriptWork_GetActorAnmProc(work);
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

GameEventReturnCode EventFieldSubscreenChange_Callback(GameEvent *event, u32 *state, void *work) {
    EventFieldSubscreenChange *change = work;
    switch (*state) {
    case 0:
        if (FieldSubscreen_IsReady(change->subscreen)) {
            FieldSubscreen_ReqChange(change->subscreen, change->mode);
            (*state)++;
        }
        break;
    case 1:
        if (FieldSubscreen_IsReady(change->subscreen)) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

BOOL func_ov021_021d1b2c(VM *vm, FieldScriptEnv *env) {
    static const u32 SCRIPTED_SUBSCREEN_LUT[1] = { 7 };
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    u16 animation = ScriptReadAny(vm, env);
    FieldSubscreen *subscreen = Field_GetSubscreen(fieldWork->field);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameEvent *event = GameEvent_Create(gsys, NULL, EventFieldSubscreenChange_Callback, sizeof(EventFieldSubscreenChange));
    EventFieldSubscreenChange *data = GameEvent_GetData(event);

    data->subscreen = subscreen;
    data->mode = SCRIPTED_SUBSCREEN_LUT[animation];
    ScriptWork_CallEvent(work, event);
    FieldScriptSubEvent_Register(8);
    return TRUE;
}
