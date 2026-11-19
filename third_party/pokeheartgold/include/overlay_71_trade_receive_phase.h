#ifndef POKEHEARTGOLD_OVERLAY_71_TRADE_RECEIVE_PHASE_H
#define POKEHEARTGOLD_OVERLAY_71_TRADE_RECEIVE_PHASE_H

// TradeReceivePhase: merged by tools/hg/promote.py from 6 m2c local copies (1 variants, base
// src/overlay_71_02246B58_0224926C_0224AFB8_0224B198.c) and 8 accesses; fields at every accessed offset, pads elsewhere.

#include "bg_window.h"

struct BgConfig;
struct NARC;
struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;
struct PokemonSprite;
struct PokemonSpriteManager;
struct Sprite;
struct String;
struct SysTask;
struct Trade3DModel;
struct Trade3DScene;
struct TradeSequenceData;

typedef struct TradeReceivePhase {
    struct TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
    BOOL fanfarePlaying;
    struct PokemonSpriteManager *spriteManager;
    struct PokemonSprite *pokemonSprite;
    struct { s8 spriteFrame; u8 frameDelay; s8 xOffset; s8 yOffset; } animFrames[10];
    struct Sprite *sprites[2];
    struct { void *cellBuffer; void *animBuffer; struct NNSG2dCellDataBank *cellBank; struct NNSG2dAnimBankData *animBank; } animResources;
    struct BgConfig *bgConfig;
    Window window;
    struct String *string0;
    struct String *string1;
    struct Trade3DScene *scene;
    struct Trade3DModel *model;
    struct SysTask *task0;
    struct SysTask *task1;
    struct NARC *pokeDataNarc;
} TradeReceivePhase;

// sizeof(TradeReceivePhase) == 0x88

#endif // POKEHEARTGOLD_OVERLAY_71_TRADE_RECEIVE_PHASE_H
