#include "global.h"

#include "legendary_cinematic_task.h"
#include "constants/heap.h"

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

void *Heap_AllocAtEnd(enum HeapID heapID, u32 size);

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

typedef struct BgConfig BgConfig;

typedef struct NARC NARC;

typedef struct SaveData SaveData;

typedef struct BugContest BugContest;

typedef struct Camera Camera;

typedef struct Location Location;

typedef struct FieldSystem FieldSystem;

typedef struct TaskManager TaskManager;

typedef struct MapLoadManager MapLoadManager;

typedef struct LocalMapObject LocalMapObject;

typedef struct PlayerAvatar PlayerAvatar;

typedef struct MapObjectManager MapObjectManager;

typedef struct GearPhoneRingManager GearPhoneRingManager;

typedef struct PhoneBookEntry {
    u8 id;
    u8 type;
    u8 unk2;
    u8 trainerClass;
    u16 trainerId;
    u16 mapId;
    u16 gift;
    u16 phoneScriptIfLocal;
    u8 unkC;
    u8 rematchWeekday;
    u8 rematchTimeOfDay;
    u8 unkF;
    u8 sortParam[4];
} PhoneBookEntry;

typedef enum RTC_TimeOfDay {
    RTC_TIMEOFDAY_MORN = 0,
    RTC_TIMEOFDAY_DAY,
    RTC_TIMEOFDAY_EVE,
    RTC_TIMEOFDAY_NITE,
    RTC_TIMEOFDAY_LATE,
    RTC_TIMEOFDAY_COUNT,
} TIMEOFDAY;

typedef struct MapEvents MapEvents;

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
    u32 moveState;
    u32 playerMoveState;
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

typedef struct PhoneCallPersistentState PhoneCallPersistentState;

typedef struct SavePokegear SavePokegear;

typedef struct FieldSystemUnkSubC8 FieldSystemUnkSubC8;

typedef struct Field3dRenderObjManager Field3dRenderObjManager;

typedef struct FieldSystemUnkSub104 FieldSystemUnkSub104;

typedef struct MapPropAnimationManager MapPropAnimationManager;

typedef struct MapPropOneShotAnimationManager MapPropOneShotAnimationManager;

typedef struct AreaDataManager AreaDataManager;

typedef struct AreaLightManager AreaLightManager;

typedef struct DynamicTerrainHeightManager DynamicTerrainHeightManager;

typedef struct MapPropManager MapPropManager;

typedef struct Signpost Signpost;

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

typedef struct LinkBattleRuleset LinkBattleRuleset;

typedef struct MapMatrix MapMatrix;

typedef enum MenuInputState {
    MENU_INPUT_STATE_BUTTONS,
    MENU_INPUT_STATE_TOUCH,
} MenuInputState;

typedef struct MenuInputStateMgr {
    MenuInputState state;
} MenuInputStateMgr;

typedef struct FogData FogData;

struct MapObjectManager {
    u32 flags;
    u32 objectCount;
    u32 unk8;
    u32 priority;
    u8 unk10[4];
    NARC *mapModelNarc;
    u32 unk18;
    u8 unk1C[0x108];
    LocalMapObject *objects;
    FieldSystem *fieldSystem;
};

typedef void (*LocalMapObject_UnkCallback)(LocalMapObject *object);

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
    VecFx32 unk88;
    VecFx32 unk94;
    u32 unkA0;
    u32 movementCmd;
    u32 movementStep;
    u16 unkAC;
    u16 unkAE;
    SysTask *unkB0;
    MapObjectManager *manager;
    LocalMapObject_UnkCallback unkB8;
    LocalMapObject_UnkCallback unkBC;
    LocalMapObject_UnkCallback unkC0;
    LocalMapObject_UnkCallback unkC4;
    LocalMapObject_UnkCallback unkC8;
    LocalMapObject_UnkCallback unkCC;
    LocalMapObject_UnkCallback unkD0;
    LocalMapObject_UnkCallback unkD4;
    u8 unkD8[0x10];
    u8 unkE8[0x10];
    u8 unkF8[0x10];
    u8 unk108[0x20];
    u16 unk128;
};

typedef BOOL (*TaskFunc)(TaskManager *taskman);

typedef struct TaskManagerUnkSub1C TaskManagerUnkSub1C;

struct TaskManager {
    TaskManager *prev;
    TaskFunc func;
    u32 state;
    void *env;
    u32 unk10;
    void *unk14;
    FieldSystem *fieldSystem;
    TaskManagerUnkSub1C *unk1C;
};

