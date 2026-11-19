#include "global.h"

#include "field_system.h"
#include "follow_mon.h"
#include "map_object.h"
#include "unk_02005D10.h"
#include "metatile_behavior.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "unk_02062108.h"

BOOL ov01_021F3068(MonRideTask *monRideTask);

s32 ov01_021F2E20(WhirlpoolTaskEnv *taskEnv);

u32 sub_0206234C(s32, s32);
s32 ov01_021F2E38(WhirlpoolTaskEnv *taskEnv);
s32 ov01_021F2E6C(WhirlpoolTaskEnv *taskEnv);
s32 ov01_021F2E94(WhirlpoolTaskEnv *taskEnv);
void ov01_021F2EDC(PlayerAvatar *arg0);

s32 ov01_021F2E20(WhirlpoolTaskEnv *taskEnv) {
    s32 iVar1;

    iVar1 = ov01_021F3068(&taskEnv->monRideTask);
    if (iVar1 == 1) {
        taskEnv->state = taskEnv->state + 1;
    }
    return 0;
}

s32 ov01_021F2E38(WhirlpoolTaskEnv *taskEnv) {
    BOOL BVar1;
    LocalMapObject *object;
    Field3dObjectTask *iVar2;

    BVar1 = FollowMon_IsActive(taskEnv->fieldSystem);
    if (BVar1 != 0) {
        object = FollowMon_GetMapObject(taskEnv->fieldSystem);
        MapObject_UnpauseMovement(object);
    }
    iVar2 = ov02_0224D67C(taskEnv->fieldSystem);
    taskEnv->field3dTask = iVar2;
    PlaySE(0x64c);
    taskEnv->state = taskEnv->state + 1;
    return 0;
}

s32 ov01_021F2E6C(WhirlpoolTaskEnv *taskEnv) {
    if (MapObject_AreBitsSetForMovementScriptInit(taskEnv->playerObject) == 1) {
        MapObject_SetHeldMovement(taskEnv->playerObject, sub_0206234C(taskEnv->direction, 4));
        taskEnv->state = (s32)(taskEnv->state + 1);
    }
    return 0;
}

s32 ov01_021F2E94(WhirlpoolTaskEnv *taskEnv) {
    if (MapObject_IsMovementPaused(taskEnv->playerObject) == 0) {
        return 0;
    }
    MapObject_GetNextFacingDirection(taskEnv->playerObject);
    if (MetatileBehavior_IsWhirlpool((u8)sub_0205F504(taskEnv->playerObject)) == 1) {
        taskEnv->state = 3;
        return 2;
    }
    ov02_0224D690(taskEnv->field3dTask);
    taskEnv->field3dTask = NULL;
    MapObject_ClearHeldMovementIfActive(taskEnv->playerObject);
    return 1;
}

void ov01_021F2EDC(PlayerAvatar *arg0) {
    LocalMapObject *object;
    u32 flags;

    object = PlayerAvatar_GetMapObject(arg0);
    if ((sub_0205F73C(object) != 0) && (PlayerAvatar_GetState(arg0) == 0) && ((u32)(MapObject_GetSpriteID(object) - 0xC4) > 1U)) {
        flags = PlayerAvatar_GetTransitionFlags(arg0);
        Field_PlayerAvatar_OrrTransitionFlags(arg0, 0x40);
        Field_PlayerAvatar_ApplyTransitionFlags(arg0);
        Field_PlayerAvatar_OrrTransitionFlags(arg0, (s32)flags);
    }
}
