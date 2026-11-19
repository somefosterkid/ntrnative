#include "global.h"

#include "screen_fade.h"

void ov74_02232910(void *arg0, enum FadeType arg1, s32 arg2, s32 *arg3);

void ov74_02232910(void *arg0, enum FadeType arg1, s32 arg2, s32 *arg3) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, arg1, arg1, 0U, 6, 1, HEAP_ID_76);
    if (arg3 != NULL) {
        *arg3 = 0x17;
    }
    *(s32 *)((u8 *)arg0 + 0x24) = arg2;
}

void ov74_02232940(void *arg0, enum FadeType arg1, s32 arg2, s32 *arg3);

void ov74_02232940(void *arg0, enum FadeType arg1, s32 arg2, s32 *arg3) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, arg1, arg1, 0x7FFFU, 6, 1, HEAP_ID_76);
    if (arg3 != NULL) {
        *arg3 = 0x17;
    }
    *(s32 *)((u8 *)arg0 + 0x24) = arg2;
}
