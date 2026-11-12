// Black's code at ov021 0x021c6ea8-0x021c7dac. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_task.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "system/fp.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_vec.h"
#include "struct_decls.h"
#include "field/field_camera.h"
#include "gfl/sound.h"
#include "gfl/calctool.h"
#include "field/ov021_021c6ea8.h"

typedef struct {
    u16 v[4];
} U16Array4;

// The tasks' work, laid out and named as pokebw2's field_task_manager.c has them (Black's offsets are the same)
typedef struct {
    Field *field;
    u8 state;
    u16 frame;
    u16 duration;
    fx32 start;
    fx32 target;
} FieldCameraZoomTask;

typedef struct {
    Field *field;
    FieldCamera *camera;
    u32 axis; // 0 yaw, 1 pitch
    u8 state;
    u16 frame;
    u16 duration;
    u32 start;
    u32 target;
} FieldCameraAngleTask;

typedef struct {
    Field *field;
    FieldActor *actor;
    u16 frame;
    u16 duration;
    u8 turns;
    u16 dir;
} FieldActorSpinTask;

typedef struct {
    u8 state;
    Field *field;
    s32 frame;
    s32 duration;
    VecFx32 start;
    VecFx32 end;
} FieldPlayerMoveTask;

typedef struct {
    u8 state;
    Field *field;
    u16 frame;
    u16 duration;
    f32 height;
    FieldActor *actor;
} FieldActorFallTask;

typedef struct {
    u16 frame;
    u16 wait;
} FieldWaitTask;

typedef struct {
    Field *field;
    u32 mode;
    s32 from;
    s32 to;
    s32 speed;
} FieldFadeTask;

typedef struct {
    u8 state;
    Field *field;
    s32 direction; // 0 forward, 1 backward
    u16 frame;
    u16 duration;
    VecFx32 offset;
    VecFx32 start;
    FieldActor *actor;
} FieldActorMoveTask;

typedef struct {
    u8 state;
    Field *field;
    s32 frame;
    s32 duration;
    VecFx32 start;
    VecFx32 end;
} FieldCameraTargetOffsTask;

typedef struct {
    u8 state;
    Field *field;
    u16 frame;
    u16 duration;
    VecFx32 from;
    VecFx32 to;
    VecFx32 delta;
    u32 transformType;
} FieldCameraTargetTask;

typedef struct {
    Field *field;
    FieldEffects *effects;
    FieldActor *actor;
    u8 state;
    u16 frame;
    u16 duration;
    u16 interval;
} FieldActorEffectTask;

// Defined in field_task.c and field_task_manager.c, typed as pokebw2's field_task.h has them. Those definitions
// still take and return integers, so these wait here rather than in field_task.h
FieldTask *FieldTask_Create(HeapID heapId, u32 dataSize, BOOL (*callback)(void *work));
void *FieldTask_GetData(FieldTask *task);
u32 FieldTask_GetState(FieldTask *task);
void FieldTask_SetStarted(FieldTask *task);
BOOL FieldTask_CheckStateReqEnd(FieldTask *task);
void FieldTaskManager_EndTask(FieldTaskManager *mgr, u8 index);

BOOL FieldCameraMoveTaskZoom_Calc(void *work);
void FieldCameraMoveTaskAngle_CalcStartValues(FieldCameraAngleTask *data);
void FieldCameraMoveTaskAngle_Calc(FieldCameraAngleTask *data);
BOOL FieldCameraMoveTaskAngle_Callback(void *work);
u16 FieldActorSpinTask_CalcSpinDirAcmdSqrt(f32 elapsed, f32 duration, f32 turns);
BOOL FieldActorSpinTask_Callback(void *work);
BOOL func_ov021_021c748c(void *work);
BOOL FieldActorSpinTask_CallbackDeaccel(void *work);
BOOL FieldPlayerMoveTask_Callback(void *work);
BOOL FieldActorFallTask_Callback(void *work);
BOOL func_ov021_021c77fc(void *work);
BOOL FieldFadeTask_Callback(void *work);
void FieldActorMoveTask_InterpVector(VecFx32 *dest, u16 elapsed, VecFx32 *vec, u16 duration);
BOOL func_ov021_021c7a20(void *work);
BOOL func_ov021_021c7a7c(void *work);
BOOL FieldCameraTargetMoveTaskTargetOffs_Callback(void *work);
BOOL FieldCameraMoveTaskTarget_Callback(void *work);
BOOL FieldActorEffectTask_Callback(void *work);

