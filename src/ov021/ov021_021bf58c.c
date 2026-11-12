// Black's code at ov021 0x021bf58c-0x021c0598. No name is known: the module and address. Its functions are pokebw2's
// field_fade.c's after the fade task (Black's field_fade.c at 0x021b2ebc); its .rodata follows field_transition.c's

#include "types.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_task.h"
#include "field/zone.h"
#include "gfl/gx_layers.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "field/field_camera.h"
#include "field/event_season_banner.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "nitro/os.h"
#include "field/building_enter_effect.h"
#include "system/wipe.h"
#include "gfl/sound.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_g3d_mapper.h"
#include "field/field_prop.h"
#include "gfl/key.h"
#include "nitro/fx/fx_vec.h"

// The work of the quicksand arrival, spin down and move anime events (pokebw2's FallEventWork; they allocate 0x68)
typedef struct {
    GameSystem *gsys;
    Field *field;
    u32 transformType;
} FallEventWork;

// The work of the spin zoom events (pokebw2's SpinEventWork, 0x68 bytes)
typedef struct {
    GameSystem *gsys;
    Field *field;
    u32 transformType;
    FieldCameraAnimation anim;
    FieldCameraAnimationController *controller;
    u32 hasWarp;
    u32 warpA;
    u32 warpB;
} SpinEventWork;

// The work of the quicksand draw-in, escape rope, dig and teleport events (pokebw2's EffectEventWork, 0x7c bytes)
typedef struct {
    GameSystem *gsys;
    Field *field;
    u32 transformType;
    s32 frame;
    BOOL teleportFlag;
    VecFx32 pos;
    FieldPropHandle *propHandle;
    FieldCameraAnimation anim;
    FieldCameraAnimationController *controller;
    s32 timer;
    BOOL seasonChanged;
} EffectEventWork;

