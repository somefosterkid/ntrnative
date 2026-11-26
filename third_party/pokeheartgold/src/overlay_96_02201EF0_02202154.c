#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"
#include "overlay_96_ov96_02203468work.h"

s32 ov96_021E637C(PokeathlonCourseData *);

s32 ov96_021E667C(PokeathlonCourseData *);

s32 ov96_0220223C(PokeathlonCourseData *);

s32 ov96_022028BC(PokeathlonCourseData *);

BOOL ov96_02202154(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_02202154(PokeathlonCourseData *data, u8 *arg1) {
    u8 v0;
    Ov96_02203468Work *v1;

    v1 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        if (ov96_021E637C(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 1:
        ov96_0220223C(data);
        if (ov96_022028BC(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 2:
        ov96_0220223C(data);
        if (ov96_021E667C(data) != 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, v1->heapID);
            PokeathlonCourse_SetStateField07(data, 1U);
        }
        break;
    }
    return 0;
}