void FieldTaskManager_UpdateParentControlTransfer(FieldTaskManager *mgr, u8 index) {
    if (mgr->tasks[index] != NULL && mgr->parents[index] != NULL) {
        if (FieldTask_GetState(mgr->tasks[index]) == 0 && FieldTask_GetState(mgr->parents[index]) == 2) {
            FieldTask_SetStarted(mgr->tasks[index]);
        }
    }
}

void FieldTaskManager_UpdateEndTasks(FieldTaskManager *mgr, u8 index) {
    if (mgr->tasks[index] != NULL && FieldTask_CheckStateReqEnd(mgr->tasks[index]) == TRUE) {
        FieldTaskManager_EndTask(mgr, index);
    }
}

FieldTask *FieldCameraMoveTaskZoom_Create(Field *field, u16 duration, fx32 zoom) {
    FieldTask *task;
    FieldCameraZoomTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldCameraZoomTask), FieldCameraMoveTaskZoom_Calc);
    data = FieldTask_GetData(task);
    data->state = 0;
    data->field = field;
    data->frame = 0;
    data->duration = duration;
    data->target = zoom;
    return task;
}

BOOL FieldCameraMoveTaskZoom_Calc(void *work) {
    FieldCameraZoomTask *data = work;
    FieldCamera *camera;
    u32 type;
    f32 t;
    f32 s;
    f32 e;
    f32 v;
    s32 k;

    camera = Field_GetCameraSystem(data->field);
    switch (data->state) {
    case 0:
        data->start = FieldCamera_CoordsGetZoom(camera);
        data->state++;
        break;
    case 1:
        data->frame += 1;
        type = FieldCamera_GetTransformType(camera);
        FieldCamera_ChangeTransformType(camera, 0);
        t = (f32)data->frame / (f32)data->duration;
        s = (f32)data->start / 4096.0f;
        e = (f32)data->target / 4096.0f;
        v = (1.0f - t) * s + t * e;
        if (v > 0.0f) {
            k = (s32)(0.5f + 4096.0f * v);
        } else {
            k = (s32)(4096.0f * v - 0.5f);
        }
        FieldCamera_CoordsSetZoom(camera, k);
        FieldCamera_ChangeTransformType(camera, type);
        if (data->duration <= data->frame) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

FieldTask *FieldCameraMoveTaskYaw_Callback(Field *field, u32 duration, u16 yaw) {
    FieldTask *task;
    FieldCameraAngleTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldCameraAngleTask), FieldCameraMoveTaskAngle_Callback);
    data = FieldTask_GetData(task);
    data->field = field;
    data->camera = Field_GetCameraSystem(field);
    data->state = 0;
    data->axis = 0;
    data->frame = 0;
    data->duration = duration;
    data->target = yaw;
    return task;
}

FieldTask *FieldCameraMoveTaskPitch_Callback(Field *field, u32 duration, u16 pitch) {
    FieldTask *task;
    FieldCameraAngleTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldCameraAngleTask), FieldCameraMoveTaskAngle_Callback);
    data = FieldTask_GetData(task);
    data->field = field;
    data->camera = Field_GetCameraSystem(field);
    data->state = 0;
    data->axis = 1;
    data->frame = 0;
    data->duration = duration;
    data->target = pitch;
    return task;
}

void FieldCameraMoveTaskAngle_CalcStartValues(FieldCameraAngleTask *data) {
    FieldCamera *camera = data->camera;
    u32 cur;
    u32 target = data->target;

    switch (data->axis) {
    case 0:
    default:
        cur = FieldCamera_CoordsGetYaw(camera);
        break;
    case 1:
        cur = FieldCamera_CoordsGetPitch(camera);
        break;
    }
    if (cur < target) {
        if (target - cur > 0x8000) {
            cur += 0x10000;
        }
    } else if (target < cur) {
        if (cur - target > 0x8000) {
            target += 0x10000;
        }
    }
    data->start = cur;
    data->target = target;
}

