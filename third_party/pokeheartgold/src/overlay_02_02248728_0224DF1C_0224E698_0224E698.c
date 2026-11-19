#include "global.h"
#include "constants/heap.h"

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BgConfig;

struct BoxPokemon;

struct BugContest;

struct CAPSULE;

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

struct Mail;

struct MailMessage;

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

struct PartyPokemon;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokedexData;

struct PokegearRingingTask;

struct SEAL;

struct SaveData;

struct SavePokegear;

struct Signpost;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

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

typedef struct FieldSystemUnkSubC8 FieldSystemUnkSubC8;

typedef struct Field3dRenderObjManager Field3dRenderObjManager;

typedef struct FieldSystemUnkSub104 FieldSystemUnkSub104;

typedef struct NARC NARC;

typedef struct MapPropAnimationManager MapPropAnimationManager;

typedef struct MapPropOneShotAnimationManager MapPropOneShotAnimationManager;

typedef struct AreaDataManager AreaDataManager;

typedef struct BgConfig BgConfig;

typedef struct SaveData SaveData;

typedef struct ModelAttributes ModelAttributes;

typedef struct AreaLightManager AreaLightManager;

typedef struct String String;

typedef struct SEAL {
    u8 kind;
    u8 x;
    u8 y;
} SEAL;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef struct {
    u16 species;
    u16 heldItem;

    u32 otID;
    u32 exp;

    u8 friendship;
    u8 ability;
    u8 markings;
    u8 originLanguage;

    u8 hpEV;
    u8 atkEV;
    u8 defEV;
    u8 speedEV;
    u8 spAtkEV;
    u8 spDefEV;

    u8 cool;
    u8 beauty;
    u8 cute;
    u8 smart;
    u8 tough;
    u8 sheen;

    u32 ribbonsDS1;
} PokemonDataBlockA;

typedef struct {
    u16 moves[4];
    u8 moveCurrentPPs[4];
    u8 movePPUps[4];

    u32 hpIV : 5;
    u32 atkIV : 5;
    u32 defIV : 5;
    u32 speedIV : 5;
    u32 spAtkIV : 5;
    u32 spDefIV : 5;
    u32 isEgg : 1;
    u32 hasNickname : 1;

    u32 ribbonsGBA;
    u8 fatefulEncounter : 1;
    u8 gender : 2;
    u8 form : 5;
    u8 HGSS_shinyLeaves : 6;
    u8 unused1 : 2;
    u16 unused2;

    u16 EggLocation_PtHGSS;
    u16 MetLocation_PtHGSS;
} PokemonDataBlockB;

typedef struct {
    u16 nickname[11];

    u8 unused;
    u8 originGame;

    u64 ribbonsDS2;
} PokemonDataBlockC;

typedef struct {
    u16 otName[7 + 1];

    u8 eggYear;
    u8 eggMonth;
    u8 eggDay;

    u8 metYear;
    u8 metMonth;
    u8 metDay;

    u16 EggLocation_DP;
    u16 MetLocation_DP;

    u8 pokerus;
    u8 pokeball;
    u8 metLevel : 7;
    u8 otGender : 1;
    u8 metTerrain;
    u8 HGSS_Pokeball;
    s8 mood;
} PokemonDataBlockD;

typedef union {
    PokemonDataBlockA blockA;
    PokemonDataBlockB blockB;
    PokemonDataBlockC blockC;
    PokemonDataBlockD blockD;
} PokemonDataBlock;

typedef struct BoxPokemon {
    u32 personality;
    u16 partyDecrypted : 1;
    u16 boxDecrypted : 1;
    u16 checksumFailed : 1;
    u16 unused : 13;
    u16 checksum;
    PokemonDataBlock dataBlocks[4];
} BoxPokemon;

union MailPatternData {
    u16 raw;
    struct {
        u16 icon : 12;
        u16 pal : 4;
    };
};

