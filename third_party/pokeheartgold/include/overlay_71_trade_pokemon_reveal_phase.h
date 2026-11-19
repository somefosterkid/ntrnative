#ifndef POKEHEARTGOLD_OVERLAY_71_TRADE_POKEMON_REVEAL_PHASE_H
#define POKEHEARTGOLD_OVERLAY_71_TRADE_POKEMON_REVEAL_PHASE_H

// TradePokemonRevealPhase: merged by tools/hg/promote.py from 4 m2c local copies (1 variants, base
// src/overlay_71_02246B58_0224926C_0224A98C_0224A98C.c) and 9 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct BgScaleAnimation;
struct PaletteShimmerEffect;
struct SysTask;
struct Trade3DModel;
struct Trade3DScene;
struct TradeSequenceData;

typedef struct TradePokemonRevealPhase {
    struct TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
    struct BgConfig *bgConfig;
    struct PaletteShimmerEffect *shimmerEffect;
    BOOL shimmerActive;
    struct BgScaleAnimation *bgScaleAnim;
    BOOL bgScaleAnimDone;
    struct Trade3DScene *scene;
    struct Trade3DModel *model;
    struct { fx16 x; fx16 y; fx16 z; } modelRotation;
    u8 pad_2E[0x2];
    struct SysTask *task0;
    struct SysTask *task1;
    struct SysTask *task2;
} TradePokemonRevealPhase;

// sizeof(TradePokemonRevealPhase) == 0x3c

#endif // POKEHEARTGOLD_OVERLAY_71_TRADE_POKEMON_REVEAL_PHASE_H
