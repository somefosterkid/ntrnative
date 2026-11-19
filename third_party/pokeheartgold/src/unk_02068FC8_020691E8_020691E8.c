#include "global.h"

#include "field_system.h"
#include "game_stats.h"
#include "gf_rtc.h"
#include "heap.h"
#include "igt.h"
#include "launch_application.h"
#include "player_data.h"
#include "pokedex.h"
#include "save_trainer_card.h"
#include "save_vars_flags.h"
#include "string_util.h"
#include "sys_flags.h"
#include "task.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_02055418.h"
#include "unk_02068FC8.h"

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

struct FrontierSave;

struct GameStats;

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

struct Pokedex;

struct PokedexData;

struct PokegearRingingTask;

struct SaveData;

struct SavePokegear;

struct SaveVarsFlags;

struct Signpost;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

int sub_020691E8(FieldSystem *fieldSystem) {
    SaveData *saveData;
    SaveVarsFlags *varsFlags;
    u8 count;
    FrontierSave *frontierSave;

    saveData = FieldSystem_GetSaveData(fieldSystem);
    Save_GameStats_Get(saveData);
    varsFlags = Save_VarsFlags_Get(saveData);
    frontierSave = Save_Frontier_GetStatic(saveData);
    count = 0;
    if (CheckGameClearFlag(varsFlags)) {
        count++;
    }
    if (Pokedex_NationalDexIsComplete(Save_Pokedex_Get(saveData))) {
        count++;
    }
    if (FrontierSave_GetStat(frontierSave, 0, 0xff) >= 100 || FrontierSave_GetStat(frontierSave, 2, 0xff) >= 100 || FrontierSave_GetStat(frontierSave, 4, 0xff) >= 100 || FrontierSave_GetStat(frontierSave, 6, 0xff) >= 100 || FrontierSave_GetStat(frontierSave, 8, 0xff) >= 100) {
        count++;
    }
    if (Save_VarsFlags_CheckFlagInArray(varsFlags, 0xf1)) {
        count++;
    }
    if (Save_VarsFlags_CheckFlagInArray(varsFlags, 0x61 << 2)) {
        count++;
    }
    return count;
}
