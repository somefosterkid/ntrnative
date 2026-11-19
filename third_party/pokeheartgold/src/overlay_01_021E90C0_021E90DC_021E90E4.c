#include "global.h"

#include "constants/heap.h"

#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "camera_translation.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_021E90C0.h"
#include "overlay_01_022053EC.h"
#include "player_avatar.h"
#include "unk_02062108.h"

s32 GetDoorSE(FieldSystem *, s32, u32);

void ov01_021E95CC(Camera *, u8 *);

s32 ov01_021E9700(u32);

s32 ov01_021EA1F4(FieldSystem *, s32);

void sub_02054A60(u32, s32, u32, u32, u32, u32, u8 *);

void sub_02054AE4(FieldSystem *, u32, u8 *, MapProp **);

s32 *sub_02054D10(FieldSystem *, u32, u32, u8 *, u32);

extern u32 ov01_02206428;

BOOL ov01_021E90E4(FieldSystem *fieldSystem, FieldEnvSubUnk18 *a1);

BOOL ov01_021E90E4(FieldSystem *fieldSystem, FieldEnvSubUnk18 *base) {
    MapPropDoorAnimEnv *a1 = (MapPropDoorAnimEnv *)base;

    u8 auStack_28[16];
    u8 auStack_40[24];
    u8 bVar2;
    u8 uVar3;
    u16 uVar4;
    s32 iVar5;
    BOOL bVar1;
    u32 uStack_50;
    NNSG3dResTex *mapPropTexture;
    NNSG3dRenderObj *mapPropRenderObj;
    NNSG3dResMdl *mapPropModel;
    s32 iVar6;
    u32 x;
    u32 z;
    BOOL BVar7;
    LocalMapObject *object;
    LocalMapObject *pLVar8;
    GFCameraTranslationWrapper *wrapper;
    NARC *narc;
    u32 *ptr;
    MapProp *pMStack_44;

    iVar5 = a1->base.state;
    {
        switch (iVar5) {
        case 0:
            a1->unk_10 = 0;
            a1->unk_0D = 0;
            sub_02054A60(a1->base.direction, a1->unk_08, 0xffffffff, 0xffffffff, 3, 1, auStack_28);
            a1->unk_0C = 1;
            bVar1 = FALSE;
            narc = ov01_021FB904(fieldSystem->areaDataManager);
            ptr = (u32 *)sub_02054D10(fieldSystem, 4, 4, auStack_28, 0);
            for (iVar5 = 0; iVar5 < 4; iVar5++) {
                if (ptr[iVar5] != 0) {
                    NARC_ReadWholeMember(narc, ptr[iVar5], auStack_40);
                    if (auStack_40[4] != 0) {
                        sub_02054AE4(fieldSystem, ptr[iVar5], auStack_28, &pMStack_44);
                        bVar1 = TRUE;
                        uStack_50 = ptr[iVar5];
                        break;
                    }
                }
            }
            Heap_Free(ptr);
            if (bVar1) {
                bVar2 = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, uStack_50);
                if (bVar2 == 0) {
                    return 1;
                }
                mapPropTexture = ov01_021FB9E0(fieldSystem->areaDataManager);
                mapPropRenderObj = MapProp_GetRenderSurface(pMStack_44);
                mapPropModel = MapProp_GetResModel(pMStack_44);
                MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 1, uStack_50, mapPropRenderObj, mapPropModel, mapPropTexture, (u32)bVar2, 1, 0);
                ov01_021E90B0(fieldSystem->mapPropOneShotAnimationManager, 1, 1);
            } else {
                return 1;
            }
            uVar4 = ov01_021E9700(uStack_50);
            a1->unk_1E = uVar4;
            if (a1->unk_1E != 0xffff) {
                a1->base.state = 100;
            } else {
                a1->base.state = a1->base.state + 1;
            }
            break;
        case 1:
            iVar5 = MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, 1);
            iVar6 = ov01_021EA1F4(fieldSystem, iVar5);
            if (iVar6 == 0) {
                Camera_ClearFixedTarget(fieldSystem->camera);
                a1->unk_10 = 1;
            }
            iVar5 = GetDoorSE(fieldSystem, iVar5, 1);
            MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 1, 0, iVar5);
            a1->base.state = a1->base.state + 1;
            break;
        case 2:
            BVar7 = MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 1);
            if (BVar7 != 0) {
                a1->base.state = a1->base.state + 1;
            }
            break;
        case 3:
            pLVar8 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
            MapObject_SetHeldMovement(pLVar8, 0xc);
            BVar7 = FollowMon_IsActive(fieldSystem);
            if ((BVar7 != 0) && (BVar7 = ov01_022057C4(fieldSystem), BVar7 == 0)) {
                x = MapObject_GetPreviousXCoord(pLVar8);
                z = MapObject_GetPreviousZCoord(pLVar8);
                ov01_02205990(0xc, x, z, (FollowMon *)((u8 *)fieldSystem + 228));
            }
            a1->base.state = a1->base.state + 1;
            break;
        case 4:
            pLVar8 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
            BVar7 = MapObject_IsMovementPaused(pLVar8);
            if (BVar7 == 1) {
                BVar7 = FollowMon_IsActive(fieldSystem);
                if (BVar7 != 0) {
                    object = FollowMon_GetMapObject(fieldSystem);
                    BVar7 = MapObject_IsMovementPaused(object);
                    if (BVar7 == 1) {
                        MapObject_ClearHeldMovementIfActive(pLVar8);
                        MapObject_ClearHeldMovementIfActive(object);
                        a1->base.state = a1->base.state + 1;
                    }
                } else {
                    a1->base.state = a1->base.state + 1;
                }
            }
            break;
        case 5:
            pLVar8 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
            MapObject_SetVisible(pLVar8, 1);
            return 1;
        case 100:
            Camera_SetPerspectiveClippingPlane(0x96000, 0x456000, fieldSystem->camera);
            wrapper = CreateCameraTranslationWrapper(HEAP_ID_FIELD1, fieldSystem->camera);
            a1->wrapper = wrapper;
            SetCameraTranslationPath(wrapper, (CameraTranslationPathTemplate *)(&ov01_02206428 + (u32) a1->unk_1E * 5), 0x18);
            a1->base.state = a1->base.state + 1;
            break;
        case 101:
            if ((uVar3 = IsCameraTranslationFinished(a1->wrapper), uVar3 != 0)) {
                DeleteCameraTranslationWrapper(a1->wrapper);
                a1->unk_1C = 1;
                a1->base.state = 1;
            }
            break;
        }
    }
    if ((a1->unk_10 != 0) && (a1->unk_1C == 0)) {
        ov01_021E95CC(fieldSystem->camera, &a1->unk_0D);
    }
    return 0;
}