void FieldCameraMoveTaskAngle_Calc(FieldCameraAngleTask *data) {
    FieldCamera *camera = data->camera;
    u16 duration = data->duration;
    u16 frame;
    u16 value;
    u32 transform;
    f32 t;
    f32 s;

    data->frame += 1;
    frame = data->frame;
    if (frame <= duration) {
        t = (f32)frame / (f32)duration;
        s = (3.0f + -2.0f * t) * (t * t);
        value = (u32)((1.0f - s) * (f32)data->start + s * (f32)data->target);
        transform = FieldCamera_GetTransformType(camera);
        FieldCamera_ChangeTransformType(camera, 0);
        switch (data->axis) {
        case 0:
            FieldCamera_CoordsSetYaw(camera, value);
            break;
        case 1:
            FieldCamera_CoordsSetPitch(camera, value);
            break;
        }
        FieldCamera_ChangeTransformType(camera, transform);
    }
}

BOOL FieldCameraMoveTaskAngle_Callback(void *work) {
    FieldCameraAngleTask *data = work;

    switch (data->state) {
    case 0:
        FieldCameraMoveTaskAngle_CalcStartValues(data);
        data->state++;
        break;
    case 1:
        FieldCameraMoveTaskAngle_Calc(data);
        if (data->duration <= data->frame) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

FieldTask *FieldActorSpinTask_CreatePlayer(Field *field, u32 duration, u32 direction) {
    HeapID heapID;
    FieldPlayer *player;
    FieldTask *task;
    FieldActorSpinTask *data;
    FieldActor *actor;

    heapID = Field_GetHeapID(field);
    player = Field_GetPlayer(field);
    task = FieldTask_Create(heapID, sizeof(FieldActorSpinTask), FieldActorSpinTask_Callback);
    data = FieldTask_GetData(task);
    data->field = field;
    actor = FieldPlayer_GetActor(player);
    data->actor = actor;
    data->frame = 0;
    data->duration = duration;
    data->turns = direction;
    data->dir = GetActorFaceDir(actor);
    return task;
}

FieldTask *FieldActorSpinTask_Create(Field *field, u32 duration, u32 direction, FieldActor *actor) {
    FieldTask *task;
    FieldActorSpinTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldActorSpinTask), FieldActorSpinTask_Callback);
    data = FieldTask_GetData(task);
    data->field = field;
    data->actor = actor;
    data->frame = 0;
    data->duration = duration;
    data->turns = direction;
    data->dir = GetActorFaceDir(actor);
    return task;
}

FieldTask *FieldActorSpinTask_CreatePlayerAccel(Field *field, u32 duration, u32 direction) {
    HeapID heapID;
    FieldPlayer *player;
    FieldTask *task;
    FieldActorSpinTask *data;
    FieldActor *actor;

    heapID = Field_GetHeapID(field);
    player = Field_GetPlayer(field);
    task = FieldTask_Create(heapID, sizeof(FieldActorSpinTask), func_ov021_021c748c);
    data = FieldTask_GetData(task);
    data->field = field;
    actor = FieldPlayer_GetActor(player);
    data->actor = actor;
    data->frame = 0;
    data->duration = duration;
    data->turns = direction;
    data->dir = GetActorFaceDir(actor);
    return task;
}

FieldTask *FieldActorSpinTask_CreatePlayerDeaccel(Field *field, u32 duration, u32 turns, u32 dir) {
    HeapID heapID;
    FieldPlayer *player;
    FieldTask *task;
    FieldActorSpinTask *data;

    heapID = Field_GetHeapID(field);
    player = Field_GetPlayer(field);
    task = FieldTask_Create(heapID, sizeof(FieldActorSpinTask), FieldActorSpinTask_CallbackDeaccel);
    data = FieldTask_GetData(task);
    data->field = field;
    data->actor = FieldPlayer_GetActor(player);
    data->frame = 0;
    data->duration = duration;
    data->turns = turns;
    data->dir = dir;
    return task;
}

