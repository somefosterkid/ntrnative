#include "global.h"

#include "error_handling.h"
#include "map_object.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_02.h"
#include "unk_0205FD20.h"

s32 sub_02054790(FieldSystem *, s32, s32, s32, s32, u8 *);

s32 ov01_021F29E4(WaterfallTaskEnv *taskEnv);

s32 ov01_021F29E4(WaterfallTaskEnv *taskEnv) {
    u8 sp8;
    VecFx32 sp18;
    VecFx32 delta;
    s32 temp_r1;
    s32 temp_r4_2;
    s32 temp_r6_2;
    u32 temp_r4;
    u32 temp_r6;
    temp_r4 = MapObject_GetXCoord(taskEnv->playerObject);
    temp_r4_2 = temp_r4 + (GetDeltaXByFacingDirection(0) * 2);
    temp_r6 = MapObject_GetZCoord(taskEnv->playerObject);
    temp_r6_2 = temp_r6 + (GetDeltaYByFacingDirection(0) * 2);
    sub_020611C8(temp_r4_2, temp_r6_2, &taskEnv->targetPosition);
    taskEnv->targetPosition.y = sub_02054790(taskEnv->fieldSystem, 1, taskEnv->targetPosition.y, taskEnv->targetPosition.x, taskEnv->targetPosition.z, &sp8);
    if ((*(&sp8)) == 0) {
        GF_AssertFail();
    }
    taskEnv->targetX = temp_r4_2;
    temp_r1 = ((s32)taskEnv->targetPosition.y) >> 3;
    taskEnv->targetY = temp_r1 / 0x1000;
    taskEnv->targetZ = temp_r6_2;
    MapObject_CopyPositionVector(taskEnv->playerObject, &sp18);
    if (sp18.z <= ((s32)taskEnv->targetPosition.z)) {
        GF_AssertFail();
    }
    if (sp18.y >= ((s32)taskEnv->targetPosition.y)) {
        GF_AssertFail();
    }
    delta.x = 0;
    delta.y = taskEnv->targetPosition.y - sp18.y;
    delta.z = taskEnv->targetPosition.z - sp18.z;
    delta.y /= 64;
    delta.z /= 64;
    taskEnv->deltaPos = delta;
    taskEnv->field3dTask = ov02_0224D598(taskEnv->fieldSystem);
    taskEnv->state += 1;
    return 0;
}
