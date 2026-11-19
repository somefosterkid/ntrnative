#include "global.h"

#include "field_system.h"
#include "overlay_01_021F1AFC.h"
#include "player_avatar.h"
#include "task.h"

BOOL ov01_021F2DD0(TaskManager *task);

void *ov01_021F30D0(u32 size);

void ov01_021F2D68(FieldSystem *fieldSystem, int direction, const MonRideTask *monRideTask);

void ov01_021F2D68(FieldSystem *fieldSystem, int direction, const MonRideTask *monRideTask) {
    WhirlpoolTaskEnv *taskEnv = ov01_021F30D0(sizeof(WhirlpoolTaskEnv));

    taskEnv->direction = direction;
    taskEnv->fieldSystem = fieldSystem;
    taskEnv->playerAvatar = fieldSystem->playerAvatar;
    taskEnv->playerObject = PlayerAvatar_GetMapObject(taskEnv->playerAvatar);
    taskEnv->monRideTask = *monRideTask;

    TaskManager_Call(fieldSystem->taskman, ov01_021F2DD0, taskEnv);
}
