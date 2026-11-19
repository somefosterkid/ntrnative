#include "global.h"

#include "error_handling.h"
#include "overlay_95_egg_hatch_app.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

void ov95_021E7410(EggHatchPrompt *);

extern TouchscreenHitbox ov95_021E7820;

s32 ov95_021E7450(EggHatchPrompt *arg0);

s32 ov95_021E7514(EggHatchPrompt *arg0);

s32 ov95_021E7450(EggHatchPrompt *arg0) {
    s32 v0;
    s32 v1;

    v1 = 0;
    v0 = TouchscreenHitbox_FindRectAtTouchNew(&ov95_021E7820);
    if (v0 != -1) {
        PlaySE(0x5DCU);
        switch (v0) {
        case 0:
            v1 = 1;
            arg0->selection = 1U;
            break;
        case 1:
            arg0->selection = 2U;
            v1 = 1;
            break;
        default:
            GF_AssertFail();
            break;
        }
    } else if (0x40 & gSystem.newKeys) {
        if (arg0->selection != 1) {
            arg0->selection = 1U;
            ov95_021E7410(arg0);
            PlaySE(0x5DCU);
        }
    } else if (0x80 & gSystem.newKeys) {
        if (arg0->selection != 2) {
            arg0->selection = 2U;
            ov95_021E7410(arg0);
            PlaySE(0x5DCU);
        }
    } else if (gSystem.newKeys & 1) {
        if (arg0->selection == 0) {
            GF_AssertFail();
            arg0->selection = 2U;
        }
        v1 = 1;
    } else if (gSystem.newKeys & 2) {
        arg0->selection = 2U;
        v1 = 1;
    }
    if (v1 != 0) {
        ov95_021E7410(arg0);
        ManagedSprite_SetAnim(arg0->cursorSprite, 3);
        PlaySE(0x5DCU);
    }
    return v1;
}

s32 ov95_021E7514(EggHatchPrompt *arg0) {
    if (ManagedSprite_GetActiveAnim(arg0->cursorSprite) != 3) {
        GF_AssertFail();
    }
    if (ManagedSprite_IsAnimated(arg0->cursorSprite) == 0) {
        return 1;
    }
    return 0;
}
