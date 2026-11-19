#include "global.h"

#include "field_system.h"
#include "mon_ride_task.h"
#include "overlay_01_021F1AFC.h"
#include "player_avatar.h"
#include "task.h"

BOOL ov01_021F2118(TaskManager *task);

void *ov01_021F30D0(int size);

void ov01_021F202C(FieldSystem *fieldSystem, int direction, const MonRideTask *monRideTask);

void ov01_021F202C(FieldSystem *fieldSystem, int direction, const MonRideTask *monRideTask) {
    SurfTaskEnv *taskEnv = ov01_021F30D0(sizeof(SurfTaskEnv));

    taskEnv->direction = direction;
    taskEnv->fieldSystem = fieldSystem;
    taskEnv->playerAvatar = fieldSystem->playerAvatar;
    taskEnv->playerObject = PlayerAvatar_GetMapObject(taskEnv->playerAvatar);
    taskEnv->monRideTask = *monRideTask;

    TaskManager_Call(fieldSystem->taskman, ov01_021F2118, taskEnv);
}
