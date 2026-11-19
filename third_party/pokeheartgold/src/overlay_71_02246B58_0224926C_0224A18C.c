#include "global.h"

#include "screen_fade.h"

s32 ov71_0224A278(void *arg0);

s32 ov71_0224A3F0(void *);

s32 ov71_0224A510(void *);

s32 ov71_0224A18C(void *arg0);

s32 ov71_0224A18C(void *arg0) {
    ov71_0224A278(arg0);
    ov71_0224A3F0(arg0);
    ov71_0224A510(arg0);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 8, 1, HEAP_ID_57);
    return 1;
}
