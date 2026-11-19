#ifndef POKEHEARTGOLD_OVERLAY_07_SPITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_SPITE_CONTEXT_H

// SpiteContext: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_07_0222C870_0222D050_0222D050.c) and 11 accesses; fields at every accessed offset, pads elsewhere.

#include "sys_task.h"

#include "overlay_07_alpha_fade_context.h"

struct BattleAnimSystem;
struct BgScrollContext;
struct ManagedSprite;
struct PaletteData;
struct Pokepic;

typedef struct SpiteContext {
    struct BattleAnimSystem *battleAnimSys;
    int state;
    int subState;
    int unused;
    struct Pokepic *defenderSprite;
    struct BgScrollContext *bgScroll;
    struct PaletteData *paletteData;
    AlphaFadeContext alpha;
    int delay;
    struct ManagedSprite *monSprite;
} SpiteContext;

// sizeof(SpiteContext) == 0x4c

void ov07_0222D050(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_SPITE_CONTEXT_H
