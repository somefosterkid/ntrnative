#include "global.h"

#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E846C(SafariAreaCustomizer *arg0);

s32 ov108_021E5B98(SafariAreaCustomizer *arg0);

s32 ov108_021E5B98(SafariAreaCustomizer *arg0) {
    s32 temp_r0;
    temp_r0 = arg0->unk_4;
    switch (temp_r0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, arg0->heapID);
        GfGfx_EngineATogglePlanes(0x10U, 1U);
        GfGfx_EngineBTogglePlanes(0x10U, 1U);

        arg0->unk_4 = (s32)((arg0->unk_4) + 1);
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0U) {
            arg0->unk_4 = 0;
            ov108_021E846C(arg0);
            return 2;
        }
        break;
    }
    ov108_021E846C(arg0);
    return 5;
}
