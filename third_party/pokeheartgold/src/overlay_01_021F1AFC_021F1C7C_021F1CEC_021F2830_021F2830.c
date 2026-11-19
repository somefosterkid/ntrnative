#include "global.h"

#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_02.h"
#include "unk_02062108.h"

void ov01_021FEB30(OverworldAnimManager *, int);

u32 sub_02060FA8(LocalMapObject *, u32);

u32 sub_0206234C(s32, s32);

s32 ov01_021F2830(RockClimbTaskEnv *taskEnv);

s32 ov01_021F2830(RockClimbTaskEnv *taskEnv) {
    BOOL BVar1;
    u32 facingDirection;
    u32 uVar2;
    u32 a1;

    BVar1 = MapObject_IsMovementPaused(taskEnv->playerObject);
    if (BVar1 == 0) {
        return 0;
    }
    facingDirection = MapObject_GetNextFacingDirection(taskEnv->playerObject);
    uVar2 = sub_02060FA8(taskEnv->playerObject, facingDirection);
    BVar1 = MetatileBehavior_IsRockClimbInDirection(uVar2 & 0xff, facingDirection);
    if (BVar1 == 1) {
        taskEnv->state = taskEnv->nextState;
        return 2;
    }
    a1 = sub_0206234C(taskEnv->direction, 0x34);
    MapObject_SetHeldMovement(taskEnv->playerObject, a1);
    taskEnv->dummy = 0;
    taskEnv->state = taskEnv->state + 1;
    ov01_021FEB30(taskEnv->animManager, 0);
    ov02_0224D9B8(taskEnv->field3dTask);
    taskEnv->field3dTask = NULL;
    return 0;
}
