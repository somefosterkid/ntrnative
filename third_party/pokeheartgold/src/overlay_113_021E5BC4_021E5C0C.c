#include "global.h"

#include "screen_fade.h"
#include "overlay_113.h"

s32 ov113_021E5EC4(UnownReportData *);

s32 ov113_021E5C0C(UnownReportData *arg0);

s32 ov113_021E5C0C(UnownReportData *arg0) {
    u16 temp_r1;
    temp_r1 = arg0->state;
    switch (temp_r1) {
    case 0:
        if (ov113_021E5EC4(arg0) == 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, arg0->heapID);
            arg0->state = (u16)(arg0->state + 1);
        }
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            arg0->state = 0U;
            return 4;
        }
        break;
    }
    return 1;
}
