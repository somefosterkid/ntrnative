#include "global.h"

#include "heap.h"
#include "map_object.h"
#include "math_util.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "unk_0205FD20.h"
#include "field/overlay_01_02204004.h"
#include "field/map_prop_animation.h"
#include "field/area_data.h"
#include "field/model_attributes.h"
#include "field/area_light.h"
#include "field/draw_map_name.h"
#include "field/dynamic_terrain_height.h"
#include "field/field_3d_object_task.h"
#include "field/hblank_system_priv.h"
#include "field/map_prop.h"
#include "constants/player_avatar.h"
#include "field/signpost.h"
#include "field/fog.h"
#include "field/weather_manager.h"
#include "bag_cursor.h"
#include "dex_mon_measures.h"
#include "save_link_ruleset.h"
#include "map_matrix.h"
#include "menu_input_state.h"
#include "overlay_01_021F1348.h"
#include "overlay_109.h"
#include "photo_album.h"
#include "photo_types_def.h"
#include "frontier_data.h"
#include "scrcmd_9.h"
#include "terrain_attributes.h"
#include "field_system_def.h"
#include "field_system.h"

struct AreaDataManager;

struct AreaLightManager;

struct AreaMapAnimation;

struct AsyncWriteManager;

struct BUGMON;

struct Background;

struct BagCursor;

struct BagCursorBattle;

struct BagCursorField;

struct BattleSetup;

struct BattleTowerChallenge;

struct BgConfig;

struct BgEvent;

struct BicycleSlopeAnimation;

struct BoxPokemon;

struct BugContest;

struct BugContestant;

struct BugContestantData;

struct CAPSULE;

struct Camera;

struct CameraAngle;

struct CameraHistory;

struct CameraLookAt;

struct CameraPerspective;

struct CoordEvent;

struct DynamicTerrainHeightManager;

struct DynamicTerrainHeightPlate;

struct EncounterData;

struct EncounterDataSlot;

struct Field3dObjectTask;

struct Field3dObjectTaskManager;

struct Field3dObjectTaskTemplate;

struct Field3dRenderObj;

struct Field3dRenderObjManager;

struct FieldCameraShift;

struct FieldDrawMapNameInfo;

struct FieldEffect;

struct FieldEffectManager;

struct FieldMapSystems;

struct FieldProcessManager;

struct FieldSystem;

struct FieldTerrainQueries;

struct FieldTextureManager;

struct FieldViewPhoto;

struct FogData;

struct FollowMon;

struct FollowMonInteraction;

struct FollowMonInteractionStep;

struct FollowMonMood;

struct FollowMonMovementStep;

struct FollowMonRule;

struct FrontierFieldSystem;

struct FrontierMon;

struct FrontierMultiBattleAllyData;

struct FrontierTrainer;

struct GearPhoneRingManager;

struct HBlankSystem;

struct JumpByOffsetWork;

struct JumpOffsetData;

struct LandEncounterData;

struct LinkBattleRuleset;

struct LocalMapObject;

struct Location;

struct MAPDATA;

struct Mail;

struct MailMessage;

struct MapEvents;

struct MapLoadManager;

struct MapLoadMode;

struct MapMarkingsSave;

struct MapMarkingsSaveArray;

struct MapMatrix;

struct MapMatrixData;

struct MapObjectManager;

struct MapObjectsToPreload;

struct MapPropAnimation;

struct MapPropAnimationData;

struct MapPropAnimationManager;

struct MapPropAnimationPool;

struct MapPropAnimationPoolList;

struct MapPropManager;

struct MapPropOneShotAnimation;

struct MapPropOneShotAnimationList;

struct MapPropOneShotAnimationManager;

struct MenuInputStateMgr;

struct ModelAttributes;

struct MsgData;

struct NARC;

struct ObjectEvent;

struct OverlayManager;

struct OverlayManagerTemplate;

struct Party;

struct PartyCore;

struct PartyExtra;

struct PartyExtraSub;

struct PartyPokemon;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PhoneContact;

struct PhoneRematch;

struct Photo;

struct PhotoAlbum;

struct PhotoAlbumArgs;

struct PhotoCameraParam;

struct PhotoMon;

struct PlayerAvatar;

struct PlayerSaveData;

struct PoisonEffect;

struct PokedexData;

struct PokedexResumeState;

struct PokegearRingingTask;

struct SEAL;

struct SaveArrayHeader;

struct SaveData;

struct SavePokegear;

struct SaveSlotSpec;

struct Signpost;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerBgGoTo;

struct TerrainAttributes;

struct TextureSlot;

struct TimeOfDayPropAnimManager;

struct TimeOfDayPropAnimSet;

struct UnionRoom;

struct UnionRoomObjects;

struct UnusedFieldModel;

struct WarpEvent;

struct WeatherManager;

struct WeatherSystem;

struct Window;

struct _MAT;

struct _MAT_ENTRY;

typedef struct MapPropAnimationPool MapPropAnimationPool;

typedef void (*MapPropAnimationLoadFunc)(MapPropAnimationPool *pool, MapPropAnimation *animation, void *res, int unused);

