#include "global.h"

#include "constants/buildmodel.h"
#include "constants/movements.h"
#include "constants/sndseq.h"

#include "field/area_data.h"
#include "field/fieldmap.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"

#include "camera.h"
#include "camera_translation.h"
#include "error_handling.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_header.h"
#include "map_object.h"
#include "overlay_01.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "constants/player_avatar.h"

struct AreaDataManager;

struct AreaLightManager;

struct AreaMapAnimation;

struct BagCursor;

struct BgConfig;

struct BugContest;

struct Camera;

struct DoorAnimListEntry;

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

typedef void (*LocalMapObject_Callback)(LocalMapObject *object);

typedef struct TaskManagerBgGoTo TaskManagerBgGoTo;

typedef struct FieldEffect FieldEffect;

typedef struct PokedexResumeState PokedexResumeState;

typedef struct FieldMapSystems FieldMapSystems;

typedef struct FollowMonMood FollowMonMood;

typedef struct FollowMonInteraction FollowMonInteraction;

typedef struct FieldTerrainQueries FieldTerrainQueries;

typedef struct DoorAnimListEntry {
    u32 unk_00;
    u8 doorType;
    u8 padding_05[0x13];
} DoorAnimListEntry;

int GetDoorSE(FieldSystem *fieldSystem, int modelId, BOOL opening);

int GetDoorSE(FieldSystem *fieldSystem, int modelId, BOOL opening) {
    DoorAnimListEntry entry;

    NARC_ReadWholeMember(ov01_021FB904(fieldSystem->areaDataManager), modelId, &entry);
    if (opening) {
        switch (entry.doorType) {
        case 1:
            return SEQ_SE_DP_DOOR_OPEN;
        case 2:
            return SEQ_SE_DP_DOOR10;
        case 3:
            return SEQ_SE_PL_DOOR_OPEN5;
        case 4:
            return SEQ_SE_GS_HIKIDO_OPEN;
        }
        GF_ASSERT(FALSE);
        return SEQ_SE_DP_DOOR_OPEN;
    }
    switch (entry.doorType) {
    case 1:
        return SEQ_SE_DP_DOOR_CLOSE2;
    case 2:
        return 0;
    case 3:
        return 0;
    case 4:
        return SEQ_SE_GS_HIKIDO_CLOSE;
    }
    GF_ASSERT(FALSE);
    return SEQ_SE_DP_DOOR_CLOSE2;
}
