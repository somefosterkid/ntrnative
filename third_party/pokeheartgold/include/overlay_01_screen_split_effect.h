#ifndef POKEHEARTGOLD_OVERLAY_01_SCREEN_SPLIT_EFFECT_H
#define POKEHEARTGOLD_OVERLAY_01_SCREEN_SPLIT_EFFECT_H

#include "global.h"
#include "quadratic_interpolation_task.h"

struct HBlankSystem;
struct HBlankTask;
struct SysTask;

// ScreenSplitEffect: the battle-intro "screen split" window effect (ov01_021F0250 allocates it, ov01_021F02C4 arms it,
// ov01_021F0360 interpolates it, ov01_021F03C8 is its HBlank callback).
typedef struct ScreenSplitEffect {
    QuadraticInterpolationTaskFX32 xInterpolationTask;
    QuadraticInterpolationTaskFX32 yInterpolationTask;
    u32 splitHeight;
    u32 state;
    struct HBlankSystem *hBlankSystem;
    struct HBlankTask *hBlankTask;
    struct SysTask *vBlankTask;
    BOOL *done;
} ScreenSplitEffect;

// sizeof(ScreenSplitEffect) == 0x48 on the DS

#endif // POKEHEARTGOLD_OVERLAY_01_SCREEN_SPLIT_EFFECT_H
