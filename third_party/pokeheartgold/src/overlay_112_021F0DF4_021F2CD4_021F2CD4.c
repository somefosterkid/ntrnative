#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

s32 ov112_021F1CC8(void *, s32);

s32 ov112_021F2328(void *);

s32 ov112_021F2B80(void *);

s32 ov112_021F2CD4(Ov112SpriteApp *arg0);

s32 ov112_021F2CD4(Ov112SpriteApp *arg0) {

    s32 var_r1;
    s32 var_r1_2;

    if (arg0->bits144.fadeActive == 0) {
        return 1;
    }
    switch (arg0->bits144.fadeState) {
    case 0:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFFU, 2, 1, arg0->heapID);
        arg0->bits144.fadeState++;
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            if (arg0->curIdx != 0) {
                var_r1 = 1;
            } else {
                var_r1 = 0;
            }
            ManagedSprite_SetDrawFlag(arg0->sprites[0], var_r1);
            if ((s32) arg0->curIdx < (s32)(arg0->count - 1)) {
                var_r1_2 = 1;
            } else {
                var_r1_2 = 0;
            }
            ManagedSprite_SetDrawFlag(arg0->sprites[1], var_r1_2);
            ov112_021F1CC8(arg0, 0);
            ov112_021F2328(arg0);
            ov112_021F2B80(arg0);
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 2, 1, arg0->heapID);
            arg0->bits144.fadeState++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() != 0) {
            arg0->bits144.fadeState = 0;
            arg0->bits144.fadeActive = 0;
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
