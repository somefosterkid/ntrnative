#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "overlay_95_egg_hatch_app.h"
#include "sprite_system.h"

s32 ov95_021E7450(EggHatchPrompt *arg0);

s32 ov95_021E7514(EggHatchPrompt *arg0);

u8 ov95_021E7258(EggHatchPrompt *arg0);

void ov95_021E72B8(EggHatchPrompt *arg0);

u8 ov95_021E7258(EggHatchPrompt *arg0) {
    u8 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        if (ov95_021E7450(arg0) != 0) {
            arg0->state = (u8)(arg0->state + 1);
        }
        break;
    case 1:
        if (ov95_021E7514(arg0) != 0) {
            if (arg0->selection == 0) {
                GF_AssertFail();
            }
            ToggleBgLayer(5U, 0U);
            ToggleBgLayer(6U, 0U);
            ManagedSprite_SetDrawFlag(arg0->cursorSprite, 0);
            GfGfx_EngineBTogglePlanes(0x10U, 0U);
            return arg0->selection;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0U;
}

void ov95_021E72B8(EggHatchPrompt *arg0) {
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[0], 6U, 4U, 8U, 0x18U, 2U, 0xFU, 0x50U);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[1], 6U, 4U, 0xEU, 0x18U, 2U, 0xFU, 0x80U);
}