typedef struct AreaMapAnimation {
    MapPropAnimationPool *pool;
    MapPropAnimation *mapPropAnimation;
} AreaMapAnimation;

struct MapPropAnimationPool {
    NNSFndAllocator allocator;
    MapPropAnimation *animations;
    MapPropAnimation **freeList;
    MapPropAnimationPool *next;
    MapPropAnimationLoadFunc loadFunc;
    u16 capacity;
    u16 numUsed;
};

typedef struct MapPropAnimationPoolList {
    MapPropAnimationPool *unk0;
    MapPropAnimationPool *head;
    MapPropAnimationPool *tail;
    int numPools;
} MapPropAnimationPoolList;

typedef struct TimeOfDayPropAnimSet {
    BOOL active;
    int numAnims;
    MapPropAnimation *animations[4];
    NNSG3dRenderObj *renderObj;
} TimeOfDayPropAnimSet;

typedef struct TimeOfDayPropAnimManager {
    TIMEOFDAY timeOfDay;
    int numSets;
    TimeOfDayPropAnimSet sets[4];
} TimeOfDayPropAnimManager;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

struct MapMarkingsSaveArray {
    MapMarkingsSave list[100];
};

struct PhoneContact {
    u8 id;
};

PokedexData *PokedexData_Create(enum HeapID heapID);

void PokedexData_Delete(PokedexData *pokedexData);

void PokedexData_LoadAll(PokedexData *pokedexData, int playerGender, enum HeapID heapID);

void PokedexData_UnloadAll(PokedexData *pokedexData);

u32 PokedexData_GetHeight(PokedexData *pokedexData, int species);

u32 PokedexData_GetWeight(PokedexData *pokedexData, int species);

typedef void (*LocalMapObject_Callback)(LocalMapObject *object);

typedef struct TaskManagerBgGoTo {
    u32 bgGoToId;
} TaskManagerBgGoTo;

typedef struct FieldEffect FieldEffect;

typedef struct BattleTowerChallenge {
    u8 allyReplaceItem : 1;
    u8 saved : 1;
    u8 towerMode : 3;
    u8 multiAllyId : 3;
    u8 unk1;
    u8 battleNumber;
    u8 totalTimesFainted;
    u16 totalTurns;
    u16 totalDamage;
    u8 partySlots[4];
    u8 trainers[28];
    u32 randomState;
    u8 multiAllyData[16];
} BattleTowerChallenge;

typedef struct UnusedFieldModel {
    enum HeapID heapID;
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *model;
    NNSG3dResFileHeader *modelFile;
} UnusedFieldModel;

typedef struct PokedexResumeState {
    u16 species;
    u16 dexMode;
} PokedexResumeState;

typedef struct FieldMapSystems {
    u32 unk0;
    Field3dObjectTaskManager *field3dObjectTaskManager;
    FieldDrawMapNameInfo *drawMapNameInfo;
    WeatherManager *weatherManager;
    FieldTextureManager *textureManager;
    u32 unk14;
    UnusedFieldModel *unusedFieldModel;
    HBlankSystem *hBlankSystem;
    struct PoisonEffect *poisonEffect;
    void *localData;
} FieldMapSystems;

typedef struct FollowMonMood {
    u32 personality;
    u16 species;
    u16 isRegistered;
    Pokemon *mon;
} FollowMonMood;

typedef struct FollowMonRule {
    u8 hp;
    u8 mood : 4;
    u8 friendship : 4;
    u8 natureGroup : 3;
    u8 gender : 2;
    u8 status : 3;
    u8 heldItem : 5;
    u8 unused_3 : 3;
    u8 type : 5;
    u8 pokeathlonStat : 3;
    u8 encounterTile;
    u8 speciesAttr;
    u8 shinyLeaf;
    u16 weather : 3;
    u16 unused_8 : 10;
    u16 facing : 3;
    u16 nearbyObjects : 3;
    u16 timeOfDay : 3;
    u16 script : 10;
    u16 mapId;
    u16 metatileBehavior;
    u8 unused_10 : 1;
    u8 level : 2;
    u8 nearbyRock : 2;
    u8 hiddenItems : 3;
    u8 chance;
    u16 flag;
} FollowMonRule;

typedef struct FollowMonInteractionStep {
    u16 movement;
    u16 message;
    u16 sound;
    u8 emote;
    u8 wait;
} FollowMonInteractionStep;

typedef struct FollowMonMovementStep {
    u8 facing;
    u8 duration;
    s8 dx;
    s8 dy;
    s8 dz;
    u8 playSound;
    u8 unk6;
    u8 unk7;
} FollowMonMovementStep;

typedef struct FollowMonInteraction {
    Window window;
    String *string;
    FollowMonRule rules[100];
    FollowMonInteractionStep steps[5];
    u32 askQuestion;
    u16 yesScript;
    u16 noScript;
    s8 friendshipChange;
    s8 moodChange;
    u8 accessory;
    u8 shinyLeaf;
    FollowMonMovementStep movements[10];
    u8 movementState;
    u8 state;
    u8 movementFrame;
    u8 movementIndex;
    u8 stepIndex : 4;
    u8 stepCount : 4;
    u8 waitFrames;
    u16 printerId;
    VecFx32 savedPosition;
    u8 savedFacing;
    u8 form;
    u16 species;
    u16 answer;
    u16 metatileBehavior;
} FollowMonInteraction;

