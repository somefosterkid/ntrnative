#include "global.h"

#include "error_handling.h"
#include "map_object.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_02.h"
#include "unk_0205FD20.h"

s32 ov01_021F2C88(WaterfallTaskEnv *taskEnv);

s32 ov01_021F2C88(WaterfallTaskEnv *taskEnv) {
    VecFx32 positionVector;
    s32 v0;
    s32 v1;
    s32 v2;

    MapObject_CopyPositionVector(taskEnv->playerObject, &positionVector);
    v2 = positionVector.z + taskEnv->deltaPos.z;
    positionVector.z = v2;
    v0 = taskEnv->targetPosition.z;
    if (v2 > v0) {
        positionVector.z = v0;
    } else {
        taskEnv->distanceMoved = (s32)(taskEnv->distanceMoved + taskEnv->deltaPos.z);
    }
    MapObject_SetPositionVector(taskEnv->playerObject, &positionVector);
    v1 = taskEnv->movementCounter + 1;
    taskEnv->movementCounter = v1;
    if (v1 >= 0x20) {
        taskEnv->movementCounter = 0;
        taskEnv->state = (s32)(taskEnv->state + 1);
    }
    return 0;
}

s32 ov01_021F2CD4(WaterfallTaskEnv *taskEnv);

s32 ov01_021F2CD4(WaterfallTaskEnv *taskEnv) {
    VecFx32 positionVector;
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v4;

    MapObject_CopyPositionVector(taskEnv->playerObject, &positionVector);
    v3 = positionVector.y + taskEnv->deltaPos.y;
    positionVector.y = v3;
    v0 = taskEnv->targetPosition.y;
    if (v3 < v0) {
        positionVector.y = v0;
    }
    v4 = positionVector.z + taskEnv->deltaPos.z;
    positionVector.z = v4;
    v1 = taskEnv->targetPosition.z;
    if (v4 > v1) {
        positionVector.z = v1;
    } else {
        taskEnv->distanceMoved = (s32)(taskEnv->distanceMoved + taskEnv->deltaPos.z);
    }
    MapObject_SetPositionVector(taskEnv->playerObject, &positionVector);
    v2 = taskEnv->movementCounter + 1;
    taskEnv->movementCounter = v2;
    if (v2 < 0x40) {
        return 0;
    }
    if (positionVector.z != taskEnv->targetPosition.z) {
        GF_AssertFail();
    }
    if (positionVector.y != taskEnv->targetPosition.y) {
        GF_AssertFail();
    }
    MapObject_SetCurrentX(taskEnv->playerObject, taskEnv->targetX);
    MapObject_SetCurrentY(taskEnv->playerObject, taskEnv->targetY);
    MapObject_SetCurrentZ(taskEnv->playerObject, taskEnv->targetZ);
    sub_02060F78(taskEnv->playerObject);
    ov02_0224D5AC(taskEnv->field3dTask);
    return 1;
}
