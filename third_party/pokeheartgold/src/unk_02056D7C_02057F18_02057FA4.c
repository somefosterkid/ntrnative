#include "global.h"

#include "comm_player_manager.h"
#include "constants/heap.h"

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

typedef struct NARC NARC;

typedef struct SysTask SysTask;

typedef struct SysTaskQueue SysTaskQueue;

typedef void (*SysTaskFunc)(SysTask *task, void *data);

struct SysTask {
    SysTaskQueue *queue;
    SysTask *prev;
    SysTask *next;
    u32 priority;
    void *data;
    SysTaskFunc func;
    u32 runNow;
};

struct SysTaskQueue {
    u16 limit;
    u16 activeCount;
    SysTask headSentinel;
    SysTask **taskStack;
    SysTask *taskList;
    BOOL isInsertingTask;
    SysTask *runningTask;
    SysTask *nextTask;
};

typedef struct Location Location;

typedef struct FieldSystem FieldSystem;

typedef struct TaskManager TaskManager;

typedef struct MapLoadManager MapLoadManager;

typedef struct LocalMapObject LocalMapObject;

typedef struct PlayerAvatar PlayerAvatar;

typedef struct MapObjectManager MapObjectManager;

typedef struct GearPhoneRingManager GearPhoneRingManager;

enum PlayerMoveState {
    PLAYER_MOVE_STATE_NONE = 0,
    PLAYER_MOVE_STATE_START,
    PLAYER_MOVE_STATE_MOVING,
    PLAYER_MOVE_STATE_END,
};

enum AvatarMoveState {
    AVATAR_MOVE_STATE_NONE = 0,
    AVATAR_MOVE_STATE_MOVING,
    AVATAR_MOVE_STATE_TURNING,
};

typedef struct SaveData SaveData;

typedef enum PlayerAvatarFlags {
    AVATAR_FLAG_FORCED_MOVEMENT = (1 << 0),
    AVATAR_FLAG_UNK1 = (1 << 1),
    AVATAR_FLAG_UNK2 = (1 << 2),
    AVATAR_FLAG_LOCK_BIKE_STATE = (1 << 3),
    AVATAR_FLAG_UNK4 = (1 << 4),
    AVATAR_FLAG_UNK5 = (1 << 5),
    AVATAR_FLAG_UNK6 = (1 << 6),
    AVATAR_FLAG_UNK7 = (1 << 7),
} PlayerAvatarFlags;

typedef struct PlayerSaveData PlayerSaveData;

struct PlayerAvatar {
    PlayerAvatarFlags flags;
    u32 transitionFlags;
    u32 unk8;
    BOOL unkC;
    enum AvatarMoveState moveState;
    enum PlayerMoveState playerMoveState;
    s32 state;
    u32 gender;
    u32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    LocalMapObject *mapObject;
    u32 unk34;
    PlayerSaveData *playerSaveData;
    u32 unk3c;
};

LocalMapObject *PlayerAvatar_GetMapObject(PlayerAvatar *avatar);

typedef struct BgConfig BgConfig;

typedef enum MenuInputState {
    MENU_INPUT_STATE_BUTTONS,
    MENU_INPUT_STATE_TOUCH,
} MenuInputState;

typedef struct MenuInputStateMgr {
    MenuInputState state;
} MenuInputStateMgr;

typedef struct LinkBattleRuleset LinkBattleRuleset;

typedef struct AreaMapAnimation AreaMapAnimation;

typedef struct MapPropAnimationPoolList MapPropAnimationPoolList;

typedef struct Field3dRenderObjManager Field3dRenderObjManager;

typedef struct TimeOfDayPropAnimManager TimeOfDayPropAnimManager;

typedef struct MapPropAnimationManager MapPropAnimationManager;

typedef struct MapPropOneShotAnimationManager MapPropOneShotAnimationManager;

typedef struct AreaDataManager AreaDataManager;

typedef struct ModelAttributes ModelAttributes;

typedef struct AreaLightManager AreaLightManager;

typedef struct DynamicTerrainHeightManager DynamicTerrainHeightManager;

typedef struct MapPropManager MapPropManager;

typedef struct BugContest BugContest;

typedef struct Camera Camera;

typedef struct PhoneBookEntry {
    u8 id;
    u8 type;
    u8 unk2;
    u8 trainerClass;
    u16 trainerId;
    u16 mapId;
    u16 gift;
    u16 phoneScriptIfLocal;
    u8 greetingMsgSet;
    u8 rematchWeekday;
    u8 rematchTimeOfDay;
    u8 randomCallGroup;
    u8 sortParam[4];
} PhoneBookEntry;

typedef struct MapEvents MapEvents;

typedef struct PhoneCallPersistentState PhoneCallPersistentState;

typedef struct SavePokegear SavePokegear;

typedef struct Signpost Signpost;

typedef struct FogData FogData;

typedef struct BagCursor BagCursor;

