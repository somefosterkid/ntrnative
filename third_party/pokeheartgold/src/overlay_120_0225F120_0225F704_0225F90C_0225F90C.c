#include "global.h"

#include "constants/heap.h"

#include "field/hblank_system.h"

#include "error_handling.h"
#include "overlay_120.h"
#include "screen_fade.h"

BOOL ov120_0225F90C(Ov120StepWork *work, s32 *state) {
    switch (*state) {
    case 0:
        HBlankSystem_Stop(work->hBlankSystem);
        BeginNormalPaletteFade(FADE_MAIN_ONLY, (enum FadeType)0x2a, (enum FadeType)0x2a, 0, 0x1c, 1, HEAP_ID_FIELD1);
        *state = 1;
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            *state = 2;
        }
        break;
    case 2:
        HBlankSystem_Start(work->hBlankSystem);
        *state = 0;
        return TRUE;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL ov120_0225F970(Ov120StepWork *work, s32 *state) {
    switch (*state) {
    case 0:
        HBlankSystem_Stop(work->hBlankSystem);
        BeginNormalPaletteFade(FADE_MAIN_ONLY, (enum FadeType)0x18, (enum FadeType)0x18, 0, 0x1c, 1, HEAP_ID_FIELD1);
        *state = 1;
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            *state = 2;
        }
        break;
    case 2:
        HBlankSystem_Start(work->hBlankSystem);
        *state = 0;
        return TRUE;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}
