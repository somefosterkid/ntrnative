#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96hurdle_work.h"
#include "screen_fade.h"

s32 ov96_021E637C(PokeathlonCourseData *);

s32 ov96_021E667C(PokeathlonCourseData *);

s32 ov96_022055AC(PokeathlonCourseData *);

s32 ov96_02205C94(PokeathlonCourseData *);

s32 ov96_02205D30(PokeathlonCourseData *);

s32 ov96_0220549C(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_0220549C(PokeathlonCourseData *data, u8 *arg1) {
    u8 v0;
    Ov96HurdleWork *v1;

    v1 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        ov96_02205D30(data);
        if (ov96_021E637C(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 1:
        ov96_022055AC(data);
        if (ov96_02205C94(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 2:
        ov96_022055AC(data);
        if (ov96_021E667C(data) != 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, v1->heapId);
            PokeathlonCourse_SetStateField07(data, 2U);
        }
        break;
    }
    return 0;
}