enum MapObjectAnim {
    MAP_OBJECT_ANIM_STAND,
    MAP_OBJECT_ANIM_WALK_SLOWER,
    MAP_OBJECT_ANIM_WALK_SLOW,
    MAP_OBJECT_ANIM_WALK_NORMAL,
    MAP_OBJECT_ANIM_WALK_FAST,
    MAP_OBJECT_ANIM_WALK_FASTER,
    MAP_OBJECT_ANIM_WALK_SLIGHTLY_FAST,
    MAP_OBJECT_ANIM_WALK_SLIGHTLY_FASTER,
    MAP_OBJECT_ANIM_WALK_EVER_SO_SLIGHTLY_FAST,
    MAP_OBJECT_ANIM_RUN,
    MAP_OBJECT_ANIM_JUMP_NEAR_SLOW,
    MAP_OBJECT_ANIM_JUMP_FARTHER,

    MAP_OBJECT_ANIM_NURSE_BOW = MAP_OBJECT_ANIM_RUN,
};

BOOL sub_02061248(FieldSystem *fieldSystem, VecFx32 *position, BOOL allowDynamicHeight);

typedef struct JumpOffsetData {
    u8 angleStep;
    s8 dx;
    s8 dy;
    s8 dz;
    fx32 stepX;
    fx32 stepY;
    fx32 stepZ;
    VecFx32 origin;
} JumpOffsetData;

typedef struct JumpByOffsetWork {
    s8 direction;
    s8 timer;
    u8 step;
    u8 animation;
    fx32 height;
    JumpOffsetData *data;
} JumpByOffsetWork;

BOOL MapObjectMovementCmd108_Step1(LocalMapObject *object);

BOOL MapObjectMovementCmd108_Step1(LocalMapObject *object) {
    VecFx32 position;
    JumpByOffsetWork *work = (JumpByOffsetWork *)sub_0205F3E4(object);
    s64 progress;
    fx32 sine;
    fx32 targetY;
    float rounded;

    if (work->step != 0) {
        rounded = (float)(work->step << 12) + 0.5f;
    } else {
        rounded = (float)(work->step << 12) - 0.5f;
    }
    progress = (int)rounded;
    position.x = work->data->origin.x + FX_MUL(work->data->stepX, progress);
    position.y = 0;
    position.z = work->data->origin.z + FX_MUL(work->data->stepZ, progress);
    if (sub_02061248(MapObject_GetFieldSystem(object), &position, MapObject_CheckFlag29(object))) {
        work->height = position.y;
    } else {
        position.y = work->height;
    }
    MapObject_SetPositionVector(object, &position);
    sine = GF_SinDegNoWrap(work->data->angleStep * work->step);
    targetY = work->data->origin.y + FX_MUL(work->data->stepY, progress);
    targetY += FX_MUL(sine, FX32_CONST(16));
    position.y = targetY - position.y;
    position.x = 0;
    position.z = 0;
    sub_0205F9A0(object, &position);
    work->timer--;
    work->step++;
    if (work->timer > 0) {
        return FALSE;
    }
    position.x = position.y = position.z = 0;
    MapObject_SetFacingVector(object, &position);
    sub_0205F9A0(object, &position);
    MapObject_AddCurrentX(object, work->data->dx);
    MapObject_AddCurrentY(object, work->data->dy);
    MapObject_AddCurrentZ(object, work->data->dz);
    if (work->data->dx << 4 > 0) {
        rounded = (float)(work->data->dx << 16) + 0.5f;
    } else {
        rounded = (float)(work->data->dx << 16) - 0.5f;
    }
    position.x = work->data->origin.x + (int)rounded;
    if (work->data->dy << 4 > 0) {
        rounded = (float)(work->data->dy << 16) + 0.5f;
    } else {
        rounded = (float)(work->data->dy << 16) - 0.5f;
    }
    position.y = work->data->origin.y + (int)rounded;
    if (work->data->dz << 4 > 0) {
        rounded = (float)(work->data->dz << 16) + 0.5f;
    } else {
        rounded = (float)(work->data->dz << 16) - 0.5f;
    }
    position.z = work->data->origin.z + (int)rounded;
    MapObject_SetPositionVector(object, &position);
    sub_02061070(object);
    MapObject_SetFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK17 | MAPOBJECTFLAG_END_MOVEMENT));
    sub_02060F78(object);
    sub_0205F484(object);
    sub_0205F328(object, MAP_OBJECT_ANIM_STAND);
    MapObject_IncrementMovementStep(object);
    if (!MapObject_CheckVisible(object)) {
        PlaySE(SEQ_SE_DP_SUTYA2);
    }
    Heap_Free(work->data);
    return FALSE;
}