u16 func_ov021_021c72f8(f32 param_1, f32 param_2, f32 param_3) {
    U16Array4 table = {{1, 3, 0, 2}};
    f32 t;
    s32 idx;


    t = 4.0f * param_3;
    t = t / param_2;
    t = t * param_1;
    idx = (s32)t;
    return table.v[idx % 4];
}

u16 func_ov021_021c734c(f32 param_1, f32 param_2, f32 param_3) {
    U16Array4 table = {{1, 3, 0, 2}};
    f32 t;
    s32 idx;


    t = 4.0f * param_3;
    t = t / (param_2 * param_2);
    t = t * param_1;
    t = param_1 * t;
    idx = (s32)t;
    return table.v[idx % 4];
}




u16 FieldActorSpinTask_CalcSpinDirAcmdSqrt(f32 param_1, f32 param_2, f32 param_3) {
    U16Array4 table = {{1, 3, 0, 2}};
    f32 t;
    s32 idx;


    t = 4.0f * param_3;
    t = t * t;
    t = t / param_2;
    t = t * param_1;
    idx = (s32)(f32)sqrt((double)t);
    return table.v[idx % 4];
}

BOOL FieldActorSpinTask_Callback(void *work) {
    FieldActorSpinTask *data = work;
    FieldActor *actor = data->actor;
    u16 acmd;

    acmd = func_ov021_021c72f8((f32)data->frame, (f32)data->duration, (f32)data->turns);
    FldAct_SetAcmd(actor, acmd);
    data->frame += 1;
    if (data->duration < data->frame) {
        switch (data->dir) {
        case 0:
        default:
            acmd = 0;
            break;
        case 1:
            acmd = 1;
            break;
        case 2:
            acmd = 2;
            break;
        case 3:
            acmd = 3;
            break;
        }
        FldAct_SetAcmd(actor, acmd);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021c748c(void *work) {
    FieldActorSpinTask *data = work;
    FieldActor *actor = data->actor;
    u16 acmd;

    acmd = func_ov021_021c734c((f32)data->frame, (f32)data->duration, (f32)data->turns);
    FldAct_SetAcmd(actor, acmd);
    data->frame += 1;
    if (data->duration < data->frame) {
        switch (data->dir) {
        case 0:
        default:
            acmd = 0;
            break;
        case 1:
            acmd = 1;
            break;
        case 2:
            acmd = 2;
            break;
        case 3:
            acmd = 3;
            break;
        }
        FldAct_SetAcmd(actor, acmd);
        return TRUE;
    }
    return FALSE;
}

BOOL FieldActorSpinTask_CallbackDeaccel(void *work) {
    FieldActorSpinTask *data = work;
    FieldActor *actor = data->actor;
    u16 acmd;

    acmd = FieldActorSpinTask_CalcSpinDirAcmdSqrt((f32)data->frame, (f32)data->duration, (f32)data->turns);
    FldAct_SetAcmd(actor, acmd);
    data->frame += 1;
    if (data->duration < data->frame) {
        CheckSetActorFaceDir(actor, data->dir);
        return TRUE;
    }
    return FALSE;
}

FieldTask *func_ov021_021c754c(Field *field, u32 duration, VecFx32 *pos) {
    FieldTask *task;
    FieldPlayerMoveTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldPlayerMoveTask), FieldPlayerMoveTask_Callback);
    data = FieldTask_GetData(task);
    data->state = 0;
    data->field = field;
    data->frame = 0;
    data->duration = duration;
    VEC_Set(&data->end, pos->x, pos->y, pos->z);
    return task;
}

