#include "global.h"

#include "field_system.h"
#include "overlay_01_021F1AFC.h"
#include "player_avatar.h"
#include "task.h"

BOOL ov01_021F2628(TaskManager *param0);

void *ov01_021F30D0(u32 size);

void ov01_021F2548(FieldSystem *fieldSystem, int direction, const MonRideTask *monRideTask);

void ov01_021F2548(FieldSystem *fieldSystem, int direction, const MonRideTask *monRideTask) {
    RockClimbTaskEnv *taskEnv = ov01_021F30D0(sizeof(RockClimbTaskEnv));
    taskEnv->direction = direction;
    taskEnv->fieldSystem = fieldSystem;
    taskEnv->playerAvatar = fieldSystem->playerAvatar;
    taskEnv->playerObject = PlayerAvatar_GetMapObject(taskEnv->playerAvatar);
    taskEnv->monRideTask = *monRideTask;
    taskEnv->nextState = (taskEnv->monRideTask.playCutIn == 1) ? 5 : 6;
    TaskManager_Call(fieldSystem->taskman, ov01_021F2628, taskEnv);
}
