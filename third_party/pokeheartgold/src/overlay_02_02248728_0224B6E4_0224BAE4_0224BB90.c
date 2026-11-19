#include "global.h"

#include "constants/sndseq.h"

#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_021FB878.h"

#include "error_handling.h"
#include "field_system.h"
#include "heap.h"
#include "overlay_02.h"
#include "task.h"
#include "unk_02005D10.h"
#include "constants/player_avatar.h"

struct AreaDataManager;

struct AreaLightManager;

struct AreaMapAnimation;

struct BagCursor;

struct BgConfig;

struct BugContest;

struct Camera;

struct DynamicTerrainHeightManager;

struct Field3dRenderObjManager;

struct FieldCameraShift;

struct FieldEffect;

struct FieldEffectManager;

struct FieldMapSystems;

struct FieldProcessManager;

struct FieldSystem;

struct FieldTerrainQueries;

struct FieldViewPhoto;

struct FogData;

struct FollowMon;

struct FollowMonInteraction;

struct FollowMonMood;

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

struct MapProp;

struct MapPropAnimationManager;

struct MapPropAnimationPoolList;

struct MapPropManager;

struct MapPropOneShotAnimationManager;

struct MenuInputStateMgr;

struct ModelAttributes;

struct NARC;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokecenterAnim;

struct PokedexData;

struct PokedexResumeState;

struct PokegearRingingTask;

struct SaveData;

struct SavePokegear;

struct Signpost;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerBgGoTo;

struct TerrainAttributes;

struct TimeOfDayPropAnimManager;

struct UnionRoom;

struct UnionRoomObjects;

typedef struct AreaMapAnimation AreaMapAnimation;

typedef struct MapPropAnimationPoolList MapPropAnimationPoolList;

typedef struct TimeOfDayPropAnimManager TimeOfDayPropAnimManager;

typedef struct PokecenterAnim {
    VecFx32 machinePos;
    u8 ballCount;
    u8 ballIndex;
    u8 timer;
    u8 state;
    u8 ballProps[6];
} PokecenterAnim;

typedef void (*LocalMapObject_Callback)(LocalMapObject *object);

typedef struct TaskManagerBgGoTo TaskManagerBgGoTo;

typedef struct FieldEffect FieldEffect;

typedef struct PokedexResumeState PokedexResumeState;

typedef struct FieldMapSystems FieldMapSystems;

typedef struct FollowMonMood FollowMonMood;

typedef struct FollowMonInteraction FollowMonInteraction;

extern const VecFx32 ov02_02253D90[];

static inline NNSG3dResMdl *MapPropModelFile_GetModel(NNSG3dResFileHeader **modelFile) {
    return NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(*modelFile), 0);
}

enum HealAnimState {
    HEAL_ANIM_STATE_LOAD,
    HEAL_ANIM_STATE_PLACE_BALL,
    HEAL_ANIM_STATE_WAIT_BALL,
    HEAL_ANIM_STATE_PLAY,
    HEAL_ANIM_STATE_WAIT,
    HEAL_ANIM_STATE_END,
};

NNSG3dResFileHeader **ov01_021FB90C(const int mapPropModelID, AreaDataManager *const areaDataManager);

s32 sub_02054C20(void *arg0, s32 arg1, MapProp **arg2, s32 *arg3);

BOOL PokecenterAnimRun(TaskManager *taskManager);

BOOL PokecenterAnimRun(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    PokecenterAnim *anim = TaskManager_GetEnvironment(taskManager);
    MapProp *machine;
    VecFx32 pos;
    NNSG3dResFileHeader **ballModelFile;
    NNSG3dResFileHeader **machineModelFile;
    NNSG3dResMdl *ballModel;
    NNSG3dResMdl *machineModel;
    NNSG3dRenderObj *renderObj;
    u8 i;

    switch (anim->state) {
    case HEAL_ANIM_STATE_LOAD:
        ballModelFile = ov01_021FB90C(0x6b, fieldSystem->areaDataManager);
        machineModelFile = ov01_021FB90C(0x25, fieldSystem->areaDataManager);
        ballModel = MapPropModelFile_GetModel(ballModelFile);
        machineModel = MapPropModelFile_GetModel(machineModelFile);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 0x10, 0x6b, ((void *)0), ballModel, ov01_021FB9E0(fieldSystem->areaDataManager), 1, 1, 0);
        if (!sub_02054C20(fieldSystem, 0x25, &machine, ((void *)0))) {
            ((0) ? (void)0 : GF_AssertFail());
        }
        renderObj = MapProp_GetRenderSurface(machine);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 0x20, 0x25, renderObj, machineModel, ov01_021FB9E0(fieldSystem->areaDataManager), 1, 1, 0);
        anim->state++;
        break;
    case HEAL_ANIM_STATE_PLACE_BALL: {
        VecFx32 rotation = { 0 };
        pos.x = anim->machinePos.x + ov02_02253D90[anim->ballIndex].x;
        pos.y = anim->machinePos.y + ov02_02253D90[anim->ballIndex].y;
        pos.z = anim->machinePos.z + ov02_02253D90[anim->ballIndex].z;
        PlaySE(1533);
        anim->ballProps[anim->ballIndex] = MapPropManager_LoadOne(fieldSystem->mapPropManager, 0x6b, &pos, &rotation, fieldSystem->mapPropAnimationManager);
        anim->state++;
        break;
    }
    case HEAL_ANIM_STATE_WAIT_BALL:
        if (anim->timer < 12) {
            anim->timer++;
        } else {
            anim->timer = 0;
            anim->ballIndex++;
            if (anim->ballIndex < anim->ballCount) {
                anim->state = HEAL_ANIM_STATE_PLACE_BALL;
            } else {
                MapPropOneShotAnimationManager_SetAnimationRenderObj(fieldSystem->mapPropOneShotAnimationManager, 0x10, 0, MapProp_GetRenderSurface(MapPropManager_GetMapPropByIndex_Checked_RequireActive(fieldSystem->mapPropManager, anim->ballProps[0])));
                anim->state++;
            }
        }
        break;
    case HEAL_ANIM_STATE_PLAY:
        MapPropOneShotAnimationManager_PlayAnimation(fieldSystem->mapPropOneShotAnimationManager, 0x10, 0);
        MapPropOneShotAnimationManager_PlayAnimation(fieldSystem->mapPropOneShotAnimationManager, 0x20, 0);
        PlayFanfare(1183);
        anim->state++;
        break;
    case HEAL_ANIM_STATE_WAIT:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 0x10)
            && MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 0x20)
            && !IsFanfarePlaying()) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 0x20);
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 0x10);
            for (i = 0; i < anim->ballCount; i = (u8)(i + 1)) {
                MapPropManager_RemoveMapPropByIndex(anim->ballProps[i], fieldSystem->mapPropManager);
            }
            anim->state++;
        }
        break;
    case HEAL_ANIM_STATE_END:
        Heap_Free(anim);
        return 1;
    }
    return 0;
}
