#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "constants/heap.h"

#include "screen_fade.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

s32 ov70_02241164(s32 arg0);

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_0223AFA8(void *, s32);

s32 ov70_0223B364(void *, s32, s32, s32, s32);

extern TouchscreenHitbox ov70_02245498;

extern u8 ov70_022454A4[];

extern u8 ov70_022454A5[];

s32 ov70_0223AFFC(GTSApplicationState *arg0);

s32 ov70_0223AFFC(GTSApplicationState *arg0) {
    s32 temp_r1 = 0;
    s32 temp_r1_2;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 temp_r0;

    if (gSystem.newKeys & 1) {
        ov70_0223B364(arg0, 0x10, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 3, 4);
        PlaySE(0x5DCU);
    } else if (2 & gSystem.newKeys) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 0x10, 1, HEAP_ID_61);
        arg0->currentScreenInstruction = 8;
        PlaySE(0x5DCU);
    } else if (0x20 & gSystem.newKeys) {
        temp_r5 = arg0->selectedSearchResult;
        temp_r1 = ov70_022454A4[temp_r5 * 2];
        if (temp_r5 != temp_r1 && arg0->searchResultCount >= temp_r1 + 1) {
            ov70_0223AFA8(arg0, temp_r1);
        }
    } else if (gSystem.newKeys & 0x10) {
        temp_r5_2 = arg0->selectedSearchResult;
        temp_r1_2 = ov70_022454A5[temp_r5_2 * 2];
        if (temp_r5_2 != temp_r1_2 && arg0->searchResultCount >= temp_r1_2 + 1) {
            ov70_0223AFA8(arg0, temp_r1_2);
        }
    } else {
        temp_r0 = ov70_02241164(arg0->searchResultCount);
        if (temp_r0 != -1) {
            ov70_0223AFA8(arg0, temp_r0);
        }
        temp_r0 = TouchscreenHitbox_FindRectAtTouchNew(&ov70_02245498);
        if (temp_r0 == 0) {
            ov70_0223B364(arg0, 0x10, 1, 0, 0xF0F);
            ov70_02238D84(arg0, 3, 4);
            PlaySE(0x5DCU);
        } else if (temp_r0 == 1) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 0x10, 1, HEAP_ID_61);
            arg0->currentScreenInstruction = 8;
            PlaySE(0x5DCU);
        }
    }
    return 3;
}