BOOL FieldPlayerMoveTask_Callback(void *work) {
    FieldPlayerMoveTask *data = work;
    FieldPlayer *player;
    FieldActor *actor;
    VecFx32 a;
    VecFx32 b;
    VecFx32 sum;
    s32 k;

    player = Field_GetPlayer(data->field);
    actor = FieldPlayer_GetActor(player);
    switch (data->state) {
    case 0:
        FieldPlayer_GetWPos(player, &data->start);
        data->state++;
        break;
    case 1:
        data->frame += 1;
        if ((f32)data->frame / (f32)data->duration > 0.0f) {
            k = (s32)(0.5f + 4096.0f * ((f32)data->frame / (f32)data->duration));
        } else {
            k = (s32)(4096.0f * ((f32)data->frame / (f32)data->duration) - 0.5f);
        }
        vecfx_mul(&data->start, 0x1000 - k, &a);
        vecfx_mul(&data->end, k, &b);
        VEC_Add(&a, &b, &sum);
        FieldPlayer_SetWPos(player, &sum);
        CheckRecalcActorY(actor);
        if (data->duration <= data->frame) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

FieldTask *FieldActorFallTask_Create(Field *field, FieldActor *actor, u32 duration, s32 height) {
    FieldTask *task;
    FieldActorFallTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldActorFallTask), FieldActorFallTask_Callback);
    data = FieldTask_GetData(task);
    data->state = 0;
    data->field = field;
    data->frame = 0;
    data->duration = duration;
    data->height = height;
    data->actor = actor;
    return task;
}

void FieldActorFallTask_CalcFallPos(VecFx32 *param_1, f32 param_2, f32 param_3, f32 param_4) {
    f32 t;
    f32 x;
    s32 value;

    t = param_4 / (param_3 * param_3);
    t = t * param_2;
    t = param_2 * t;
    x = param_4 - t;
    param_1->x = 0;
    if (x > 0.0f) {
        value = (s32)(0.5f + 4096.0f * x);
    } else {
        value = (s32)(4096.0f * x - 0.5f);
    }
    param_1->y = value;
    param_1->z = 0;
}

BOOL FieldActorFallTask_Callback(void *work) {
    FieldActorFallTask *data = work;
    FieldActor *actor;
    FieldEffects *effects;
    VecFx32 offset;

    Field_GetPlayer(data->field);
    actor = data->actor;
    switch (data->state) {
    case 0:
        FieldActorFallTask_CalcFallPos(&offset, (f32)data->frame, (f32)data->duration, data->height);
        SetActorWPosOffset(actor, &offset);
        GFL_SndSEPlay(0x681);
        data->state++;
        break;
    case 1:
        data->frame += 1;
        FieldActorFallTask_CalcFallPos(&offset, (f32)data->frame, (f32)data->duration, data->height);
        SetActorWPosOffset(actor, &offset);
        if (data->duration <= data->frame) {
            effects = Field_GetFieldEffects(data->field);
            func_ov021_021ac528(actor, effects);
            GFL_SndSEPlay(0x682);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

FieldTask *FieldWaitTask_Create(Field *field, u16 wait) {
    FieldTask *task;
    FieldWaitTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldWaitTask), func_ov021_021c77fc);
    data = FieldTask_GetData(task);
    data->frame = 0;
    data->wait = wait;
    return task;
}

BOOL func_ov021_021c77fc(void *work) {
    FieldWaitTask *data = work;

    data->frame = data->frame + 1;
    if (data->wait <= data->frame) {
        return TRUE;
    }
    return FALSE;
}

FieldTask *FieldFadeTask_Create(Field *field, u32 mode, u32 from, u32 to, u32 speed) {
    FieldTask *task;
    FieldFadeTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldFadeTask), FieldFadeTask_Callback);
    data = FieldTask_GetData(task);
    data->field = field;
    data->mode = mode;
    data->from = from;
    data->to = to;
    data->speed = speed;
    return task;
}

BOOL FieldFadeTask_Callback(void *work) {
    FieldFadeTask *data = work;

    GFL_FadeSet(data->mode, data->from, data->to, data->speed);
    return TRUE;
}

FieldTask *FieldActorMoveTask_CreatePlayer(Field *field, u32 duration, const VecFx32 *offset) {
    FieldPlayer *player;
    FieldActor *actor;
    FieldTask *task;

    player = Field_GetPlayer(field);
    actor = FieldPlayer_GetActor(player);
    task = FieldActorMoveTask_CreateRel(field, duration, offset, actor);
    return task;
}