typedef struct PokedexData {
    u32 *height;
    u32 *weight;
    s16 *playerYPos;
    s16 *monYPos;
    s16 *playerScale;
    s16 *monScale;
} PokedexData;

PokedexData *PokedexData_Create(enum HeapID heapID);

void PokedexData_Delete(PokedexData *pokedexData);

void PokedexData_LoadAll(PokedexData *pokedexData, int playerGender, enum HeapID heapID);

void PokedexData_UnloadAll(PokedexData *pokedexData);

u32 PokedexData_GetHeight(PokedexData *pokedexData, int species);

u32 PokedexData_GetWeight(PokedexData *pokedexData, int species);

static inline u32 SpeciesGetDexHeight(int species, enum HeapID heapID) {
    u32 ret;
    PokedexData *pokedexData;

    pokedexData = PokedexData_Create(heapID);
    PokedexData_LoadAll(pokedexData, 0, heapID);
    ret = PokedexData_GetHeight(pokedexData, species);
    PokedexData_UnloadAll(pokedexData);
    PokedexData_Delete(pokedexData);

    return ret;
}

static inline u32 SpeciesGetDexWeight(int species, enum HeapID heapID) {
    u32 ret;
    PokedexData *pokedexData;

    pokedexData = PokedexData_Create(heapID);
    PokedexData_LoadAll(pokedexData, 0, heapID);
    ret = PokedexData_GetWeight(pokedexData, species);
    PokedexData_UnloadAll(pokedexData);
    PokedexData_Delete(pokedexData);

    return ret;
}

typedef struct MapMatrix MapMatrix;

struct MapObjectManager {
    u32 flags;
    u32 objectCount;
    u32 activeCount;
    u32 priority;
    u8 unk10[4];
    NARC *mapModelNarc;
    u32 render;
    u8 unk1C[0x108];
    LocalMapObject *objects;
    FieldSystem *fieldSystem;
};

typedef void (*LocalMapObject_Callback)(LocalMapObject *object);

struct LocalMapObject {
    u32 flags;
    u32 flags2;
    u32 id;
    u32 mapId;
    u32 spriteId;
    u32 movement;
    u32 type;
    u32 eventFlag;
    u32 scriptId;
    u32 initialFacing;
    u32 currentFacing;
    u32 nextFacing;
    u32 previousFacing;
    u32 nextFacingBackup;
    s32 param[3];
    s32 xRange;
    s32 yRange;
    u32 initialX;
    u32 initialY;
    u32 initialZ;
    u32 previousX;
    u32 previousY;
    u32 previousZ;
    u32 currentX;
    s32 currentY;
    u32 currentZ;
    VecFx32 positionVector;
    VecFx32 facingVector;
    VecFx32 drawOffset;
    VecFx32 tileDrawOffset;
    u32 animation;
    u32 movementCmd;
    u32 movementStep;
    u16 currentBehavior;
    u16 previousBehavior;
    SysTask *updateTask;
    MapObjectManager *manager;
    LocalMapObject_Callback movementInit;
    LocalMapObject_Callback movementUpdate;
    LocalMapObject_Callback movementDelete;
    LocalMapObject_Callback drawInit;
    LocalMapObject_Callback drawUpdate;
    LocalMapObject_Callback drawDelete;
    LocalMapObject_Callback drawPause;
    LocalMapObject_Callback drawResume;
    u8 movementData[0x10];
    u8 behaviorData[0x10];
    u8 movementCmdData[0x10];
    u8 drawData[0x20];
    u16 savedPreviousBehavior;
};

void MapObject_SetCurrentX(LocalMapObject *object, u32 currentX);

void MapObject_SetCurrentZ(LocalMapObject *object, u32 currentZ);

void MapObject_SetPositionFromXYZAndDirection(LocalMapObject *object, u32 x, u32 y, u32 z, u32 direction);

typedef BOOL (*TaskFunc)(TaskManager *taskman);

typedef struct TaskManagerBgGoTo TaskManagerBgGoTo;

struct TaskManager {
    TaskManager *prev;
    TaskFunc func;
    u32 state;
    void *env;
    u32 unk10;
    void *ownedData;
    FieldSystem *fieldSystem;
    TaskManagerBgGoTo *bgGoTo;
};

typedef struct FieldEffectManager FieldEffectManager;

typedef struct FieldEffect FieldEffect;

typedef struct FieldViewPhoto FieldViewPhoto;

typedef struct FrontierFieldSystem FrontierFieldSystem;

typedef struct TerrainAttributes TerrainAttributes;

typedef struct PokedexResumeState PokedexResumeState;

typedef struct FollowMon {
    LocalMapObject *mapObject;
    u32 queuedMovement;
    u32 queuedX;
    u32 queuedZ;
    u32 species;
    u8 gender;
    u8 recreatePending;
    u8 active;
    u8 shiny;
    u16 form;
    u16 dummy;
    u32 queuedStepState;
} FollowMon;

