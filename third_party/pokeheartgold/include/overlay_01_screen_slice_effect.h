#ifndef POKEHEARTGOLD_OVERLAY_01_SCREEN_SLICE_EFFECT_H
#define POKEHEARTGOLD_OVERLAY_01_SCREEN_SLICE_EFFECT_H

#include "global.h"
#include "quadratic_interpolation_task.h"

struct HBlankSystem;
struct HBlankTask;
struct SysTask;

// ScreenSliceEffect: the battle-intro "screen slice" window effect (ov01_021EFFEC allocates it, ov01_021F0050 arms it,
// ov01_021F010C interpolates it, ov01_021F01D0 is its HBlank callback, ov01_021F0028 deletes it).
typedef struct ScreenSliceEffect {
    QuadraticInterpolationTaskFX32 interpolationTask;
    u32 pixelsPerSlice;
    u32 state;
    struct HBlankSystem *hBlankSystem;
    struct HBlankTask *hBlankTask;
    struct SysTask *vBlankTask;
    BOOL *done;
} ScreenSliceEffect;

// sizeof(ScreenSliceEffect) == 0x30 on the DS

#endif // POKEHEARTGOLD_OVERLAY_01_SCREEN_SLICE_EFFECT_H
