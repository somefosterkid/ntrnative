#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "screen_fade.h"
#include "sound.h"
#include "unk_02005D10.h"
#include "overlay_96_app_021ed1d0.h"

s32 ov96_021EBA98(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_021EDA58(PokeathlonCourseData *);
s32 ov96_021EBAFC(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_021EBA98(PokeathlonCourseData *data, u8 *arg1) {
    u8 v0;

    v0 = *arg1;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFFU, 0x5A, 1, PokeathlonCourse_GetHeapID(data));
        GF_SndStartFadeOutBGM(0U, 0x5AU);
        PlaySE(0x8DDU);
        *arg1 += 1;
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

s32 ov96_021EBAFC(PokeathlonCourseData *data, u8 *arg1) {
    Ov96App_021ED1D0 *work;
    u8 v0;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        if (ov96_021E5F24(data) == 0) {
            ov96_021EDA58(data);
        }
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0U, 6, 1, work->heapId);
        *arg1 += 1;
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            PokeathlonCourse_SetStateField07(data, 1U);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