void TaskManager_Call(TaskManager *taskman, TaskFunc taskFunc, void *env);

typedef struct FieldEffectManager FieldEffectManager;

typedef struct FieldViewPhoto FieldViewPhoto;

typedef struct FrontierFieldSystem FrontierFieldSystem;

typedef struct TerrainAttributes TerrainAttributes;

TIMEOFDAY Field_GetTimeOfDay(FieldSystem *fieldSystem);

typedef struct UnkStruct_02092BB8 UnkStruct_02092BB8;

typedef struct FollowMon {
    LocalMapObject *mapObject;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 species;
    u8 gender;
    u8 unk15;
    u8 active;
    u8 shiny;
    u16 form;
    u16 dummy;
    u32 unk1C;
} FollowMon;

struct GearPhoneRingManager {
    u8 active : 1;
    u8 unk_var0_1 : 1;
    u8 unk_var0_2 : 1;
    u8 ringing : 1;
    u8 unk_var0_4 : 4;
    u8 unk_var1;
    u8 callerId;
    u8 isScriptedCall;
    u8 callScriptID;
    u8 unk_arr5[2];
    u8 callTriggerID;
    s32 unk_var8;
    s32 unk_varC;
    u16 maxActiveDuration;
    u16 unk_var12;
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

typedef struct FieldSystemUnkSub4 FieldSystemUnkSub4;

typedef struct MapObjectsToPreload MapObjectsToPreload;

typedef struct FieldSystemUnkSub108 FieldSystemUnkSub108;

typedef struct FieldSystemUnkSub120 FieldSystemUnkSub120;

struct FieldSystem {
    FieldProcessManager *processManager;
    FieldSystemUnkSub4 *unk4;
    BgConfig *bgConfig;
    SaveData *saveData;
    TaskManager *taskman;
    MapEvents *mapEvents;
    int bottomScreenType;
    int unk1C;
    Location *location;
    Camera *camera;
    void *unk28;
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
    void *unk60;
    BOOL skipMapAttributes;
    Signpost *signpost;
    BOOL runningFieldMap;
    int mapLoadType;
    const struct MapLoadMode *mapLoadMode;
    u16 unk78;
    u16 lastFacingDirection;
    u16 reverseTurnFrameSteps;
    u16 encounterInhibitSteps;
    struct UnionRoomManager *unk80;
    struct UnkStruct_0205AC88 *unk84;
    u8 filler_88[0x8];
    int unk90;
    BagCursor *bagCursor;
    DynamicTerrainHeightManager *dynamicTerrainHeightManager;
    MapPropManager *mapPropManager;
    FrontierFieldSystem *frontierFsys;
    LinkBattleRuleset *linkBattleRuleset;
    UnkStruct_02092BB8 *unkA8;
    u32 unkAC;
    void *unkB0;
    s64 unkB4;
    int unkBC;
    Field3dRenderObjManager *renderObjManager;
    int environmentSoundState;
    FieldSystemUnkSubC8 *unkC8;
    void *unkCC;
    u16 lastTouchMenuInput;
    u8 unkD2_0 : 6;
    u8 textbox_open : 1;
    u8 unkD2_7 : 1;
    u8 unkD3;
    u8 filler_D4[0x4];
    SysTask *unk_D8;
    FieldViewPhoto *viewPhotoTask;
    int lastStartMenuAction;
    FollowMon followMon;
    FieldSystemUnkSub104 *unk104;
    FieldSystemUnkSub108 *unk108;
    MenuInputStateMgr menuInputState;
    u8 unk_110;
    u8 unk_111[3];
    GearPhoneRingManager *phoneRingManager;
    BugContest *bugContest;
    fx32 unk11C;
    FieldSystemUnkSub120 *unk120;
    u32 judgeStatPosition;
};

BOOL sub_02097C50(TaskManager *taskMan);

void sub_02097BE0(FieldSystem *fieldSystem, u16 scene);

void sub_02097BE0(FieldSystem *fieldSystem, u16 scene) {
    LegendaryCinematicTask *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(LegendaryCinematicTask));

    env->unk_00 = *fieldSystem->modelAttributes;
    env->unk_48 = scene;
    env->state = 0;
    switch (Field_GetTimeOfDay(fieldSystem)) {
    case 0:
    case 1:
        env->unk_4c = 0;
        break;
    case 2:
        env->unk_4c = 1;
        break;
    case 3:
    case 4:
        env->unk_4c = 2;
        break;
    default:
        GF_AssertFail();
        env->unk_4c = 1;
        break;
    }
    TaskManager_Call(fieldSystem->taskman, sub_02097C50, env);
}
