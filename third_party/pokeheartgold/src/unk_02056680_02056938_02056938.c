#include "global.h"

#include "constants/heap.h"

#include "field/fieldmap.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_021F1AFC.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "task.h"
#include "unk_02055BF0.h"
#include "unk_02062108.h"

BOOL sub_02056938(TaskManager *man);

BOOL sub_02056938(TaskManager *man) {
    FieldSystem *fieldSystem;
    FieldTransitionEnvironment *env;
    FieldEnvSubUnk18 *newUnk18;
    LocalMapObject *pLVar3;
    BOOL BVar4;
    FieldEnvSubUnk18 *unk18;
    VecFx32 VStack_20;

    fieldSystem = TaskManager_GetFieldSystem(man);
    env = TaskManager_GetEnvironment(man);
    switch (env->transitionState) {
    case 0:
        newUnk18 = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(FieldEnvSubUnk18));
        env->unk18 = newUnk18;
        newUnk18->state = 0;
        FieldMap_FadeScreen(1);
        env->transitionState = env->transitionState + 1;
        break;
    case 1:
        pLVar3 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        unk18 = env->unk18;
        unk18->state = unk18->state + 1;
        MapObject_CopyPositionVector(pLVar3, &VStack_20);
        VStack_20.y = VStack_20.y + -0x2000;
        MapObject_SetPositionVector(pLVar3, &VStack_20);
        if (unk18->state >= 0x10) {
            PlayerAvatar_ToggleAutomaticHeightUpdatingImmediate(fieldSystem->playerAvatar, 1);
            env->transitionState = env->transitionState + 1;
        }
        break;
    case 2:
        Field_PlayerAvatar_OrrTransitionFlags(fieldSystem->playerAvatar, 1);
        Field_PlayerAvatar_ApplyTransitionFlags(fieldSystem->playerAvatar);
        env->transitionState = env->transitionState + 1;
        break;
    case 3:
        pLVar3 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        BVar4 = MapObject_AreBitsSetForMovementScriptInit(pLVar3);
        if (BVar4 != 0) {
            MapObject_SetHeldMovement(pLVar3, 0xd);
            env->transitionState = env->transitionState + 1;
        }
        break;
    case 4:
        pLVar3 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        BVar4 = MapObject_AreBitsSetForMovementScriptInit(pLVar3);
        if ((BVar4 != 0) && (BVar4 = IsPaletteFadeFinished(), BVar4 != 0)) {
            BVar4 = FollowMon_IsActive(fieldSystem);
            if (BVar4 != 0) {
                ov01_02205790(fieldSystem, 1);
                pLVar3 = FollowMon_GetMapObject(fieldSystem);
                sub_0205FC94(pLVar3, 0x30);
                pLVar3 = FollowMon_GetMapObject(fieldSystem);
                sub_02069DC8(pLVar3, 1);
            }
            env->transitionState = env->transitionState + 1;
        }
        break;
    case 5:
        Heap_Free(env->unk18);
        return 1;
    }
    return 0;
}
