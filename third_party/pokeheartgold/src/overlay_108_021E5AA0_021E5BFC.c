#include "global.h"

#include "screen_fade.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E846C(SafariAreaCustomizer *arg0);

s32 ov108_021E5BFC(SafariAreaCustomizer *arg0);

s32 ov108_021E5BFC(SafariAreaCustomizer *arg0) {
    s32 temp_r0;
    temp_r0 = arg0->unk_4;
    switch (temp_r0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, arg0->heapID);
        arg0->unk_4 = (s32)((arg0->unk_4) + 1);
        break;

    case 1:
        if (IsPaletteFadeFinished() != 0) {
            arg0->unk_4 = 0;
            ov108_021E846C(arg0);
            return 1;
        }
        break;
    }

    ov108_021E846C(arg0);
    return 6;
}
