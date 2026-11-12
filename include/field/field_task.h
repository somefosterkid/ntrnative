// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_task.h, the lines in use so far.
#ifndef BWPORT_FIELD_FIELD_TASK_H
#define BWPORT_FIELD_FIELD_TASK_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// A task that the field runs each frame until its callback returns TRUE
typedef struct FieldTask FieldTask;

// Runs on the task's data each frame until it returns TRUE
typedef BOOL (*FieldTaskCallback)(void *data);

// pokebw2's (src/ov036/field_task.c)
struct FieldTask {
    u32 state;
    void *data;
    FieldTaskCallback callback;
};

void FieldTaskManager_AddTask(FieldTaskManager *mgr, FieldTask *task, FieldTask *parent);
FieldTask *FieldActorSpinTask_CreatePlayerAccel(Field *field, u32 duration, u32 direction);
FieldTask *FieldActorSpinTask_CreatePlayer(Field *field, u32 duration, u32 direction);
FieldTask *FieldActorMoveTask_CreatePlayer(Field *field, u32 duration, const VecFx32 *offset);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
FieldTask *FieldActorSpinTask_Create(Field *field, u32 duration, u32 direction, FieldActor *actor);
// Moves the actor by offset, or to pos, over duration frames
FieldTask *FieldActorMoveTask_CreateRel(Field *field, s32 duration, const VecFx32 *offset, FieldActor *actor);
FieldTask *FieldActorMoveTask_CreateAbs(Field *field, s32 duration, const VecFx32 *pos, FieldActor *actor);
// Drops the actor from height onto the ground over duration frames
FieldTask *FieldActorFallTask_Create(Field *field, FieldActor *actor, u32 duration, s32 height);
// Tasks that move the camera to a zoom, angle or target offset over duration frames
FieldTask *FieldCameraMoveTaskZoom_Create(Field *field, u16 duration, fx32 zoom);
FieldTask *FieldCameraMoveTaskPitch_Callback(Field *field, u32 duration, u16 pitch);
FieldTask *FieldCameraMoveTaskYaw_Callback(Field *field, u32 duration, u16 yaw);
FieldTask *FieldCameraMoveTaskTargetOffs_Create(Field *field, u32 duration, const VecFx32 *offset);

#endif // BWPORT_FIELD_FIELD_TASK_H
