#include "global.h"
struct BattleHallApp;

#include "constants/heap.h"

#include "screen_fade.h"
#include "overlay_82_battle_hall_app.h"

void ov82_0223F90C(void *arg0);

s32 ov82_0223E820(struct BattleHallApp *arg0);

s32 ov82_0223E820(BattleHallApp *arg0) {
    u8 v0;

    v0 = arg0->subState;
    switch (v0) {
    case 0:
        arg0->fadeOutDelay = 0xAU;
        arg0->subState = (u8)(arg0->subState + 1);
    default:
        break;
    case 1:
        arg0->fadeOutDelay = (u8)(arg0->fadeOutDelay - 1);
        if (arg0->fadeOutDelay == 0) {
            ov82_0223F90C(arg0);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_105);
            arg0->subState = (u8)(arg0->subState + 1);
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }
    return 0;
}