typedef struct MailMessage {
    u16 msg_bank;
    u16 msg_no;
    u16 fields[(2)];
} MailMessage;

typedef struct Mail {
    u32 author_otId;
    u8 author_gender;
    u8 author_language;
    u8 author_version;
    u8 mail_type;
    u16 author_name[7 + 1];
    union MailPatternData mon_icons[3];
    u16 form_flags;
    MailMessage unk_20[3];
} Mail;

typedef struct PartyPokemon {
    u32 status;
    u8 level;
    u8 ballCapsuleID;
    u16 hp;
    u16 maxHP;
    u16 atk;
    u16 def;
    u16 speed;
    u16 spatk;
    u16 spdef;
    Mail mail;
    CAPSULE sealCoords;
} PartyPokemon;

typedef struct {
    BoxPokemon box;
    PartyPokemon party;
} Pokemon;

typedef struct DynamicTerrainHeightManager DynamicTerrainHeightManager;

typedef struct Location Location;

typedef struct FieldSystem FieldSystem;

typedef struct TaskManager TaskManager;

typedef struct MapLoadManager MapLoadManager;

typedef struct LocalMapObject LocalMapObject;

typedef struct PlayerAvatar PlayerAvatar;

typedef struct MapObjectManager MapObjectManager;

typedef struct GearPhoneRingManager GearPhoneRingManager;

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
    u8 unkC;
    u8 rematchWeekday;
    u8 rematchTimeOfDay;
    u8 unkF;
    u8 sortParam[4];
} PhoneBookEntry;

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

u32 PlayerAvatar_GetFacingDirection(PlayerAvatar *avatar);

u32 PlayerAvatar_GetXCoord(PlayerAvatar *avatar);

u32 PlayerAvatar_GetZCoord(PlayerAvatar *avatar);

void PlayerAvatar_CopyPositionVector(PlayerAvatar *avatar, VecFx32 *vector);

s32 PlayerAvatar_GetState(PlayerAvatar *avatar);

typedef struct PhoneCallPersistentState PhoneCallPersistentState;

typedef struct SavePokegear SavePokegear;

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

typedef struct FieldEffectManager FieldEffectManager;

typedef struct FieldViewPhoto FieldViewPhoto;

typedef struct FrontierFieldSystem FrontierFieldSystem;

typedef struct TerrainAttributes TerrainAttributes;

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

extern void *Save_SafariZone_Get(SaveData *saveData);

extern void *SafariZone_GetAreaSet(void *safari_zone, int area_set_no);

extern int ov02_0224EE4C(void *a0, s16 a1, s16 a2, u8 a3, fx32 a4, u16 *a5, u16 *a6, u8 a7);

BOOL ov02_0224E640(SaveData *saveData);

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void ov02_0224E074(FieldSystem *fieldSystem, u16 *p_ret, int type, enum HeapID heapID);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

int ov02_0224E698(FieldSystem *fieldSystem);

int ov02_0224E698(FieldSystem *fieldSystem) {
    u8 facing = (u8)PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    s16 x = (s16)PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    s16 z = (s16)PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    u16 buf[2];
    VecFx32 pos;
    void *areaSet;
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &pos);
    areaSet = SafariZone_GetAreaSet(Save_SafariZone_Get(fieldSystem->saveData), 0);
    if (*(u8 *)((u8 *)areaSet + (u8)((x - 0x20) / 0x20 + (z - 0x20) / 0x20 * 3) * 0x7a + 1) >= 0x1e) {
        return 1;
    }
    if (PlayerAvatar_GetState(fieldSystem->playerAvatar) != 2) {
        return 0;
    }
    if (ov02_0224E640(fieldSystem->saveData) == 0) {
        return 2;
    }
    if (ov02_0224EE4C(fieldSystem, x, z, facing, pos.y, &buf[1], &buf[0], 1) != 0) {
        return 0;
    }
    return 3;
}
