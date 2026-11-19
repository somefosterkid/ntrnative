#include "global.h"

#include "error_handling.h"
#include "map_object.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_02.h"
#include "unk_0205FD20.h"
#include "unk_02005D10.h"

BOOL ov01_021F3068(MonRideTask *monRideTask);

s32 ov01_021F2BA4(WaterfallTaskEnv *taskEnv);
int ov01_021F2BC8(WaterfallTaskEnv *taskEnv);

s32 ov01_021F2BA4(WaterfallTaskEnv *taskEnv) {
    s32 iVar1;

    iVar1 = ov01_021F3068(&taskEnv->monRideTask);
    if (iVar1 == 1) {
        PlaySE(0x64c);
        taskEnv->state = taskEnv->state + 1;
    }
    return 0;
}

int ov01_021F2BC8(WaterfallTaskEnv *taskEnv)
{
    int targetX, targetZ;
    VecFx32 playerPos, deltaPos;

    targetX = MapObject_GetXCoord(taskEnv->playerObject) + (GetDeltaXByFacingDirection(1 ) << 1);
    targetZ = MapObject_GetZCoord(taskEnv->playerObject) + (GetDeltaYByFacingDirection(1 ) << 1);

    sub_020611C8(targetX, targetZ, &taskEnv->targetPosition);
    sub_0206121C(taskEnv->fieldSystem, &taskEnv->targetPosition);

    taskEnv->distanceMoved = 0;
    taskEnv->targetX = targetX;
    taskEnv->targetY = (((taskEnv->targetPosition.y) >> 3) / ((fx32) 0x0000000000001000L) );
    taskEnv->targetZ = targetZ;

    MapObject_CopyPositionVector(taskEnv->playerObject, &playerPos);

    ((playerPos.z < taskEnv->targetPosition.z) ? (void)0 : GF_AssertFail()) ;
    ((playerPos.y > taskEnv->targetPosition.y) ? (void)0 : GF_AssertFail()) ;

    deltaPos.x = 0;
    deltaPos.y = taskEnv->targetPosition.y - playerPos.y;
    deltaPos.z = taskEnv->targetPosition.z - playerPos.z;
    deltaPos.y /= 64;
    deltaPos.z /= 64;

    taskEnv->deltaPos = deltaPos;
    taskEnv->field3dTask = ov02_0224D598(taskEnv->fieldSystem);
    taskEnv->state++;

    return 0;
}
