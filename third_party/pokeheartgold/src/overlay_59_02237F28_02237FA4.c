#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "screen_fade.h"

s32 sub_02031C00(void *);

s32 ov59_02237FA4(Ov59ApricornApp *arg0);

s32 ov59_02237FA4(Ov59ApricornApp *arg0) {
    u16 temp_r0;
    u8 temp_r0_2;

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
            temp_r0_2 = arg0->mode;
            switch (temp_r0_2) {
            case 1:
                if (sub_02031C00(arg0->apricornBox) != 1) {
                    return 2;
                }
                return 4;
            case 2:
                return 5;
            default:
            case 0:
                return 3;
            }
        } else {
            break;
        }
        break;
    }
    return 0;
}

s32 ov59_0223801C(Ov59ApricornApp *arg0);

s32 ov59_0223801C(Ov59ApricornApp *arg0) {
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
            return 0x10;
        }
        break;
    }
    return 1;
}
