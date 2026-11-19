#include "global.h"

#include "screen_fade.h"

void ov104_021E5EB0(s32 arg0, enum HeapID arg1);

void ov104_021E5EB0(s32 arg0, enum HeapID heapID) {
    if ((s32)(arg0 + 1) >= 3) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, heapID);
        return;
    }
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_CAMERA_SHUTTER_CLOSE, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, heapID);
}
