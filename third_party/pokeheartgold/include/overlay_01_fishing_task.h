#ifndef POKEHEARTGOLD_OVERLAY_01_FISHING_TASK_H
#define POKEHEARTGOLD_OVERLAY_01_FISHING_TASK_H

// FishingTask: merged by tools/hg/promote.py from 10 m2c local copies (1 variants, base
// src/overlay_01_021FC66C_021FC748.c) and 48 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct FieldSystem;
struct MsgData;
struct OverworldAnimManager;
struct String;
struct StringTemplate;

typedef struct FishingTask {
    BOOL isFishEncountered;
    BOOL doneFishing;
    BOOL caughtFish;
    u32 fishingTask;
    int counter;
    int fishDelayCounter;
    int fishHookedCounter;
    u32 rodType;
    struct FieldSystem *fieldSystem;
    struct OverworldAnimManager *unk_24;
    u8 printerID;
    u8 pad_29[0x3];
    struct String *formattedFishingMessage;
    struct String *fishingMessage;
    struct StringTemplate *strTemplate;
    struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } window;
    struct MsgData *messageLoader;
} FishingTask;

// sizeof(FishingTask) == 0x4c

struct BattleSetup;
struct SysTask;

// The environment of Task_OverworldFish, allocated by CreateFishingRodTaskEnv. The pointer fields (battleSetup at DS 0x10,
// sysTask at DS 0x14) are read by field: raw +0x10 / +0x14 collide on the host. sizeof == 0x18 on the DS.
typedef struct FishingContext {
    int state;
    u32 fishingRodItemID;
    BOOL isFishEncountered;
    int rodType; // enum EncounterFishingRodType
    struct BattleSetup *battleSetup;
    struct SysTask *sysTask;
} FishingContext;

#endif // POKEHEARTGOLD_OVERLAY_01_FISHING_TASK_H
