#include "global.h"

#include "constants/heap.h"

#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "task.h"
#include "unk_02062108.h"

s32 GetDoorSE(FieldSystem *, s32, s32);

s32 ov01_021E9610(Camera *, void *);

s32 sub_02054A60(s32, s32, s32, s32, s32, s32, s32 *);

s32 sub_02054AE4(FieldSystem *, u32, s32 *, MapProp **);

u32 *sub_02054D10(FieldSystem *, s32, s32, s32 *, s32);

typedef struct DoorEnv {
    u32 state;
    s32 unk4;
    s32 unk8;
    u8 unkC;
    u8 unkD;
    u16 angle;
    BOOL unk10;
} DoorEnv;

s32 ov01_021E9C40(TaskManager *taskman);

s32 ov01_021E9C40(TaskManager *taskman) {
    s32 pos[4];
    MapPropAnimListFile listFile;
    MapProp *mapProp;
    NNSG3dResTex *tex;
    LocalMapObject *playerObj;
    BOOL found;
    int modelId;
    NARC *narc;
    u32 *list;
    FieldSystem *fieldSystem;
    DoorEnv *env;
    int i;
    u8 count;
    NNSG3dRenderObj *renderObj;
    LocalMapObject *obj;

    fieldSystem = TaskManager_GetFieldSystem(taskman);
    env = TaskManager_GetEnvironment(taskman);
    switch (env->state) {
    case 0:
        playerObj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        env->unk10 = 0;
        env->unkD = 0;
        env->angle = Camera_GetPerspectiveAngle(fieldSystem->camera);
        Camera_AdjustPerspectiveAngle(0xFFA0, fieldSystem->camera);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 6, 1, HEAP_ID_FIELD2);
        env->unk10 = 1;
        if (PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar) == 1) {
            env->unkC = 1;
            found = FALSE;
            sub_02054A60(env->unk4, env->unk8, -1, found, 3, 1, pos);
            narc = ov01_021FB904(fieldSystem->areaDataManager);
            list = sub_02054D10(fieldSystem, 4, 4, pos, 0);
            for (i = 0; i < 4; i++) {
                if (list[i] != 0) {
                    NARC_ReadWholeMember(narc, list[i], &listFile);
                    if (*(u8 *)&listFile.unk4 != 0) {
                        sub_02054AE4(fieldSystem, list[i], pos, &mapProp);
                        found = TRUE;
                        modelId = list[i];
                        break;
                    }
                }
            }
            Heap_Free(list);
            if (found) {
                count = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, modelId);
                if (count == 0) {
                    return 0;
                }
                tex = ov01_021FB9E0(fieldSystem->areaDataManager);
                renderObj = MapProp_GetRenderSurface(mapProp);
                MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 1, modelId, renderObj, MapProp_GetResModel(mapProp), tex, count, 1, 0);
                env->state = 4;
                break;
            }
        }
        if (PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar) == 1) {
            MapObject_SetVisible(playerObj, 1);
            env->state = 1;
        } else {
            MapObject_SetVisible(playerObj, 0);
            env->state = 3;
        }
        break;
    case 1:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        MapObject_SetVisible(obj, 0);
        MapObject_SetHeldMovement(obj, 13);
        env->state++;
        break;
    case 2:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == 1) {
            MapObject_ClearHeldMovementIfActive(obj);
            env->state++;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished() && env->angle == Camera_GetPerspectiveAngle(fieldSystem->camera)) {
            return 1;
        }
        break;
    case 4:
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 1, 0, GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, 1), 1));
        env->state = 5;
        break;
    case 5:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 1)) {
            MapObject_SetVisible(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), 0);
            env->state = 6;
        }
        break;
    case 6:
        MapObject_SetHeldMovement(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), 13);
        env->state = 7;
        break;
    case 7:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == 1) {
            MapObject_ClearHeldMovementIfActive(obj);
            ov01_02205790(fieldSystem, 1);
            MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 1, 1, GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, 1), 0));
            env->state++;
        }
        break;
    case 8:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 1) && IsPaletteFadeFinished() && env->angle == Camera_GetPerspectiveAngle(fieldSystem->camera)) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 1);
            return 1;
        }
        break;
    }
    if (env->unk10) {
        ov01_021E9610(fieldSystem->camera, &env->unkD);
    }
    return 0;
}
