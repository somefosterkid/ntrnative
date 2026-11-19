#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "screen_fade.h"

#include "overlay_96_ov96_rd_work.h"

s32 ov96_021E667C(PokeathlonCourseData *);

u32 ov96_02215864(Ov96RdScroll *scroll);

s32 ov96_02215FC8(PokeathlonCourseData *);

s32 ov96_02216234(PokeathlonCourseData *);

s32 ov96_022159F0(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_022159F0(PokeathlonCourseData *arg0, u8 *arg1) {
    Ov96RdWork *temp_r6;
    u8 temp_r0;

    temp_r6 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = *arg1;
    switch (temp_r0) {
    case 0:
        ov96_02215FC8(arg0);
        if (ov96_02216234(arg0) != 0) {
            *arg1 += 1;
        }
        break;
    case 1:
        ov96_02215FC8(arg0);
        if ((ov96_021E667C(arg0) != 0) && (ov96_02215864(&temp_r6->scroll) != 0)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, temp_r6->heapId);
            PokeathlonCourse_SetStateField07(arg0, 2U);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
