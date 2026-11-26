#ifndef POKEHEARTGOLD_OVERLAY_71_TRADE_SEND_PHASE_H
#define POKEHEARTGOLD_OVERLAY_71_TRADE_SEND_PHASE_H

#include "bg_window.h"

// TradeSendPhase: merged by tools/hg/promote.py from 5 m2c local copies (1 variants, base
// src/overlay_71_02246B58_02247A10_02247A10.c) and 8 accesses; fields at every accessed offset, pads elsewhere.

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

typedef struct TradeSendPhase {
    struct TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
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
} TradeSendPhase;

// sizeof(TradeSendPhase) == 0x84

#endif // POKEHEARTGOLD_OVERLAY_71_TRADE_SEND_PHASE_H
