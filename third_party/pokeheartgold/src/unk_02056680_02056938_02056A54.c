#include "global.h"

#include "field/fieldmap.h"

#include "assert.h"
#include "camera.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "overlay_01_021E90C0.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_manager.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "unk_02054648.h"
#include "unk_02055BF0.h"
#include "unk_02056680.h"
#include "unk_02062108.h"

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BgConfig;

struct BugContest;

struct Camera;

struct DynamicTerrainHeightManager;

struct Field3dRenderObjManager;

struct FieldEffectManager;

struct FieldProcessManager;

struct FieldSystem;

struct FieldSystemUnkSub104;

struct FieldSystemUnkSub108;

struct FieldSystemUnkSub120;

struct FieldSystemUnkSub4;

struct FieldSystemUnkSubC8;

struct FieldViewPhoto;

struct FogData;

struct FollowMon;

struct FrontierFieldSystem;

struct GearPhoneRingManager;

struct LinkBattleRuleset;

struct LocalMapObject;

struct Location;

struct MapEvents;

struct MapLoadManager;

struct MapLoadMode;

struct MapMatrix;

struct MapObjectManager;

struct MapObjectsToPreload;

struct MapPropAnimationManager;

struct MapPropManager;

struct MapPropOneShotAnimationManager;

struct MenuInputStateMgr;

struct ModelAttributes;

struct NARC;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokedexData;

struct PokegearRingingTask;

struct SaveData;

struct SavePokegear;

struct Signpost;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

const fx32 sub_02054940(const FieldSystem *fieldSystem, const fx32 objectHeight, const fx32 objectX, const fx32 objectZ, u8 *newObjectHeightSource);

void sub_02056A54(FieldSystem *fieldSystem) {
    VecFx32 pos;
    VecFx32 delta;
    const VecFx32 *curTarget = Camera_GetCurrentTarget(fieldSystem->camera);
    VecFx32 lookAt = Camera_GetLookAtCamTarget(fieldSystem->camera);
    u32 dir;

    VEC_Subtract(&lookAt, curTarget, &delta);
    dir = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &pos);
    if (dir == 3) {
        pos.x -= FX32_CONST(16);
    } else {
        pos.x += FX32_CONST(16);
    }
    pos.y = sub_02054940(fieldSystem, pos.y, pos.x, pos.z, NULL);
    sub_0205C810(fieldSystem->playerAvatar, &pos, dir);
    Camera_SetLookAtTargetAndRecalcPos(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_SetFixedTarget(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_OffsetLookAtPosAndTarget(&delta, fieldSystem->camera);
}

void sub_02056AEC(FieldSystem *fieldSystem) {
    VecFx32 pos;
    VecFx32 delta;
    const VecFx32 *curTarget = Camera_GetCurrentTarget(fieldSystem->camera);
    VecFx32 lookAt = Camera_GetLookAtCamTarget(fieldSystem->camera);
    u32 dir;
    u8 behavior;

    VEC_Subtract(&lookAt, curTarget, &delta);
    dir = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &pos);
    behavior = GetMetatileBehavior(fieldSystem, PlayerAvatar_GetXCoord(fieldSystem->playerAvatar), PlayerAvatar_GetZCoord(fieldSystem->playerAvatar));
    if (MetatileBehavior_IsWarpStairsEast(behavior)) {
        pos.x += FX32_CONST(16);
        dir = 2;
    } else if (MetatileBehavior_IsWarpStairsWest(behavior)) {
        pos.x -= FX32_CONST(16);
        dir = 3;
    }
    pos.y = sub_02054940(fieldSystem, pos.y, pos.x, pos.z, NULL);
    sub_0205C810(fieldSystem->playerAvatar, &pos, dir);
    Camera_SetLookAtTargetAndRecalcPos(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_SetFixedTarget(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_OffsetLookAtPosAndTarget(&delta, fieldSystem->camera);
    if (FollowMon_IsActive(fieldSystem)) {
        MapObject_SetFacingDirectionDirect(FollowMon_GetMapObject(fieldSystem), dir);
    }
}

void sub_02056BC8(FieldSystem *fieldSystem) {
    VecFx32 pos;
    VecFx32 delta;
    const VecFx32 *curTarget = Camera_GetCurrentTarget(fieldSystem->camera);
    VecFx32 lookAt = Camera_GetLookAtCamTarget(fieldSystem->camera);
    LocalMapObject *obj;

    VEC_Subtract(&lookAt, curTarget, &delta);
    obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &pos);
    PlayerAvatar_ToggleAutomaticHeightUpdating(fieldSystem->playerAvatar, FALSE);
    pos.y -= FX32_CONST(32);
    sub_0205C810(fieldSystem->playerAvatar, &pos, 0);
    Camera_SetLookAtTargetAndRecalcPos(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_SetFixedTarget(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_OffsetLookAtPosAndTarget(&delta, fieldSystem->camera);
    MapObject_ClearHeldMovementIfActive(obj);
    Field_PlayerAvatar_OrrTransitionFlags(fieldSystem->playerAvatar, 512);
    Field_PlayerAvatar_ApplyTransitionFlags(fieldSystem->playerAvatar);
    sub_0205F328(obj, 0);
}

void sub_02056C64(FieldSystem *fieldSystem) {
    VecFx32 pos;
    VecFx32 delta;
    const VecFx32 *curTarget = Camera_GetCurrentTarget(fieldSystem->camera);
    VecFx32 lookAt = Camera_GetLookAtCamTarget(fieldSystem->camera);
    LocalMapObject *obj;

    VEC_Subtract(&lookAt, curTarget, &delta);
    obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &pos);
    PlayerAvatar_ToggleAutomaticHeightUpdating(fieldSystem->playerAvatar, FALSE);
    pos.y += FX32_CONST(32);
    sub_0205C810(fieldSystem->playerAvatar, &pos, 0);
    Camera_SetLookAtTargetAndRecalcPos(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_SetFixedTarget(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_OffsetLookAtPosAndTarget(&delta, fieldSystem->camera);
    MapObject_ClearHeldMovementIfActive(obj);
    Field_PlayerAvatar_OrrTransitionFlags(fieldSystem->playerAvatar, 512);
    Field_PlayerAvatar_ApplyTransitionFlags(fieldSystem->playerAvatar);
    sub_0205F328(obj, 0);
}