GameEventReturnCode func_ov021_021bf694(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf76c(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf8ec(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bfa6c(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bfb90(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bfc98(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventQuicksandDrawIn_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021c00c8(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021c021c(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventTeleportEffect_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021c04c0(GameEvent *event, u32 *state, void *data);
FieldTask *FieldActorSpinTask_CreatePlayerDeaccel(Field *field, u16 duration, u8 direction, int param_4);
FieldTask *FieldActorEffectTask_Create(Field *field, FieldActor *actor, u16 param_3, u16 param_4);
u32 FieldCamera_GetTransformType(FieldCamera *camera);
void func_ov021_021bf7f8(SpinEventWork *work);
void func_ov021_021bf860(SpinEventWork *param_1);
void func_ov021_021bf874(SpinEventWork *param_1);
GameEvent *func_ov021_021bf898(SpinEventWork *param_1);
void func_ov021_021bf8dc(void *data);
void func_ov021_021bf978(SpinEventWork *work);
void func_ov021_021bf9e0(SpinEventWork *param_1);
void func_ov021_021bf9f4(SpinEventWork *param_1);
GameEvent *func_ov021_021bfa18(SpinEventWork *param_1);
void func_ov021_021bfa5c(void *data);
void func_ov021_021c0140(EffectEventWork *wk);
void func_ov021_021c017c(EffectEventWork *work);
void func_ov021_021c01e4(EffectEventWork *wk);
GameEvent *func_ov021_021c01f8(EffectEventWork *wk);
void func_ov021_021c029c(EffectEventWork *wk);
void func_ov021_021c02f8(EffectEventWork *work);
void func_ov021_021c0360(EffectEventWork *wk);
GameEvent *func_ov021_021c0374(EffectEventWork *wk);

FieldTask *FieldFadeTask_Create(Field *, u32, u32, u32, u32);
FieldTask *FieldWaitTask_Create(Field *, u16);
FieldTask *func_ov021_021c754c(Field *field, u32 duration, VecFx32 *pos);

GameEvent *EventQuicksandArrive_Create(GameEvent *event, GameSystem *gsys, Field *field) {
    GameEvent *event_00;
    FallEventWork *data;

    event_00 = GameEvent_Create(gsys, event, func_ov021_021bf694, 0x68);
    data = GameEvent_GetData(event_00);
    data->field = field;
    data->gsys = gsys;
    return event_00;
}

GameEvent *func_ov021_021bf5b4(GameEvent *param_1, GameSystem *param_2, Field *param_3, BOOL param_4, int param_5,
                               int param_6) {
    GameEvent *event;
    SpinEventWork *data;

    event = GameEvent_Create(param_2, param_1, func_ov021_021bf76c, sizeof(SpinEventWork));
    data = GameEvent_GetData(event);
    data->field = param_3;
    data->gsys = param_2;
    data->hasWarp = param_4;
    data->warpA = param_5;
    data->warpB = param_6;
    return event;
}

GameEvent *func_ov021_021bf5e8(GameEvent *param_1, GameSystem *param_2, Field *param_3, BOOL param_4, int param_5,
                               int param_6) {
    GameEvent *event;
    SpinEventWork *data;

    event = GameEvent_Create(param_2, param_1, func_ov021_021bf8ec, sizeof(SpinEventWork));
    data = GameEvent_GetData(event);
    data->field = param_3;
    data->gsys = param_2;
    data->hasWarp = param_4;
    data->warpA = param_5;
    data->warpB = param_6;
    return event;
}

GameEvent *func_ov021_021bf61c(GameEvent *event, GameSystem *gsys, Field *field) {
    GameEvent *event_00;
    SpinEventWork *data;

    event_00 = GameEvent_Create(gsys, event, func_ov021_021bfa6c, sizeof(SpinEventWork));
    data = GameEvent_GetData(event_00);
    data->field = field;
    data->gsys = gsys;
    return event_00;
}

GameEvent *EventPlayerSpinDown_Create(GameEvent *event, GameSystem *gsys, Field *field) {
    GameEvent *event_00;
    FallEventWork *data;

    event_00 = GameEvent_Create(gsys, event, func_ov021_021bfb90, 0x68);
    data = GameEvent_GetData(event_00);
    data->field = field;
    data->gsys = gsys;
    return event_00;
}

GameEvent *func_ov021_021bf66c(GameEvent *event, GameSystem *gsys, Field *field) {
    GameEvent *event_00;
    FallEventWork *data;

    event_00 = GameEvent_Create(gsys, event, func_ov021_021bfc98, 0x68);
    data = GameEvent_GetData(event_00);
    data->field = field;
    data->gsys = gsys;
    return event_00;
}

GameEventReturnCode func_ov021_021bf694(GameEvent *event, u32 *state, void *data) {
    FallEventWork *work = data;
    FieldCamera *camera = Field_GetCameraSystem(work->field);

    switch (*state) {
    case 0: {
        FieldActor *actor;
        FieldTask *task;

        work->transformType = FieldCamera_GetTransformType(camera);
        FieldCamera_ChangeTransformType(camera, 0);
        {
            VecFx32 offset = { 0, 0xfa000, 0 };
            actor = FieldPlayer_GetActor(Field_GetPlayer(work->field));
            SetActorWPosOffset(actor, &offset);
        }
        FldAct_SetAcmd(actor, 1);
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(work->gsys, work->field, 0, 1, 1, 0, 0));
        actor = FieldPlayer_GetActor(Field_GetPlayer(work->field));
        task = FieldActorFallTask_Create(work->field, actor, 0x28, 0xfa);
        FieldTaskManager_AddTask(Field_GetTaskManager(work->field), task, 0);
        (*state)++;
        break;
    }
    case 1:
        if (FieldTaskManager_IsIdle(Field_GetTaskManager(work->field))) {
            (*state)++;
        }
        break;
    case 2:
        FieldCamera_ChangeTransformType(camera, work->transformType);
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf76c(GameEvent *event, u32 *state, void *data) {
    SpinEventWork *work = data;
    Field *field = work->field;
    FieldCamera *camera = Field_GetCameraSystem(field);
    FieldTaskManager *taskManager = Field_GetTaskManager(field);

    switch (*state) {
    case 0:
        FieldCamera_FinishDelay(camera);
        (*state)++;
        break;
    case 1:
        if (!FieldCamera_IsDelayActive(camera)) {
            (*state)++;
        }
        break;
    case 2:
        if (work->hasWarp == 0) {
            func_ov021_021bf874(work);
            func_ov021_021bf7f8(work);
        }
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, func_ov021_021bf898(work));
        (*state)++;
        break;
    case 4:
        if (FieldTaskManager_IsIdle(taskManager)) {
            (*state)++;
        }
        break;
    case 5:
        func_ov021_021bf860(work);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

void func_ov021_021bf7f8(SpinEventWork *work) {
    FieldCamera *camera = Field_GetCameraSystem(work->field);

    work->controller = Field_CreateCameraAnimationController(work->field);
    work->anim.frames = 60;
    work->anim.unk48 = TRUE;
    work->anim.unk4C = TRUE;
    FieldCameraAnimationController_SetAnimation(work->controller, &work->anim);
    FieldCameraAnimationController_PrepareCamera(work->controller);
    FieldCamera_GetAnimationCoords(camera, &work->anim.dst);
    work->anim.src = work->anim.dst;
    work->anim.src.distance -= FX32_CONST(100);
    FieldCameraAnimationController_CommitSrcCoords(work->controller);
    FieldCameraAnimationController_StartAnimation(work->controller);
}

void func_ov021_021bf860(SpinEventWork *param_1) {
    FieldCameraAnimationController_FinishCamera(param_1->controller);
    FieldCameraAnimationController_FreeAnimation(param_1->controller);
}

void func_ov021_021bf874(SpinEventWork *param_1) {
    FieldTaskManager *mgr;
    FieldTask *task;

    mgr = Field_GetTaskManager(param_1->field);
    task = FieldActorSpinTask_CreatePlayerDeaccel(param_1->field, 0x3c, 8, 1);
    FieldTaskManager_AddTask(mgr, task, 0);
}

GameEvent *func_ov021_021bf898(SpinEventWork *param_1) {
    GameSystem *gsys = param_1->gsys;
    Field *field = param_1->field;
    if (param_1->hasWarp != 0) {
        return func_ov021_021be9a0(gsys, field, (u8)param_1->warpA, (u8)param_1->warpB, func_ov021_021bf8dc, param_1);
    }
    return CallFieldMapEntranceInTransition(gsys, field, 1, 1, 1, 0, 0);
}

void func_ov021_021bf8dc(void *data) {
    SpinEventWork *wk = data;

    func_ov021_021bf874(wk);
    func_ov021_021bf7f8(wk);
}

GameEventReturnCode func_ov021_021bf8ec(GameEvent *event, u32 *state, void *data) {
    SpinEventWork *work = data;
    Field *field = work->field;
    FieldCamera *camera = Field_GetCameraSystem(field);
    FieldTaskManager *taskManager = Field_GetTaskManager(field);

    switch (*state) {
    case 0:
        FieldCamera_FinishDelay(camera);
        (*state)++;
        break;
    case 1:
        if (!FieldCamera_IsDelayActive(camera)) {
            (*state)++;
        }
        break;
    case 2:
        if (work->hasWarp == 0) {
            func_ov021_021bf9f4(work);
            func_ov021_021bf978(work);
        }
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, func_ov021_021bfa18(work));
        (*state)++;
        break;
    case 4:
        if (FieldTaskManager_IsIdle(taskManager)) {
            (*state)++;
        }
        break;
    case 5:
        func_ov021_021bf9e0(work);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

void func_ov021_021bf978(SpinEventWork *work) {
    FieldCamera *camera = Field_GetCameraSystem(work->field);

    work->controller = Field_CreateCameraAnimationController(work->field);
    work->anim.frames = 60;
    work->anim.unk48 = TRUE;
    work->anim.unk4C = TRUE;
    FieldCameraAnimationController_SetAnimation(work->controller, &work->anim);
    FieldCameraAnimationController_PrepareCamera(work->controller);
    FieldCamera_GetAnimationCoords(camera, &work->anim.dst);
    work->anim.src = work->anim.dst;
    work->anim.src.distance -= FX32_CONST(100);
    FieldCameraAnimationController_CommitSrcCoords(work->controller);
    FieldCameraAnimationController_StartAnimation(work->controller);
}

void func_ov021_021bf9e0(SpinEventWork *param_1) {
    FieldCameraAnimationController_FinishCamera(param_1->controller);
    FieldCameraAnimationController_FreeAnimation(param_1->controller);
}

void func_ov021_021bf9f4(SpinEventWork *param_1) {
    FieldTaskManager *mgr;
    FieldTask *task;

    mgr = Field_GetTaskManager(param_1->field);
    task = FieldActorSpinTask_CreatePlayerDeaccel(param_1->field, 0x3c, 8, 1);
    FieldTaskManager_AddTask(mgr, task, 0);
}

GameEvent *func_ov021_021bfa18(SpinEventWork *param_1) {
    GameSystem *gsys = param_1->gsys;
    Field *field = param_1->field;
    if (param_1->hasWarp != 0) {
        return func_ov021_021be9a0(gsys, field, (u8)param_1->warpA, (u8)param_1->warpB, func_ov021_021bfa5c, param_1);
    }
    return CallFieldMapEntranceInTransition(gsys, field, 1, 1, 1, 0, 0);
}

void func_ov021_021bfa5c(void *data) {
    SpinEventWork *wk = data;

    func_ov021_021bf9f4(wk);
    func_ov021_021bf978(wk);
}

GameEventReturnCode func_ov021_021bfa6c(GameEvent *event, u32 *state, void *data) {
    SpinEventWork *work = data;
    GameSystem *gsys = work->gsys;
    Field *field = work->field;
    FieldCamera *camera = Field_GetCameraSystem(field);
    FieldTaskManager *taskManager = Field_GetTaskManager(field);

    switch (*state) {
    case 0:
        FieldCamera_FinishDelay(camera);
        (*state)++;
        break;
    case 1:
        if (!FieldCamera_IsDelayActive(camera)) {
            (*state)++;
        }
        break;
    case 2:
        FieldTaskManager_AddTask(taskManager, FieldActorSpinTask_CreatePlayerDeaccel(field, 0x3c, 8, 1), 0);
        work->controller = Field_CreateCameraAnimationController(field);
        work->anim.frames = 0x3c;
        work->anim.unk48 = TRUE;
        work->anim.unk4C = TRUE;
        FieldCameraAnimationController_SetAnimation(work->controller, &work->anim);
        FieldCameraAnimationController_PrepareCamera(work->controller);
        FieldCamera_GetAnimationCoords(camera, &work->anim.dst);
        work->anim.src = work->anim.dst;
        work->anim.src.distance = work->anim.src.distance - (0x19 << 0xe);
        FieldCameraAnimationController_CommitSrcCoords(work->controller);
        FieldCameraAnimationController_StartAnimation(work->controller);
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 0, 1, 1, 0, 0));
        (*state)++;
        break;
    case 4: {
        FieldActor *actor;

        if (FieldTaskManager_IsIdle(taskManager)) {
            actor = FieldPlayer_GetActor(Field_GetPlayer(field));
            func_ov021_021ac528(actor, Field_GetFieldEffects(field));
            (*state)++;
        }
        break;
    }
    case 5:
        FieldCameraAnimationController_FinishCamera(work->controller);
        FieldCameraAnimationController_FreeAnimation(work->controller);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bfb90(GameEvent *event, u32 *state, void *data) {
    FallEventWork *work = data;
    Field *field = work->field;
    FieldCamera *camera = Field_GetCameraSystem(field);

    switch (*state) {
    case 0: {
        VecFx32 offset;
        VecFx32 target;
        FieldTask *spin;
        FieldTask *move;
        FieldTaskManager *taskManager;
        FieldActor *actor;

        work->transformType = FieldCamera_GetTransformType(camera);
        FieldCamera_ChangeTransformType(camera, 0);
        actor = FieldPlayer_GetActor(Field_GetPlayer(field));
        offset.y = 0xa000;
        offset.x = 0;
        offset.z = 0;
        SetActorWPosOffset(actor, &offset);
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(work->gsys, field, 0, 1, 1, 0, 0));
        target.x = 0;
        target.y = 0x96000;
        target.z = 0;
        spin = FieldActorSpinTask_CreatePlayer(field, 0x18, 3);
        move = FieldActorMoveTask_CreatePlayer(field, -0x18, &target);
        taskManager = Field_GetTaskManager(field);
        FieldTaskManager_AddTask(taskManager, spin, 0);
        FieldTaskManager_AddTask(taskManager, move, 0);
        (*state)++;
        break;
    }
    case 1:
        if (FieldTaskManager_IsIdle(Field_GetTaskManager(field)) != 0) {
            (*state)++;
        }
        break;
    case 2:
        func_ov021_021ac528(FieldPlayer_GetActor(Field_GetPlayer(field)), Field_GetFieldEffects(work->field));
        GFL_SndSEPlay(0x67b);
        (*state)++;
        break;
    case 3:
        FieldCamera_ChangeTransformType(camera, work->transformType);
        (*state)++;
        break;
    case 4:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bfc98(GameEvent *event, u32 *state, void *data) {
    FallEventWork *work = data;
    FieldCamera *camera = Field_GetCameraSystem(work->field);
    FieldActor *actor = FieldPlayer_GetActor(Field_GetPlayer(work->field));

    switch (*state) {
    case 0:
        work->transformType = FieldCamera_GetTransformType(camera);
        FieldCamera_ChangeTransformType(camera, 0);
        FldAct_SetAcmd(actor, 0x44);
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(work->gsys, work->field, 0, 1, 1, 0, 0));
        (*state)++;
        break;
    case 1:
        if (func_ov010_0216d454(actor) == 1) {
            func_ov010_0216d480(actor);
            (*state)++;
        }
        break;
    case 2:
        FieldCamera_ChangeTransformType(camera, work->transformType);
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventQuicksandDrawIn_Create(GameEvent *parent, GameSystem *gsys, Field *field, const VecFx32 *pos) {
    GameEvent *event;
    EffectEventWork *work;

    event = GameEvent_Create(gsys, parent, EventQuicksandDrawIn_Callback, sizeof(EffectEventWork));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->field = field;
    work->frame = 0;
    work->propHandle = NULL;
    VEC_Set(&work->pos, pos->x, pos->y, pos->z);
    return event;
}

GameEvent *func_ov021_021bfd70(GameEvent *event, GameSystem *gsys, Field *field, BOOL seasonChanged) {
    GameEvent *event_00;
    EffectEventWork *data;

    event_00 = GameEvent_Create(gsys, event, func_ov021_021c00c8, sizeof(EffectEventWork));
    data = GameEvent_GetData(event_00);
    data->gsys = gsys;
    data->field = field;
    data->frame = 0;
    data->timer = 0;
    data->seasonChanged = seasonChanged;
    return event_00;
}

GameEvent *EventDig_Create(GameEvent *event, GameSystem *gsys, Field *field, BOOL seasonChanged) {
    GameEvent *event_00;
    EffectEventWork *data;

    event_00 = GameEvent_Create(gsys, event, func_ov021_021c021c, sizeof(EffectEventWork));
    data = GameEvent_GetData(event_00);
    data->gsys = gsys;
    data->field = field;
    data->frame = 0;
    data->timer = 0;
    data->seasonChanged = seasonChanged;
    return event_00;
}

GameEvent *func_ov021_021bfdd0(GameEvent *event, GameSystem *gsys, Field *field, BOOL flag) {
    GameEvent *event_00;
    EffectEventWork *data;

    event_00 = GameEvent_Create(gsys, event, EventTeleportEffect_Callback, sizeof(EffectEventWork));
    data = GameEvent_GetData(event_00);
    data->gsys = gsys;
    data->field = field;
    data->frame = 0;
    data->teleportFlag = flag;
    return event_00;
}

GameEvent *func_ov021_021bfe00(GameEvent *event, GameSystem *gsys, Field *field) {
    GameEvent *event_00;
    EffectEventWork *data;

    event_00 = GameEvent_Create(gsys, event, func_ov021_021c04c0, sizeof(EffectEventWork));
    data = GameEvent_GetData(event_00);
    data->gsys = gsys;
    data->field = field;
    data->frame = 0;
    return event_00;
}

GameEventReturnCode EventQuicksandDrawIn_Callback(GameEvent *event, u32 *state, void *data) {
    EffectEventWork *wk = data;
    FieldPlayer *player = Field_GetPlayer(wk->field);
    FieldActor *actor = FieldPlayer_GetActor(player);
    FieldEffects *effects = Field_GetFieldEffects(wk->field);
    FieldCamera *camera = Field_GetCameraSystem(wk->field);

    switch (*state) {
    case 0:
        wk->transformType = FieldCamera_GetTransformType(camera);
        FieldCamera_ChangeTransformType(camera, 0);
        *state = 1;
        break;
    case 1: {
        FieldPropSystem *propSystem;
        FieldPropAreaBounds bounds;
        FieldChunkPropHolder **props;
        u32 count;

        GFL_SEPlayKeepVol(0x680, GFL_SndSeqGetPlayerIndex(0x680));
        propSystem = FieldG3DMapper_GetBMSystem(Field_GetG3DMapper(wk->field));
        bounds.minX = wk->pos.x - FX32_CONST(32);
        bounds.maxX = wk->pos.x + FX32_CONST(32);
        bounds.minZ = wk->pos.z - FX32_CONST(32);
        bounds.maxZ = wk->pos.z + FX32_CONST(32);
        props = FieldPropSystem_FindPropsInArea(propSystem, &bounds, 2, &count);
        if (*props != NULL) {
            wk->propHandle = FieldPropSystem_CreateHandleFromExisting(propSystem, *props);
            FieldPropHandle_CallAnmCmd(wk->propHandle, 0, 0);
        } else {
            wk->propHandle = NULL;
        }
        GFL_HeapFree(props);
        *state = 2;
        break;
    }
    case 2: {
        VecFx32 playerPos;
        s32 dx;
        s32 dz;
        s32 duration;

        FieldPlayer_GetWPos(player, &playerPos);
        dx = (playerPos.x >> 12) / 16 - (wk->pos.x >> 12) / 16;
        dz = (playerPos.z >> 12) / 16 - (wk->pos.z >> 12) / 16;
        if (dx < 0) {
            dx = -dx;
        }
        if (dz < 0) {
            dz = -dz;
        }
        if (dx == 0 && dz == 0) {
            duration = 0;
        } else if (dx < 2 && dz < 2) {
            duration = 0x3c;
        } else {
            duration = 0x78;
        }
        if (duration > 0) {
            FieldTask *task = func_ov021_021c754c(wk->field, duration, &wk->pos);

            FieldTaskManager_AddTask(Field_GetTaskManager(wk->field), task, 0);
            *state = 3;
        } else {
            *state = 4;
        }
        wk->frame = 0;
        break;
    }
    case 3: {
        u32 keys;

        if (wk->propHandle != NULL && FieldPropHandle_IsAnmIdle(wk->propHandle, 0)) {
            FieldPropHandle_CallAnmCmd(wk->propHandle, 0, 0);
        }
        keys = GCTX_HIDGetHeldKeys();
        if (keys & 0x40) {
            FldAct_SetAcmd(actor, 0x24);
        }
        if (keys & 0x80) {
            FldAct_SetAcmd(actor, 0x25);
        }
        if (keys & 0x20) {
            FldAct_SetAcmd(actor, 0x26);
        }
        if (keys & 0x10) {
            FldAct_SetAcmd(actor, 0x27);
        }
        if (wk->frame++ % 10 == 0) {
            func_ov021_021ac528(actor, effects);
        }
        if (FieldTaskManager_IsIdle(Field_GetTaskManager(wk->field))) {
            *state = 4;
        }
        break;
    }
    case 4: {
        VecFx32 move;
        FieldTask *moveTask;
        FieldTask *spin;
        FieldTask *zoom;
        FieldTask *fade;
        FieldTaskManager *taskManager;

        GFL_SndSEPlay(0x7a0);
        SetActorMovementFlag(actor, 0x8000);
        move.x = 0;
        move.y = FX32_CONST(-30);
        move.z = 0;
        moveTask = FieldActorMoveTask_CreatePlayer(wk->field, 0x22, &move);
        spin = FieldActorSpinTask_CreatePlayer(wk->field, 0x22, 5);
        zoom = FieldCameraMoveTaskZoom_Create(wk->field, 0x1e, FX32_CONST(180));
        fade = FieldFadeTask_Create(wk->field, 3, 0, 16, 0);
        taskManager = Field_GetTaskManager(wk->field);
        FieldTaskManager_AddTask(taskManager, moveTask, 0);
        FieldTaskManager_AddTask(taskManager, spin, 0);
        FieldTaskManager_AddTask(taskManager, zoom, 0);
        FieldTaskManager_AddTask(taskManager, fade, zoom);
        wk->frame = 0;
        *state = 5;
        break;
    }
    case 5:
        if (FieldTaskManager_IsIdle(Field_GetTaskManager(wk->field))) {
            *state = 6;
        }
        break;
    case 6:
        GFL_SndPlayerStop(GFL_SndSeqGetPlayerIndex(0x680));
        FieldCamera_ChangeTransformType(camera, wk->transformType);
        if (wk->propHandle != NULL) {
            FieldPropHandle_Free(wk->propHandle);
        }
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021c00c8(GameEvent *event, u32 *state, void *data) {
    EffectEventWork *work = data;
    FieldTaskManager *taskManager = Field_GetTaskManager(work->field);
    s32 counter;

    switch (*state) {
    case 0:
        func_ov021_021c0140(work);
        func_ov021_021c017c(work);
        (*state)++;
        break;
    case 1:
        counter = work->timer;
        work->timer = counter + 1;
        if (counter > 0x23) {
            GameEvent_ChainNext(event, func_ov021_021c01f8(work));
            (*state)++;
            break;
        }
        /* fall through */
    case 2:
        if (FieldTaskManager_IsIdle(taskManager) != 0) {
            GFL_SndSEPlay(0x559);
            (*state)++;
        }
        break;
    case 3:
        func_ov021_021c01e4(work);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

void func_ov021_021c0140(EffectEventWork *wk) {
    FieldTaskManager *mgr;
    FieldTask *task;
    FieldTask *task_00;
    Field *field;

    field = wk->field;
    mgr = Field_GetTaskManager(field);
    task = FieldActorSpinTask_CreatePlayerAccel(field, 0x14, 3);
    task_00 = FieldActorSpinTask_CreatePlayer(field, 0x14, 5);
    FieldTaskManager_AddTask(mgr, task, 0);
    FieldTaskManager_AddTask(mgr, task_00, task);
}

void func_ov021_021c017c(EffectEventWork *work) {
    FieldCamera *camera = Field_GetCameraSystem(work->field);

    work->controller = Field_CreateCameraAnimationController(work->field);
    work->anim.frames = 60;
    work->anim.unk48 = TRUE;
    work->anim.unk4C = TRUE;
    FieldCameraAnimationController_SetAnimation(work->controller, &work->anim);
    FieldCameraAnimationController_PrepareCamera(work->controller);
    FieldCamera_GetAnimationCoords(camera, &work->anim.src);
    work->anim.dst = work->anim.src;
    work->anim.dst.distance -= FX32_CONST(100);
    FieldCameraAnimationController_CommitSrcCoords(work->controller);
    FieldCameraAnimationController_StartAnimation(work->controller);
}

void func_ov021_021c01e4(EffectEventWork *wk) {
    FieldCameraAnimationController_FinishCamera(wk->controller);
    FieldCameraAnimationController_FreeAnimation(wk->controller);
}

GameEvent *func_ov021_021c01f8(EffectEventWork *wk) {
    GameSystem *gsys = wk->gsys;
    Field *field = wk->field;
    if (wk->seasonChanged != 0) {
        return CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0);
    }
    return CallFieldMapEntranceOutTransitionDefault(gsys, field, 1, 0);
}

GameEventReturnCode func_ov021_021c021c(GameEvent *event, u32 *state, void *data) {
    EffectEventWork *work = data;
    FieldTaskManager *taskManager = Field_GetTaskManager(work->field);
    s32 counter;

    switch (*state) {
    case 0:
        func_ov021_021c029c(work);
        func_ov021_021c02f8(work);
        GFL_SndSEPlay(0x6df);
        (*state)++;
        break;
    case 1:
        counter = work->timer;
        work->timer = counter + 1;
        if (counter > 0x23) {
            GameEvent_ChainNext(event, func_ov021_021c0374(work));
            (*state)++;
            break;
        }
        /* fall through */
    case 2:
        if (FieldTaskManager_IsIdle(taskManager) != 0) {
            GFL_SndSEPlay(0x559);
            (*state)++;
        }
        break;
    case 3:
        func_ov021_021c0360(work);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

void func_ov021_021c029c(EffectEventWork *wk) {
    FieldTaskManager *mgr;
    FieldPlayer *player;
    FieldActor *pFVar1;
    FieldTask *task;
    FieldTask *task_00;
    FieldTask *task_01;
    Field *field;

    field = wk->field;
    mgr = Field_GetTaskManager(field);
    player = Field_GetPlayer(field);
    pFVar1 = FieldPlayer_GetActor(player);
    task = FieldActorEffectTask_Create(field, pFVar1, 0x28, 6);
    task_00 = FieldActorSpinTask_CreatePlayerAccel(field, 0x14, 4);
    task_01 = FieldActorSpinTask_CreatePlayer(field, 0x14, 5);
    FieldTaskManager_AddTask(mgr, task, 0);
    FieldTaskManager_AddTask(mgr, task_00, 0);
    FieldTaskManager_AddTask(mgr, task_01, task_00);
}

void func_ov021_021c02f8(EffectEventWork *work) {
    FieldCamera *camera = Field_GetCameraSystem(work->field);

    work->controller = Field_CreateCameraAnimationController(work->field);
    work->anim.frames = 60;
    work->anim.unk48 = TRUE;
    work->anim.unk4C = TRUE;
    FieldCameraAnimationController_SetAnimation(work->controller, &work->anim);
    FieldCameraAnimationController_PrepareCamera(work->controller);
    FieldCamera_GetAnimationCoords(camera, &work->anim.src);
    work->anim.dst = work->anim.src;
    work->anim.dst.distance -= FX32_CONST(100);
    FieldCameraAnimationController_CommitSrcCoords(work->controller);
    FieldCameraAnimationController_StartAnimation(work->controller);
}

void func_ov021_021c0360(EffectEventWork *wk) {
    FieldCameraAnimationController_FinishCamera(wk->controller);
    FieldCameraAnimationController_FreeAnimation(wk->controller);
}

GameEvent *func_ov021_021c0374(EffectEventWork *wk) {
    GameSystem *gsys = wk->gsys;
    Field *field = wk->field;
    if (wk->seasonChanged != 0) {
        return CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0);
    }
    return CallFieldMapEntranceOutTransitionDefault(gsys, field, 1, 0);
}

GameEventReturnCode EventTeleportEffect_Callback(GameEvent *event, u32 *state, void *data) {
    EffectEventWork *work = data;
    Field *field = work->field;
    FieldCamera *camera;
    FieldTaskManager *taskManager;

    Field_GetPlayer(field);
    camera = Field_GetCameraSystem(work->field);
    taskManager = Field_GetTaskManager(field);

    switch (*state) {
    case 0: {
        FieldTask *spinAccel;
        FieldTask *spin;
        FieldTask *wait;
        FieldTask *sound;
        spinAccel = FieldActorSpinTask_CreatePlayerAccel(field, 0x14, 3);
        spin = FieldActorSpinTask_CreatePlayer(field, 0x14, 5);
        wait = FieldWaitTask_Create(field, 0x1e);
        sound = FieldFadeTask_Create(field, 3, 0, 0x10, 1);
        FieldTaskManager_AddTask(taskManager, spinAccel, 0);
        FieldTaskManager_AddTask(taskManager, spin, spinAccel);
        FieldTaskManager_AddTask(taskManager, wait, 0);
        FieldTaskManager_AddTask(taskManager, sound, wait);
        if (work->teleportFlag != 0) {
            GFL_SndSEPlay(0x6e7);
        } else {
            GFL_SndSEPlay(0x676);
        }
        work->controller = Field_CreateCameraAnimationController(field);
        work->anim.frames = 0x3c;
        work->anim.unk48 = TRUE;
        work->anim.unk4C = TRUE;
        FieldCameraAnimationController_SetAnimation(work->controller, &work->anim);
        FieldCameraAnimationController_PrepareCamera(work->controller);
        FieldCamera_GetAnimationCoords(camera, &work->anim.src);
        work->anim.dst = work->anim.src;
        work->anim.dst.distance -= FX32_CONST(100);
        FieldCameraAnimationController_CommitSrcCoords(work->controller);
        FieldCameraAnimationController_StartAnimation(work->controller);
        (*state)++;
        break;
    }
    case 1:
        if (FieldTaskManager_IsIdle(taskManager) != 0) {
            (*state)++;
        }
        break;
    case 2:
        FieldCameraAnimationController_FinishCamera(work->controller);
        FieldCameraAnimationController_FreeAnimation(work->controller);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021c04c0(GameEvent *event, u32 *state, void *data) {
    EffectEventWork *work = data;
    Field *field = work->field;
    FieldCamera *camera;

    Field_GetPlayer(field);
    camera = Field_GetCameraSystem(field);

    switch (*state) {
    case 0: {
        VecFx32 target;
        FieldTask *spin;
        FieldTask *sound;
        FieldTask *move;
        FieldTaskManager *taskManager;

        work->transformType = FieldCamera_GetTransformType(camera);
        FieldCamera_ChangeTransformType(camera, 0);
        target.x = 0;
        target.y = 0x96000;
        target.z = 0;
        spin = FieldActorSpinTask_CreatePlayer(field, 0x18, 3);
        sound = FieldFadeTask_Create(field, 3, 0, 0x10, 1);
        move = FieldActorMoveTask_CreatePlayer(field, 0x18, &target);
        taskManager = Field_GetTaskManager(field);
        FieldTaskManager_AddTask(taskManager, spin, 0);
        FieldTaskManager_AddTask(taskManager, move, 0);
        FieldTaskManager_AddTask(taskManager, sound, move);
        GFL_SndSEPlay(0x676);
        (*state)++;
        break;
    }
    case 1:
        if (FieldTaskManager_IsIdle(Field_GetTaskManager(field)) != 0 && !GFL_FadeIsRunning()) {
            (*state)++;
        }
        break;
    case 2:
        FieldCamera_ChangeTransformType(camera, work->transformType);
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}
