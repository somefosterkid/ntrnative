#include "global.h"
#include "screen_fade.h"
#include "overlay_59_juice_feed_app.h"

s32 ov59_0223AB3C(Ov59JuiceFeedApp *arg0);

s32 ov59_0223AB3C(Ov59JuiceFeedApp *arg0) {
    u16 temp_r0;

    temp_r0 = arg0->state;
    switch (temp_r0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, arg0->heapId);
        arg0->state = (u16) (arg0->state + 1);
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            arg0->state = 0U;
            return 2;
        }
        break;
    }
    return 0;
}

s32 ov59_0223AB90(Ov59JuiceFeedApp *arg0);

s32 ov59_0223AB90(Ov59JuiceFeedApp *arg0) {
    u16 temp_r0;

    temp_r0 = arg0->state;
    switch (temp_r0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, arg0->heapId);
        arg0->state = (u16) (arg0->state + 1);
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            arg0->state = 0U;
            return 8;
        }
        break;
    }
    return 1;
}
