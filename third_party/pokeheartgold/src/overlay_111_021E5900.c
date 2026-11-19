#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "overlay_111.h"

void ov111_021E5E34(OverlayManager *man);

BOOL BugContestSwapMon_Init(OverlayManager *man, int *state) {
    BugContestSwapMon *swapMon;
    BOOL BVar2;
    s32 iVar3;

    iVar3 = *state;
    switch (iVar3) {
    case 0:
        ov111_021E5E34(man);
        *state = *state + 1;
        break;
    case 1:
        swapMon = OverlayManager_GetData(man);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, swapMon->heapID);
        *state = *state + 1;
        break;
    case 2:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            *state = 0;
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

void ov111_021E5F04(OverlayManager *);

BOOL BugContestSwapMon_Exit(OverlayManager *man, int *state) {
    BugContestSwapMon *swapMon;
    s32 v0;

    swapMon = OverlayManager_GetData(man);
    v0 = *state;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, swapMon->heapID);
        *state += 1;
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            ov111_021E5F04(man);
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
