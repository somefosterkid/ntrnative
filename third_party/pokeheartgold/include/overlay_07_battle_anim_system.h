#ifndef POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_SYSTEM_H
#define POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_SYSTEM_H

#include "filesystem_files_def.h"
#include "managed_sprite_template.h"

// BattleAnimSystem: merged by tools/hg/promote.py from 73 m2c local copies (5 variants, base
// src/overlay_07_0221BE20.c) and 33 accesses; fields at every accessed offset, pads elsewhere.

struct BattleAnimContext;
struct BattleBgAnim;
struct BgConfig;
struct BgTrackingTask;
struct ManagedSprite;
struct NARC;
struct PaletteData;
struct SpriteManager;
struct SpriteTrackingTask;

// What the sys task ov07_0221DAD0 gets (BattleAnimSystem.pokemonSpriteDrawContexts[slot]): draws the Pokemon sprites while active.
typedef struct BattleAnimPokemonDrawContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *spriteManager;
    struct ManagedSprite *sprite;
    BOOL active;
} BattleAnimPokemonDrawContext;

typedef struct BattleAnimSystem {
    u32 heapID;
    u32 moveArcID;
    BOOL isContest;
    BOOL isActive;
    BOOL moveActive;
    void *scriptData;
    u32 *scriptPtr;
    u32 *callStack[3];
    struct { u32 *startPos; u8 curIteration; u8 maxIterations; u8 unk_8_pad[0x2]; BOOL isActive; } loopStack[3];
    BattleAnimPokemonDrawContext pokemonSpriteDrawContexts[2];
    u8 cameraProjections[16];
    u8 particleSystemCameraFlip[16];
    s8 unused_88;
    u8 scriptDelay;
    u16 activeAnimTasks;
    u16 activeSoundTasks;
    u8 pad_92[0x2];
    s32 scriptVars[10];
    void (*executeAnimScriptFunc)(struct BattleAnimSystem *);
    struct BattleAnimContext *context;
    struct BgConfig *bgConfig;
    struct PaletteData *paletteData;
    struct SpriteManager *spriteManagers[4];
    struct ManagedSprite *sprites[10];
    ManagedSpriteTemplate lastSpriteTemplate;
    struct SpriteManager *pokemonSpriteManager;
    struct ManagedSprite *pokemonSprites[5];
    int usedPokemonSprites[5];
    struct SpriteTrackingTask *spriteTrackingTasks[5];
    struct BgTrackingTask *bgTrackingTask;
    u8 bgSwitchState;
    u8 soundEffectWaitTimer;
    u8 endDelayFrames;
    u8 unused_17B;
    struct BattleBgAnim *bgAnim[4];
    struct { NarcId narcID; int tilesNarcMemberIdx; int paletteNarcMemberIdx; int tilemapNarcMemberIdx; int paletteDestStart; int paletteSrcSize; } battleBgRefs;
    int baseBgPalettes;
    u8 bgLayerPriorities[4];
    struct NARC *arcs[6];
} BattleAnimSystem;

// sizeof(BattleAnimSystem) == 0x1c8

#endif // POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_SYSTEM_H
