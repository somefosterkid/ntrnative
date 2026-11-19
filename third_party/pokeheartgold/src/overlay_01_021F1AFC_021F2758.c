#include "global.h"

#include "field_system.h"
#include "follow_mon.h"
#include "map_object.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "unk_02005D10.h"
#include "unk_0205FD20.h"
#include "unk_02062108.h"

OverworldAnimManager *ov01_021FEAB0(LocalMapObject *, int, int, int, int);

s32 ov01_021F2758(RockClimbTaskEnv *taskEnv);

u32 sub_0206234C(s32, s32);
s32 ov01_021F27C0(RockClimbTaskEnv *taskEnv);
void ov01_021FEB30(OverworldAnimManager *, int);
s32 ov01_021F27E8(RockClimbTaskEnv *taskEnv);
s32 ov01_021F2808(RockClimbTaskEnv *taskEnv);

s32 ov01_021F2758(RockClimbTaskEnv *taskEnv) {
    u32 uVar1;
    s32 iVar2;
    u32 uVar3;
    s32 iVar4;
    BOOL BVar6;
    LocalMapObject *object;

    uVar1 = PlayerAvatar_GetXCoord(taskEnv->playerAvatar);
    iVar2 = GetDeltaXByFacingDirection(taskEnv->direction);
    uVar3 = PlayerAvatar_GetZCoord(taskEnv->playerAvatar);
    iVar4 = GetDeltaYByFacingDirection(taskEnv->direction);
    taskEnv->animManager = ov01_021FEAB0(taskEnv->playerObject, uVar1 + iVar2, uVar3 + iVar4, taskEnv->direction, 0);
    BVar6 = FollowMon_IsActive(taskEnv->fieldSystem);
    if (BVar6 != 0) {
        object = FollowMon_GetMapObject(taskEnv->fieldSystem);
        MapObject_UnpauseMovement(object);
    }
    taskEnv->field3dTask = ov02_0224D9A4(taskEnv->fieldSystem);
    PlaySE(0x626);
    taskEnv->state = taskEnv->state + 1;
    return 0;
}

s32 ov01_021F27C0(RockClimbTaskEnv *taskEnv) {
    if (MapObject_AreBitsSetForMovementScriptInit(taskEnv->playerObject) == 1) {
        MapObject_SetHeldMovement(taskEnv->playerObject, sub_0206234C(taskEnv->direction, 0x34));
        taskEnv->state = (u16)(taskEnv->state + 1);
    }
    return 0;
}

s32 ov01_021F27E8(RockClimbTaskEnv *taskEnv) {
    if (MapObject_IsMovementPaused(taskEnv->playerObject) == 1) {
        taskEnv->state = (u16)(taskEnv->state + 1);
        ov01_021FEB30(taskEnv->animManager, 1);
    }
    return 0;
}

s32 ov01_021F2808(RockClimbTaskEnv *taskEnv) {
    if (MapObject_AreBitsSetForMovementScriptInit(taskEnv->playerObject) == 1) {
        MapObject_SetHeldMovement(taskEnv->playerObject, sub_0206234C(taskEnv->direction, 0x10));
        taskEnv->state = (u16)(taskEnv->state + 1);
    }
    return 0;
}
