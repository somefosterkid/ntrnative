#include "global.h"

#include "screen_fade.h"

s32 ov71_02249E6C();

s32 ov71_02249D58(void);

s32 ov71_02249D58(void) {
    ov71_02249E6C();
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 8, 1, HEAP_ID_57);
    return 1;
}
