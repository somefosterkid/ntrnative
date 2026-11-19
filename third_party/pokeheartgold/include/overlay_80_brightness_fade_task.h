#ifndef POKEHEARTGOLD_OVERLAY_80_BRIGHTNESS_FADE_TASK_H
#define POKEHEARTGOLD_OVERLAY_80_BRIGHTNESS_FADE_TASK_H

// BrightnessFadeTask: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/overlay_80_0223AC24_0223AC68.c) and 1 accesses; fields at every accessed offset, pads elsewhere.


typedef struct BrightnessFadeTask {
    struct { int currentValue; int startValue; int delta; int currentStep; int numSteps; } interpolationTask;
    u32 screen;
} BrightnessFadeTask;

// sizeof(BrightnessFadeTask) == 0x18

#endif // POKEHEARTGOLD_OVERLAY_80_BRIGHTNESS_FADE_TASK_H
