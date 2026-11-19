#include "global.h"

#include "constants/heap.h"

#include "field/area_data.h"
#include "field/fieldmap.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_021E90C0.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "unk_02055BF0.h"
#include "unk_02062108.h"

s32 GetDoorSE(FieldSystem *, s32, s32);

void ov01_021E9610(Camera *camera, u8 *param1);

s32 ov01_021EA1F4(FieldSystem *, s32);

s32 sub_02054A60(u32, s32, s32, s32, s32, s32, s32 *);

s32 sub_02054AE4(FieldSystem *, u32, s32 *, MapProp **);

u32 *sub_02054D10(FieldSystem *, s32, s32, s32 *, s32);

BOOL ov01_021E9374(FieldSystem *fieldSystem, FieldEnvSubUnk18 *a1);

BOOL ov01_021E9374(FieldSystem *fieldSystem, FieldEnvSubUnk18 *base) {
    MapPropDoorAnimEnv *a1 = (MapPropDoorAnimEnv *)base;

    s32 sp48[4];
    s32 sp30[6];
    MapProp *sp2C;
    NNSG3dResTex *sp28;
    s32 sp24;
    s32 sp20;
    NARC *sp1C;
    u32 *sp18;
    LocalMapObject *temp_r0_6;
    NNSG3dRenderObj *temp_r5;
    NNSG3dResTex *temp_r0_4;
    s32 temp_r0;
    s32 temp_r0_5;
    s32 var_r7;
    u32 *temp_r0_2;
    u8 temp_r0_3;

    temp_r0 = a1->base.state;
    switch ((u32)temp_r0) {
    case 0:
        a1->unk_10 = 0;
        a1->unk_0D = 0;
        sub_02054A60(a1->base.direction, a1->unk_08, -1, 0, 3, 1, sp48);
        a1->unk_0C = 1;
        sp24 = 0;
        sp1C = ov01_021FB904(fieldSystem->areaDataManager);
        temp_r0_2 = sub_02054D10(fieldSystem, 4, 4, sp48, 0);
        sp18 = temp_r0_2;
        for (var_r7 = 0; var_r7 < 4; var_r7++) {
            if (sp18[var_r7] != 0) {
                NARC_ReadWholeMember(sp1C, sp18[var_r7], &sp30);
                if (*(u8 *)&sp30[1] != 0) {
                    sub_02054AE4(fieldSystem, sp18[var_r7], sp48, &sp2C);
                    sp24 = 1;
                    sp20 = (s32)sp18[var_r7];
                    break;
                }
            }
        }
        Heap_Free(sp18);
        if (sp24 != 0) {
            temp_r0_3 = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, sp20);
            if (temp_r0_3 == 0) {
                FieldMap_FadeScreen(1U);
                a1->base.state = 6;
                return 0;
            }
            temp_r0_4 = ov01_021FB9E0(fieldSystem->areaDataManager);
            sp28 = temp_r0_4;
            temp_r5 = MapProp_GetRenderSurface(sp2C);
            MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 1, sp20, temp_r5, MapProp_GetResModel(sp2C), temp_r0_4, (s32)temp_r0_3, 1U, 0);
        } else {
            FieldMap_FadeScreen(1);
            a1->base.state = 6;
            return 0;
        }
        a1->unk_0E = Camera_GetPerspectiveAngle(fieldSystem->camera);
        if (ov01_021EA1F4(fieldSystem, sp20) == 0) {
            Camera_AdjustPerspectiveAngle(0xFFA0U, fieldSystem->camera);
        }
        a1->base.state += 1;
        break;
    case 1:
        NewFieldFadeEnvironment(fieldSystem->taskman, FADE_BOTH_SCREENS, FADE_TYPE_CAMERA_SHUTTER_OPEN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_FIELD2);
        temp_r0_5 = MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, 1);
        if (ov01_021EA1F4(fieldSystem, temp_r0_5) == 0) {
            a1->unk_10 = 1;
        }
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 1, 0, GetDoorSE(fieldSystem, temp_r0_5, 1));
        a1->base.state += 1;
        break;
    case 2:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 1) != 0) {
            MapObject_SetVisible(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), 0);
            a1->base.state += 1;
        }
        break;
    case 3:
        MapObject_SetHeldMovement(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), 0xDU);
        a1->base.state += 1;
        break;
    case 4:
        temp_r0_6 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(temp_r0_6) == 1) {
            MapObject_ClearHeldMovementIfActive(temp_r0_6);
            ov01_02205790(fieldSystem, 1U);
            MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 1, 1, GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, 1), 0));
            a1->base.state += 1;
        }
        break;
    case 5:
        if ((MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 1) != 0) && (IsPaletteFadeFinished() != 0) && (a1->unk_0E == Camera_GetPerspectiveAngle(fieldSystem->camera))) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 1);
            return 1;
        }
        break;
    case 6:
        if (IsPaletteFadeFinished() != 0) {
            return 1;
        }
        break;
    }
    if (a1->unk_10 != 0) {
        ov01_021E9610(fieldSystem->camera, &a1->unk_0D);
    }
    return 0;
}