struct GearPhoneRingManager {
    u8 active : 1;
    u8 unk_var0_1 : 1;
    u8 receivedFirstMinutes : 1;
    u8 ringing : 1;
    u8 unk_var0_4 : 4;
    u8 unk_var1;
    u8 callerId;
    u8 isScriptedCall;
    u8 callScriptID;
    u8 callerParams[2];
    u8 callTriggerID;
    s32 elapsedMinutes;
    s32 callIntervalMinutes;
    u16 maxActiveDuration;
    u16 randomCallChance;
    s64 activeTimer;
    PhoneBookEntry entry;
    SavePokegear *pokegearData;
    PhoneCallPersistentState *savingsData;
    SaveData *saveData;
    FieldSystem *sys;
    struct PokegearRingingTask {
        SysTask *task;
        u8 counter;
    } gearRing;
};

typedef struct FieldProcessManager FieldProcessManager;

typedef struct FieldMapSystems FieldMapSystems;

typedef struct MapObjectsToPreload MapObjectsToPreload;

typedef struct FollowMonMood FollowMonMood;

typedef struct FollowMonInteraction FollowMonInteraction;

struct FieldSystem {
    FieldProcessManager *processManager;
    FieldMapSystems *unk4;
    BgConfig *bgConfig;
    SaveData *saveData;
    TaskManager *taskman;
    MapEvents *mapEvents;
    int bottomScreenType;
    int unk1C;
    Location *location;
    Camera *camera;
    struct FieldCameraShift *unk28;
    MapLoadManager *mapLoadManager;
    MapMatrix *mapMatrix;
    AreaDataManager *areaDataManager;
    MapObjectsToPreload *mapObjectsToPreload;
    MapObjectManager *mapObjectManager;
    PlayerAvatar *playerAvatar;
    FieldEffectManager *fieldEffectManager;
    ModelAttributes *modelAttributes;
    FogData *fog;
    AreaLightManager *areaLightManager;
    MapPropAnimationManager *mapPropAnimationManager;
    MapPropOneShotAnimationManager *mapPropOneShotAnimationManager;
    TerrainAttributes *terrainAttributes;
    const struct FieldTerrainQueries *unk60;
    BOOL skipMapAttributes;
    Signpost *signpost;
    BOOL runningFieldMap;
    int mapLoadType;
    const struct MapLoadMode *mapLoadMode;
    u16 unk78;
    u16 lastFacingDirection;
    u16 reverseTurnFrameSteps;
    u16 encounterInhibitSteps;
    struct UnionRoom *unk80;
    struct UnionRoomObjects *unk84;
    u8 filler_88[0x8];
    int unk90;
    BagCursor *bagCursor;
    DynamicTerrainHeightManager *dynamicTerrainHeightManager;
    MapPropManager *mapPropManager;
    FrontierFieldSystem *frontierFsys;
    LinkBattleRuleset *linkBattleRuleset;
    PokedexResumeState *unkA8;
    u32 unkAC;
    void *unkB0;
    s64 unkB4;
    int unkBC;
    Field3dRenderObjManager *renderObjManager;
    int environmentSoundState;
    MapPropAnimationPoolList *unkC8;
    AreaMapAnimation *unkCC;
    u16 lastTouchMenuInput;
    u8 unkD2_0 : 6;
    u8 textbox_open : 1;
    u8 unkD2_7 : 1;
    u8 unkD3;
    u8 filler_D4[4];
    SysTask *unk_D8;
    FieldViewPhoto *viewPhotoTask;
    int lastStartMenuAction;
    FollowMon followMon;
    TimeOfDayPropAnimManager *unk104;
    FollowMonMood *unk108;
    MenuInputStateMgr menuInputState;
    u8 unk_110;
    u8 unk_111[3];
    GearPhoneRingManager *phoneRingManager;
    BugContest *bugContest;
    fx32 unk11C;
    FollowMonInteraction *unk120;
    u32 judgeStatPosition;
};

u16 sub_0203769C(void);

void CommPlayerManager_ForcePosition(void);

void CommPlayerManager_ForcePosition(void) {
    int i;
    if (_021D41C4 == ((void *)0)) {
        return;
    }
    for (i = 0; i < 8; i++) {
        LocalMapObject *object;
        u32 x;
        u32 z;
        u32 facing;

        if (_021D41C4->avatars[i] != ((void *)0) && i != sub_0203769C()) {
            object = PlayerAvatar_GetMapObject(_021D41C4->avatars[i]);
            x = _021D41C4->positions[i].x;
            z = _021D41C4->positions[i].z;
            facing = _021D41C4->positions[i].facing;
            MapObject_SetCurrentX(object, x);
            MapObject_SetCurrentZ(object, z);
            MapObject_SetPositionFromXYZAndDirection(object, x, 0, z, facing);
        }
    }
}
