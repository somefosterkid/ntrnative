#include "global.h"

#include "screen_fade.h"
#include "system.h"
#include "overlay_113.h"

void ov113_021E5DA4(UnownReportData *, u8);

s32 ov113_021E5C68(UnownReportData *arg0);

void ov113_021E62B0(void *);
void ov113_021E6300(UnownReportData *);
void ov113_021E650C(UnownReportData *);
void ov113_021E6680(UnownReportData *);
void ov113_021E671C(UnownReportData *);
void ov113_021E6988(UnownReportData *);
void ov113_021E5CF8(UnownReportData *arg0);
void ov113_021E6490(UnownReportData *);
s32 ov113_021E663C(UnownReportData *);
s32 ov113_021E66E4(UnownReportData *);
s32 ov113_021E6754(UnownReportData *);
void ov113_021E69A8(UnownReportData *);
void ov113_021E5D28(UnownReportData *arg0);
void ov113_021E5E64(UnownReportData *, s32);
void ov113_021E5D4C(UnownReportData *arg0);

s32 ov113_021E5F48(UnownReportData *);
s32 ov113_021E5F94(UnownReportData *, s32 *);
int ov113_021E5D60(UnownReportData *arg0);

s32 ov113_021E5C68(UnownReportData *arg0) {
    u16 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_UPWARD_OUT, FADE_TYPE_DOWNWARD_OUT, 0U, 6, 1, arg0->heapID);
        arg0->state = (u16)(arg0->state + 1);
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            ov113_021E5DA4(arg0, arg0->page);
            arg0->state = (u16)(arg0->state + 1);
        }
        break;
    case 2:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_UNK_5, 0U, 6, 1, arg0->heapID);
        arg0->state = (u16)(arg0->state + 1);
        break;
    case 3:
        if (IsPaletteFadeFinished() != 0) {
            arg0->state = 0U;
            return 2;
        }
        break;
    }
    return 3;
}

void ov113_021E5CF8(UnownReportData *arg) {
    ov113_021E6300(arg);
    ov113_021E650C(arg);
    ov113_021E6680(arg);
    ov113_021E671C(arg);
    ov113_021E6988(arg);
    Main_SetVBlankIntrCB(ov113_021E62B0, arg);
}

void ov113_021E5D28(UnownReportData *arg0) {
    ov113_021E69A8(arg0);
    ov113_021E6754(arg0);
    ov113_021E66E4(arg0);
    ov113_021E663C(arg0);
    ov113_021E6490(arg0);
}

void ov113_021E5D4C(UnownReportData *arg0) {
    ov113_021E5E64(arg0, 0);
    ov113_021E5DA4(arg0, arg0->page);
}

int ov113_021E5D60(UnownReportData *arg0) {
    s32 v0;
    v0 = 0;
    ov113_021E5F94(arg0, &v0);
    if (v0 == 0) {
        ov113_021E5F48(arg0);
    }
}
