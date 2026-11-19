#include "global.h"

#include "unk_020210A0.h"
#include "screen_fade.h"
#include "overlay_113.h"

s32 ov113_021E5D28(UnownReportData *);

s32 ov113_021E5BB0(UnownReportData *arg0);

s32 ov113_021E5BC4(UnownReportData *arg0);

s32 ov113_021E5BB0(UnownReportData *arg0) {
    DisableTouchPad();
    ov113_021E5D28(arg0);
    return 1;
}

s32 ov113_021E5BC4(UnownReportData *arg0) {
    u16 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, arg0->heapID);
        arg0->state = (u16)(arg0->state + 1);
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            arg0->state = 0U;
            return 2;
        }
        break;
    }
    return 0;
}
