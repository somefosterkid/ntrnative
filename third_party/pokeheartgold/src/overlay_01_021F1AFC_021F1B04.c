#include "global.h"

#include "constants/flags.h"
#include "constants/global_fieldmap.h"
#include "constants/maps.h"
#include "constants/player_avatar.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/sprites.h"
#include "constants/std_script.h"

#include "field/field_3d_object_task.h"
#include "field/field_control.h"

#include "field_bgm.h"
#include "field_system.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "overlay_02.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "save_vars_flags.h"
#include "script_manager.h"
#include "sound_radio.h"
#include "sys_flags.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "task.h"

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

struct RockClimbWork;

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

struct WaterfallWork;

typedef struct AreaMapAnimation AreaMapAnimation;

typedef struct MapPropAnimationPoolList MapPropAnimationPoolList;

typedef struct TimeOfDayPropAnimManager TimeOfDayPropAnimManager;

typedef void (*LocalMapObject_Callback)(LocalMapObject *object);

typedef struct TaskManagerBgGoTo TaskManagerBgGoTo;

typedef struct FieldEffect FieldEffect;

typedef struct PokedexResumeState PokedexResumeState;

typedef struct FieldMapSystems FieldMapSystems;

typedef struct FollowMonMood FollowMonMood;

typedef struct FollowMonInteraction FollowMonInteraction;

typedef struct FieldTerrainQueries FieldTerrainQueries;

typedef struct RockClimbWork RockClimbWork;

typedef struct WaterfallWork WaterfallWork;

enum FieldMoveTaskStepResult {
    FIELD_MOVE_TASK_STEP_WAIT,
    FIELD_MOVE_TASK_STEP_DONE,
    FIELD_MOVE_TASK_STEP_NEXT,
};

void Field_PlayerAvatar_TransitionToWalking(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToCycling(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToSurfing(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToRocket(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionX0008(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToWatering(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToFishing(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToPoketch(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToSaving(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToHeal(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToLadder(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToRocketHeal(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToPokeathlon(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToApricornShake(PlayerAvatar *playerAvatar);

void Field_PlayerAvatar_TransitionToRocketSaving(PlayerAvatar *playerAvatar);

extern void (*const sPlayerAvatarBitUpdateFuncs[15])(PlayerAvatar *);

enum FieldMoveTaskStepResult RockClimb_PlayFollowMonCry(RockClimbWork *work, TaskManager *taskManager);

enum FieldMoveTaskStepResult RockClimb_CreateEffect(RockClimbWork *work, TaskManager *taskManager);

enum FieldMoveTaskStepResult WaterfallUp_Init(WaterfallWork *work);

void Field_PlayerAvatar_ApplyTransitionFlags(PlayerAvatar *avatar);

void Field_PlayerAvatar_ApplyTransitionFlags(PlayerAvatar *avatar) {
    u32 i = 0;
    u32 flags = PlayerAvatar_GetTransitionFlags(avatar);
    for (; i < 15; i++) {
        if (flags & 1) {
            sPlayerAvatarBitUpdateFuncs[i](avatar);
        }
        flags >>= 1;
    }
    PlayerAvatar_SetTransitionFlags(avatar, 0);
}
