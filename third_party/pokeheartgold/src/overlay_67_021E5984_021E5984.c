#include "global.h"

#include "bg_window.h"
#include "overlay_manager.h"
#include "overlay_67_app.h"
#include "screen_fade.h"

s32 ov67_021E6A40(void);

BOOL ov67_021E5984(OverlayManager *man, s32 *state);

BOOL ov67_021E5984(OverlayManager *man, s32 *state) {
    Ov67App *app;
    BOOL BVar1;
    s32 iVar2;

    app = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        iVar2 = ov67_021E5A6C(app);
        if (iVar2 != 0) {
            *state = 1;
        }
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, app->heapID);
        *state = 2;
        break;
    case 2:
        BVar1 = IsPaletteFadeFinished();
        if (BVar1 != 0) {
            if (app->isRankingsScreen == 0) {
                *state = 3;
            } else {
                *state = 4;
            }
        }
        break;
    case 3:
        iVar2 = ov67_021E5C04(app);
        *state = iVar2;
        break;
    case 4:
        iVar2 = ov67_021E5C44(app);
        *state = iVar2;
        break;
    case 5:
        iVar2 = ov67_021E6A40();
        if (iVar2 == 0) {
            *state = app->unk_49C;
        }
        break;
    case 6:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, app->heapID);
        *state = 7;
        break;
    case 7:
        BVar1 = IsPaletteFadeFinished();
        if (BVar1 != 0) {
            *state = 8;
        }
        break;
    case 8:
        iVar2 = ov67_021E5B44(app);
        if (iVar2 != 0) {
            return 1;
        }
    }
    if ((*state >= 1) && (*state <= 7)) {
        ov67_021E69D8(app);
    }
    return 0;
}