FieldTask *FieldActorMoveTask_CreateRel(Field *field, s32 duration, const VecFx32 *offset, FieldActor *actor) {
    FieldTask *task;
    FieldActorMoveTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldActorMoveTask), func_ov021_021c7a20);
    data = FieldTask_GetData(task);
    data->state = 0;
    data->field = field;
    data->actor = actor;
    CopyActorWPos(actor, &data->start);
    if (duration >= 0) {
        data->direction = 0;
        data->frame = 0;
        data->duration = duration;
    } else {
        data->direction = 1;
        data->frame = 0;
        data->duration = -duration;
    }
    VEC_Set(&data->offset, offset->x, offset->y, offset->z);
    return task;
}

FieldTask *FieldActorMoveTask_CreateAbs(Field *field, s32 duration, const VecFx32 *offset, FieldActor *actor) {
    FieldTask *task;
    FieldActorMoveTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldActorMoveTask), func_ov021_021c7a7c);
    data = FieldTask_GetData(task);
    data->state = 0;
    data->field = field;
    data->actor = actor;
    CopyActorWPos(actor, &data->start);
    if (duration >= 0) {
        data->direction = 0;
        data->frame = 0;
        data->duration = duration;
    } else {
        data->direction = 1;
        data->frame = 0;
        data->duration = -duration;
    }
    VEC_Set(&data->offset, offset->x, offset->y, offset->z);
    return task;
}
void FieldActorMoveTask_InterpVector(VecFx32 *param_1, u16 param_2, VecFx32 *param_3, u16 param_4) {
    s32 k;

    k = (s32)(((f32)param_2 / (f32)param_4 > 0.0f) ? 0.5f + 4096.0f * ((f32)param_2 / (f32)param_4) : 4096.0f * ((f32)param_2 / (f32)param_4) - 0.5f);
    param_1->x = (fx32)(((u64)param_3->x * k + 0x800LL) >> 12);
    param_1->y = (fx32)(((u64)param_3->y * k + 0x800LL) >> 12);
    param_1->z = (fx32)(((u64)param_3->z * k + 0x800LL) >> 12);
}

