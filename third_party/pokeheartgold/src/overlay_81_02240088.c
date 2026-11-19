#include "global.h"

#include "constants/heap.h"

#include "overlay_83_battle_castle_self_app.h"
#include "screen_fade.h"

BOOL ov81_02240088(BattleCastleSelfApp *app);

BOOL ov81_02240088(BattleCastleSelfApp *app) {
    switch (app->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_100);
        app->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }

    return 0;
}
