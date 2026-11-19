#ifndef POKEHEARTGOLD_OVERLAY_07_FADE_BG_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_FADE_BG_CONTEXT_H

#include "palette.h"

struct BattleAnimSystem;

// sizeof == 0x8 on the DS
typedef struct FadeBgContext {
    struct BattleAnimSystem *battleAnimSys;
    PaletteData *paletteData;
} FadeBgContext;

#endif // POKEHEARTGOLD_OVERLAY_07_FADE_BG_CONTEXT_H
