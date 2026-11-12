// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/brightness.h.
#ifndef BWPORT_SYSTEM_BRIGHTNESS_H
#define BWPORT_SYSTEM_BRIGHTNESS_H

#include "types.h"

// Brightness blending of the screens (brightness.c, a guessed name). pokebw2 names it after pokeplatinum's brightness
// controller, which works the same way

#define BRIGHTNESS_MAIN_SCREEN 1
#define BRIGHTNESS_SUB_SCREEN 2
#define BRIGHTNESS_BOTH_SCREENS 3

// pokebw2's BrightnessController_SetScreenBrightness and BrightnessController_StartTransition
void func_02023f60(s16 brightness, u32 planes, u32 screens);
void func_02023ef4(u8 steps, s16 targetBrightness, s16 startBrightness, u32 planes, u32 screens);
// Ends the screens' transitions (BrightnessController_Reset)
void func_02023f98(u32 screens);
// Runs a frame of the transitions (BrightnessController_Update)
void func_02023fd8(void);
// BrightnessController_IsTransitionComplete
BOOL func_02024004(u32 screens);

#endif // BWPORT_SYSTEM_BRIGHTNESS_H
