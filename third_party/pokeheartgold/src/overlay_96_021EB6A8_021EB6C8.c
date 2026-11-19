#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "screen_fade.h"
#include "overlay_96_app_021ed1d0.h"

s32 ov96_021ED8DC(PokeathlonCourseData *);

s32 ov96_021EB6C8(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_021EB6C8(PokeathlonCourseData *data, u8 *arg1) {
    Ov96App_021ED1D0 *work;
    u8 v0;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        if (ov96_021E5F24(data) == 0) {
            ov96_021ED8DC(data);
        }
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0U, 6, 1, work->heapId);
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
