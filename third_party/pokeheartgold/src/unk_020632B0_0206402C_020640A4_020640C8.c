#include "global.h"

#include "constants/sprites.h"
#include "constants/std_script.h"

#include "error_handling.h"
#include "field_system.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "player_avatar.h"
#include "script_manager.h"
#include "sys_task_api.h"
#include "constants/player_avatar.h"

struct AreaDataManager;

struct AreaLightManager;

struct AreaMapAnimation;

struct BagCursor;

struct BgConfig;

struct BugContest;

struct Camera;

struct DynamicTerrainHeightManager;

struct EngageParams;

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

typedef void (*LocalMapObject_Callback)(LocalMapObject *object);

typedef struct TaskManagerBgGoTo TaskManagerBgGoTo;

typedef struct FieldEffect FieldEffect;

typedef struct AreaMapAnimation AreaMapAnimation;

typedef struct MapPropAnimationPoolList MapPropAnimationPoolList;

typedef struct TimeOfDayPropAnimManager TimeOfDayPropAnimManager;

typedef struct PokedexResumeState PokedexResumeState;

typedef struct FieldMapSystems FieldMapSystems;

typedef struct FollowMonMood FollowMonMood;

typedef struct FollowMonInteraction FollowMonInteraction;

typedef struct EngageParams {
    int x;
    int z;
    int scriptId;
    int trainerNum;
    BOOL isDouble;
    LocalMapObject *object;
} EngageParams;

BOOL CheckSeenByNpcTrainers(FieldSystem *fieldSystem, MapObjectManager *manager, PlayerAvatar *avatar, LocalMapObject *exclude, EngageParams *out);

void GetEngagingTrainerParams(EngageParams *out, LocalMapObject *object, int x, int z);

LocalMapObject *sub_02064520(FieldSystem *unused, MapObjectManager *manager, LocalMapObject *exclude, u32 trainerNum);

BOOL TryGetSeenByNpcTrainers(FieldSystem *fieldSystem, BOOL doublesEligible);

BOOL TryGetSeenByNpcTrainers(FieldSystem *fieldSystem, BOOL doublesEligible) {
    EngageParams first;
    EngageParams second;
    EngageParams partner;
    PlayerAvatar *avatar = fieldSystem->playerAvatar;
    MapObjectManager *manager = fieldSystem->mapObjectManager;

    if (!CheckSeenByNpcTrainers(fieldSystem, manager, avatar, NULL, &first)) {
        return FALSE;
    }
    if (first.isDouble == FALSE) {
        StartMapSceneScript(fieldSystem, std_trainer_approach, first.object);
        if (!doublesEligible || !CheckSeenByNpcTrainers(fieldSystem, manager, avatar, first.object, &second)) {
            FieldSystem_SetEngagedTrainer(fieldSystem, first.object, first.x, first.z, first.scriptId, first.trainerNum, 0, 0);
            return TRUE;
        }
        FieldSystem_SetEngagedTrainer(fieldSystem, first.object, first.x, first.z, first.scriptId, first.trainerNum, 2, 0);
        FieldSystem_SetEngagedTrainer(fieldSystem, second.object, second.x, second.z, second.scriptId, second.trainerNum, 2, 1);
        return TRUE;
    }
    if (first.isDouble == TRUE) {
        if (!doublesEligible) {
            return FALSE;
        }
        GetEngagingTrainerParams(&partner, sub_02064520(fieldSystem, manager, first.object, first.trainerNum), first.x, first.z);
        StartMapSceneScript(fieldSystem, std_trainer_approach, first.object);
        FieldSystem_SetEngagedTrainer(fieldSystem, first.object, first.x, first.z, first.scriptId, first.trainerNum, 1, 0);
        FieldSystem_SetEngagedTrainer(fieldSystem, partner.object, partner.x, partner.z, partner.scriptId, partner.trainerNum, 1, 1);
        return TRUE;
    }
    GF_ASSERT(FALSE);
    return FALSE;
}
