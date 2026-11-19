#ifndef POKEHEARTGOLD_OVERLAY_59_CUTSCENE_TASKS_H
#define POKEHEARTGOLD_OVERLAY_59_CUTSCENE_TASKS_H

#include "global.h"
#include "sprite.h"

struct Ov59JuiceFeedApp;

// Heap objects of the ov59 sprite-effect tasks. Each begins with the pointer to the ov59 work object (`work`,
// the app's main heap object) that its task callback reads; sizeof is the allocation size on every build
// (DS: 0x20 / 8 / 8).

// ov59_0223BE70: scale and spin one sprite in or out.
typedef struct Ov59ScaleTask {
    struct Ov59JuiceFeedApp *work;
    u8 isOut;
    u8 pad_5;
    u8 timer;
    u8 pad_7;
    s32 scaleStep;
    u16 rotation;
    s16 rotationStep;
    VecFx32 scale;
    Sprite *sprite;
} Ov59ScaleTask;

// ov59_0223C054 / ov59_0223C09C: shake the mon picture.
typedef struct Ov59ShakeTask {
    struct Ov59JuiceFeedApp *work;
    u8 counter;
    u8 phase;
    u8 amount;
    u8 pad_7;
} Ov59ShakeTask;

// ov59_0223C138 / ov59_0223C16C: fade the mon picture and play the sound sequence.
typedef struct Ov59FadeTask {
    struct Ov59JuiceFeedApp *work;
    u8 timer;
    u8 step;
    u8 pad_6[2];
} Ov59FadeTask;

#endif // POKEHEARTGOLD_OVERLAY_59_CUTSCENE_TASKS_H
