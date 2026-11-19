#include "global.h"

#include "field/fieldmap.h"

#include "error_handling.h"
#include "follow_mon.h"
#include "overlay_01.h"
#include "overlay_01_021E90C0.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "task.h"
#include "unk_02055BF0.h"
#include "unk_02062108.h"

s32 ov01_021E971C(FieldSystem *, FieldEnvSubUnk18 *, u8);

s32 sub_02056680(TaskManager *man);

s32 sub_02056680(TaskManager *man) {
    FieldEnvSubUnk18 *fieldEnvSubUnk18;
    FieldSystem *fieldSystem;
    u16 v1;
    u32 x;
    FieldTransitionEnvironment *env;

    fieldSystem = TaskManager_GetFieldSystem(man);
    env = TaskManager_GetEnvironment(man);
    v1 = env->transitionState;
    switch (v1) {
    case 0:
        env->unk18 = ov01_021E90C0();
        x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
        ov01_021E90DC(x, PlayerAvatar_GetZCoord(fieldSystem->playerAvatar), env->unk18);
        env->transitionState = (u16)(env->transitionState + 1);
    default:
        break;
    case 1:
        fieldEnvSubUnk18 = env->unk18;
        if (ov01_021E971C(fieldSystem, fieldEnvSubUnk18, (u8)PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar)) != 0) {
            ov01_021E90D4(env->unk18);
            env->transitionState = (u16)(env->transitionState + 1);
        }
        break;
    case 2:
        return 1;
    }
    return 0;
}

s32 sub_020566F8(TaskManager *man);

s32 sub_020566F8(TaskManager *man) {
    FieldSystem *fieldSystem;
    LocalMapObject *localMapObject;
    LocalMapObject *obj;
    u16 v1;
    u32 v2;
    FieldTransitionEnvironment *env;

    fieldSystem = TaskManager_GetFieldSystem(man);
    env = TaskManager_GetEnvironment(man);
    v1 = env->transitionState;
    switch (v1) {
    case 0:
        FieldMap_FadeScreen(1U);
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        v2 = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
        if (v2 == 2) {
            MapObject_SetHeldMovement(obj, 0xAU);
        } else if (v2 == 3) {
            MapObject_SetHeldMovement(obj, 0xBU);
        } else {
            GF_AssertFail();
        }
        env->transitionState = (u16)(env->transitionState + 1);
    default:
        break;
    case 1:
        localMapObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(localMapObject) == 1) {
            MapObject_ClearHeldMovementIfActive(localMapObject);
            if (FollowMon_IsActive(fieldSystem) != 0) {
                ov01_02205790(fieldSystem, (u8)PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar));
            }
            env->transitionState = (u16)(env->transitionState + 1);
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() != 0) {
            env->transitionState = (u16)(env->transitionState + 1);
        }
        break;
    case 3:
        return 1;
    }
    return 0;
}
