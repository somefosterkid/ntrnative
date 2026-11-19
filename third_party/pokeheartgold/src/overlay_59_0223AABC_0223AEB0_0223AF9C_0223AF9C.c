#include "global.h"

#include "constants/heap.h"

#include "overlay_59_juice_feed_app.h"
#include "screen_fade.h"
#include "sprite.h"
#include "unk_02005D10.h"

s32 ov59_0223AF9C(Ov59JuiceFeedApp *arg0) {
    switch (arg0->subState) {
    case 0:
        if (Sprite_IsAnimated(arg0->sprites[0]) != 0) {
            return 0;
        }
        ov59_0223AEB0(arg0, 3);
        ov59_0223BBB0(arg0);
        ov59_0223BBD4(arg0, 0);
        Sprite_SetDrawFlag(arg0->sprites[2], 1);
        Sprite_SetAnimCtrlSeq(arg0->sprites[2], 0xA);
        Sprite_ResetAnimCtrlState(arg0->sprites[2]);
        PlaySE(0x921U);
        arg0->subState++;
        break;
    case 1:
        if (Sprite_IsAnimated(arg0->sprites[2]) != 0) {
            return 0;
        }
        Sprite_SetAnimCtrlSeq(arg0->sprites[2], 0xB);
        Sprite_ResetAnimCtrlState(arg0->sprites[2]);
        arg0->subState++;
        break;
    case 2:
        if (Sprite_IsAnimated(arg0->sprites[2]) != 0) {
            return 0;
        }
        ov59_0223C054(arg0);
        arg0->subState++;
        break;
    case 3:
        if (arg0->taskCount != 0) {
            return 0;
        }
        Sprite_SetDrawFlag(arg0->sprites[2], 0);
        ov59_0223C138(arg0);
        arg0->subState++;
        break;
    case 4:
        if (arg0->taskCount != 0) {
            return 0;
        }
        arg0->subState = 6;
        break;
    case 5:
        if (IsPaletteFadeFinished() != 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 6, 2, arg0->heapId);
            arg0->subState++;
        }
        break;
    default:
        if (IsPaletteFadeFinished() != 0) {
            arg0->subState = 0;
            return 1;
        }
        break;
    }
    return 0;
}
