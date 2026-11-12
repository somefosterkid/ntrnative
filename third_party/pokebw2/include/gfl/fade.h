#ifndef POKEBW2_GFL_FADE_H
#define POKEBW2_GFL_FADE_H

#include "types.h"

// Screen fades with the master brightness (fade.c). Names from swan (https://github.com/ds-pokemon-hacking/swan,
// GPL-3.0)

// The screens a fade applies to, and whether it fades to black or to white
#define FADE_ENGINE_A_BLACK 0x1
#define FADE_ENGINE_B_BLACK 0x2
#define FADE_ENGINE_A_WHITE 0x4
#define FADE_ENGINE_B_WHITE 0x8

void GFL_FadeCreate(u32 heapId);
// Steps the fade; called every frame
void GFL_FadeUpdate(void);
s32 GFL_FadeGetUpdateFreq(void);
void GFL_FadeSetUpdateFreq(s32 updateFreq);
void GFL_FadeSetUpdateFreqOne(void);
// Fades from brightnessStart to brightnessEnd (0 to 16), waiting slowness frames between steps
void GFL_FadeSet(u32 mode, s32 brightnessStart, s32 brightnessEnd, s32 slowness);
BOOL GFL_FadeIsRunning(void);
// Sets the master brightness of the fade's screens
void GFL_FadeFlush(void);

#endif // POKEBW2_GFL_FADE_H