BOOL func_ov021_021c7a20(void *work) {
    FieldActorMoveTask *data = work;
    FieldActor *actor = data->actor;
    VecFx32 pos;

    if (data->state == 0) {
        data->frame += 1;
        if (data->direction == 0) {
            FieldActorMoveTask_InterpVector(&pos, data->frame, &data->offset, data->duration);
        } else if (data->direction == 1) {
            FieldActorMoveTask_InterpVector(&pos, (u16)(data->duration - data->frame), &data->offset, data->duration);
        }
        SetActorWPosOffset(actor, &pos);
        if (data->duration <= data->frame) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov021_021c7a7c(void *work) {
    FieldActorMoveTask *data = work;
    FieldActor *actor = data->actor;
    VecFx32 pos;

    if (data->state == 0) {
        data->frame += 1;
        if (data->direction == 0) {
            FieldActorMoveTask_InterpVector(&pos, data->frame, &data->offset, data->duration);
        } else if (data->direction == 1) {
            FieldActorMoveTask_InterpVector(&pos, (u16)(data->duration - data->frame), &data->offset, data->duration);
        }
        VEC_Add(&pos, &data->start, &pos);
        SetActorWPosValue(actor, &pos);
        if (data->duration <= data->frame) {
            return TRUE;
        }
    }
    return FALSE;
}

FieldTask *FieldCameraMoveTaskTargetOffs_Create(Field *field, u32 duration, const VecFx32 *offset) {
    FieldTask *task;
    FieldCameraTargetOffsTask *data;

    task = FieldTask_Create(Field_GetHeapID(field), sizeof(FieldCameraTargetOffsTask),
                            FieldCameraTargetMoveTaskTargetOffs_Callback);
    data = FieldTask_GetData(task);
    data->state = 0;
    data->field = field;
    data->frame = 0;
    data->duration = duration;
    VEC_Set(&data->end, offset->x, offset->y, offset->z);
    return task;
}

BOOL FieldCameraTargetMoveTaskTargetOffs_Callback(void *work) {
    FieldCameraTargetOffsTask *data = work;
    FieldCamera *camera;
    VecFx32 a;
    VecFx32 b;
    VecFx32 sum;
    s32 k;

    camera = Field_GetCameraSystem(data->field);
    switch (data->state) {
    case 0:
        FieldCamera_CoordsGetTargetOffset(camera, &data->start);
        data->state++;
        break;
    case 1:
        data->frame += 1;
        if ((f32)data->frame / (f32)data->duration > 0.0f) {
            k = (s32)(0.5f + 4096.0f * ((f32)data->frame / (f32)data->duration));
        } else {
            k = (s32)(4096.0f * ((f32)data->frame / (f32)data->duration) - 0.5f);
        }
        vecfx_mul(&data->start, 0x1000 - k, &a);
        vecfx_mul(&data->end, k, &b);
        VEC_Add(&a, &b, &sum);
        FieldCamera_CoordsSetTargetOffset(camera, &sum);
        if (data->duration <= data->frame) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

FieldTask *FieldCameraMoveTaskTarget_Create(Field *field, u16 duration, const VecFx32 *from, const VecFx32 *to) {
    HeapID heapID;
    FieldTask *task;
    FieldCameraTargetTask *data;

    heapID = Field_GetHeapID(field);
    Field_GetCameraSystem(field);
    task = FieldTask_Create(heapID, sizeof(FieldCameraTargetTask), FieldCameraMoveTaskTarget_Callback);
    data = FieldTask_GetData(task);
    data->state = 0;
    data->field = field;
    data->frame = 0;
    data->duration = duration;
    data->from = *from;
    data->to = *to;
    VEC_Subtract(&data->to, &data->from, &data->delta);
    return task;
}

BOOL FieldCameraMoveTaskTarget_Callback(void *work) {
    FieldCameraTargetTask *data = work;
    FieldCamera *camera;
    VecFx32 dest;
    f32 t;
    s32 k;

    camera = Field_GetCameraSystem(data->field);
    switch (data->state) {
    case 0:
        data->transformType = FieldCamera_GetTransformType(camera);
        FieldCamera_ChangeTransformType(camera, 0);
        data->state++;
        break;
    case 1:
        data->frame += 1;
        t = (f32)data->frame / (f32)data->duration;
        if (t > 0.0f) {
            k = (s32)(0.5f + 4096.0f * t);
        } else {
            k = (s32)(4096.0f * t - 0.5f);
        }
        VEC_MultAdd(k, &data->delta, &data->from, &dest);
        FieldCamera_CoordsSetTarget(camera, &dest);
        if (data->duration <= data->frame) {
            FieldCamera_ChangeTransformType(camera, data->transformType);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// actor goes unused: the task always follows the player's actor
FieldTask *FieldActorEffectTask_Create(Field *field, FieldActor *actor, u16 duration, u16 interval) {
    HeapID heapID;
    FieldPlayer *player;
    FieldTask *task;
    FieldActorEffectTask *data;

    heapID = Field_GetHeapID(field);
    player = Field_GetPlayer(field);
    task = FieldTask_Create(heapID, sizeof(FieldActorEffectTask), FieldActorEffectTask_Callback);
    data = FieldTask_GetData(task);
    data->field = field;
    data->effects = Field_GetFieldEffects(field);
    data->actor = FieldPlayer_GetActor(player);
    data->state = 0;
    data->frame = 0;
    data->duration = duration;
    data->interval = interval;
    return task;
}

BOOL FieldActorEffectTask_Callback(void *work) {
    FieldActorEffectTask *data = work;
    u16 frame;

    if (data->state == 0) {
        if (data->frame % data->interval == 0) {
            func_ov021_021add7c(data->actor, data->effects);
        }
        frame = data->frame;
        data->frame = frame + 1;
        if (data->duration <= frame) {
            return TRUE;
        }
    }
    return FALSE;
}
