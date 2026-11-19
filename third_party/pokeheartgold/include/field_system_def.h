#ifndef POKEHEARTGOLD_FIELD_SYSTEM_DEF_H
#define POKEHEARTGOLD_FIELD_SYSTEM_DEF_H

// struct FieldSystem and the types it embeds / points to at the top level, with only forward-declared pointee tags:
// small enough for files that carry their own copies of other types (field_system.h pulls in the whole field).

#include "global.h"

#include "menu_input_state.h"

struct AreaDataManager;
struct AreaLightManager;
struct BagCursor;
struct BgConfig;
struct BugContest;
struct Camera;
struct DynamicTerrainHeightManager;
struct Field3dObjectTaskManager;
struct Field3dRenderObjManager;
struct FieldDrawMapNameInfo;
struct FieldEffectManager;
struct FieldSystemUnkSub104;
struct FieldSystemUnkSub108;
struct FieldSystemUnkSub120;
struct FieldSystemUnkSubC8;
struct FieldTextureManager;
struct FieldViewPhoto;
struct FogData;
struct FrontierFieldSystem;
struct GearPhoneRingManager;
struct HBlankSystem;
struct LinkBattleRuleset;
struct LocalMapObject;
struct Location;
struct MapEvents;
struct MapLoadManager;
struct MapLoadMode;
struct MapMatrix;
struct MapObjectManager;
struct MapPropAnimationManager;
struct MapPropManager;
struct MapPropOneShotAnimationManager;
struct ModelAttributes;
struct OverlayManager;
struct PlayerAvatar;
struct SaveData;
struct Signpost;
struct SysTask;
struct TaskManager;
struct TerrainAttributes;
struct TerrainCollisionManager;
struct UnkStruct_020556FC;
struct UnionRoomManager;
struct UnkStruct_0205AC88;
struct UnkStruct_02092BB8;
struct WeatherManager;

#ifndef POKEHEARTGOLD_FIELD_SYSTEM_TYPEDEF // also in field_types_def.h
#define POKEHEARTGOLD_FIELD_SYSTEM_TYPEDEF
typedef struct FieldSystem FieldSystem;
#endif

typedef struct FollowMon {
    struct LocalMapObject *mapObject;
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

typedef struct FieldProcessManager {
    struct OverlayManager *parent;
    struct OverlayManager *child;
    BOOL isPaused;
    BOOL isDone;
} FieldProcessManager;

typedef struct FieldSystemUnkSub4 {
    u32 unk0;
    struct Field3dObjectTaskManager *field3dObjectTaskManager;
    struct FieldDrawMapNameInfo *drawMapNameInfo;
    struct WeatherManager *weatherManager;
    struct FieldTextureManager *textureManager;
    struct PoketchSystem *unk14;
    struct UnkStruct_020556FC *unk18;
    struct HBlankSystem *hBlankSystem;
    void *unk20;
    void *unk24;
} FieldSystemUnkSub4;

#define MAP_OBJECT_PRELOAD_SENTINEL 0xFFFF
#define MAX_MAP_OBJECTS_TO_PRELOAD  24

typedef struct MapObjectsToPreload {
    u16 count;
    int ids[MAX_MAP_OBJECTS_TO_PRELOAD];
} MapObjectsToPreload;

struct FieldSystem {
    struct FieldProcessManager *processManager;
    struct FieldSystemUnkSub4 *unk4;
    struct BgConfig *bgConfig;
    struct SaveData *saveData;
    struct TaskManager *taskman;
    struct MapEvents *mapEvents;
    int bottomScreenType;
    int unk1C;
    struct Location *location;
    struct Camera *camera;
    void *unk28;
    struct MapLoadManager *mapLoadManager;
    struct MapMatrix *mapMatrix;
    struct AreaDataManager *areaDataManager;
    struct MapObjectsToPreload *mapObjectsToPreload;
    struct MapObjectManager *mapObjectManager;
    struct PlayerAvatar *playerAvatar;
    struct FieldEffectManager *fieldEffectManager;
    struct ModelAttributes *modelAttributes;
    struct FogData *fog;
    struct AreaLightManager *areaLightManager;
    struct MapPropAnimationManager *mapPropAnimationManager;
    struct MapPropOneShotAnimationManager *mapPropOneShotAnimationManager;
    struct TerrainAttributes *terrainAttributes;
    const struct TerrainCollisionManager *unk60; // the loaded map's terrain height / tile attribute queries (sub_0205489C)
    BOOL skipMapAttributes;
    struct Signpost *signpost;
    BOOL runningFieldMap;
    int mapLoadType;
    const struct MapLoadMode *mapLoadMode;
    u16 unk78;
    u16 lastFacingDirection;   // 0x7A, used to determine whether the following field should be incremented
    u16 reverseTurnFrameSteps; // 0x7C, turning back and forth in place will increase the effective encounter rate
    u16 encounterInhibitSteps; // 0x7E, don't generate an encounter in the first 4 steps on a new map
    struct UnionRoomManager *unk80;
    struct UnkStruct_0205AC88 *unk84;
    s32 unk88; // ov07_02232A14 sets it to 1, ov07_02232A5C clears it
    void *unk8C; // ov01_021EEF9C reads it as a MsgData *
    int unk90;
    struct BagCursor *bagCursor;
    struct DynamicTerrainHeightManager *dynamicTerrainHeightManager;
    struct MapPropManager *mapPropManager;
    struct FrontierFieldSystem *frontierFsys;
    struct LinkBattleRuleset *linkBattleRuleset;
    struct UnkStruct_02092BB8 *unkA8;
    u32 unkAC;
    void *unkB0;
    s64 unkB4;
    int unkBC;                                 // flags?
    struct Field3dRenderObjManager *renderObjManager; // Field3dRenderObjManager
    int environmentSoundState;                 // Used to keep track of the active weather/soundplate sound and their associated loading status.
    struct FieldSystemUnkSubC8 *unkC8;
    void *unkCC;
    u16 lastTouchMenuInput;
    u8 unkD2_0 : 6;
    u8 textbox_open : 1;
    u8 unkD2_7 : 1;
    u8 unkD3;
    struct SysTask *unk_D4; // touch-save app task (ov01_021F6840)
    struct SysTask *unk_D8;
    struct FieldViewPhoto *viewPhotoTask;
    int lastStartMenuAction;
    FollowMon followMon; // E4
    struct FieldSystemUnkSub104 *unk104;
    struct FieldSystemUnkSub108 *unk108;
    MenuInputStateMgr menuInputState; // Tracks whether the last menu input was touch or keypad
    u8 unk_110;
    u8 unk_111[3];
    struct GearPhoneRingManager *phoneRingManager;
    struct BugContest *bugContest;
    fx32 unk11C;
    struct FieldSystemUnkSub120 *unk120;
    u32 judgeStatPosition;
}; // size: 0x128

#endif // POKEHEARTGOLD_FIELD_SYSTEM_DEF_H
