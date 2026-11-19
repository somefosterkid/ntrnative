#ifndef POKEHEARTGOLD_OVERLAY_71_BG_SLIDE_STATE_H
#define POKEHEARTGOLD_OVERLAY_71_BG_SLIDE_STATE_H

#include "global.h"

struct BgConfig;
struct SysTask;
struct TradePokemonRevealPhase;

typedef struct BgSlideState {
    struct TradePokemonRevealPhase *phase;
    struct BgConfig *bgConfig;
    int framesRemaining;
    fx32 mainBgOffset;
    fx32 subBgOffset;
    fx32 offsetStep;
    fx32 unused_18;
    struct SysTask **taskHandle;
    struct SysTask *unused_20;
} BgSlideState;

#endif // POKEHEARTGOLD_OVERLAY_71_BG_SLIDE_STATE_H
