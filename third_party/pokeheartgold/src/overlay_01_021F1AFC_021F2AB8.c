#include "global.h"

#include "error_handling.h"
#include "map_object.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_02.h"
#include "unk_0205FD20.h"

void ov01_021F3054(FieldSystem *fieldSystem, MonRideTask *monRideTask);
s32 ov01_021F2B80(WaterfallTaskEnv *taskEnv);

int ov01_021F2AB8(WaterfallTaskEnv *taskEnv);
int ov01_021F2AF8(WaterfallTaskEnv *taskEnv);

int ov01_021F2AB8(WaterfallTaskEnv *taskEnv)
{
    VecFx32 newPos;

    MapObject_CopyPositionVector(taskEnv->playerObject, &newPos);

    newPos.y += taskEnv->deltaPos.y;

    if (newPos.y > taskEnv->targetPosition.y) {
        newPos.y = taskEnv->targetPosition.y;
    }

    MapObject_SetPositionVector(taskEnv->playerObject, &newPos);

    taskEnv->movementCounter++;

    if (taskEnv->movementCounter >= 32) {
        taskEnv->movementCounter = 0;
        taskEnv->state++;
    }

    return 0;
}

int ov01_021F2AF8(WaterfallTaskEnv *taskEnv)
{
    VecFx32 newPos;

    MapObject_CopyPositionVector(taskEnv->playerObject, &newPos);

    newPos.y += taskEnv->deltaPos.y;

    if (newPos.y > taskEnv->targetPosition.y) {
        newPos.y = taskEnv->targetPosition.y;
    }

    newPos.z += taskEnv->deltaPos.z;

    if (newPos.z < taskEnv->targetPosition.z) {
        newPos.z = taskEnv->targetPosition.z;
    }

    MapObject_SetPositionVector(taskEnv->playerObject, &newPos);

    taskEnv->movementCounter++;

    if (taskEnv->movementCounter < 64) {
        return 0;
    }

    ((newPos.z == taskEnv->targetPosition.z) ? (void)0 : GF_AssertFail()) ;
    ((newPos.y == taskEnv->targetPosition.y) ? (void)0 : GF_AssertFail()) ;

    MapObject_SetCurrentX(taskEnv->playerObject, taskEnv->targetX);
    MapObject_SetCurrentY(taskEnv->playerObject, taskEnv->targetY);
    MapObject_SetCurrentZ(taskEnv->playerObject, taskEnv->targetZ);
    sub_02060F78(taskEnv->playerObject);

    ov02_0224D5AC(taskEnv->field3dTask);
    return 1;
}

s32 ov01_021F2B80(WaterfallTaskEnv *taskEnv) {
    if (taskEnv->monRideTask.playCutIn == 1) {
        ov01_021F3054(taskEnv->fieldSystem, &taskEnv->monRideTask);
        taskEnv->state = taskEnv->state + 1;
        return 0;
    }
    taskEnv->state = 2;
    return 2;
}
