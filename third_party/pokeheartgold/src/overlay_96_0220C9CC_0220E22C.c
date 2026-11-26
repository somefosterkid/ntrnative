#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "screen_fade.h"

s32 ov96_021E637C(PokeathlonCourseData *);

s32 ov96_0220EFD0(u8 *, BgConfig *, enum HeapID);

BOOL ov96_0220E22C(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_0220E22C(PokeathlonCourseData *data, u8 *arg1) {
    u8 v1;
    UnkStruct_ov96_0220E3F0 *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_0220EFD0(v0->hud.unk_194, v0->bgConfig, v0->heapId);
    v1 = *arg1;
    switch (v1) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 6, 1, v0->heapId);
        *arg1 += 1;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            *arg1 += 1;
        }
        break;
    case 2:
        if (ov96_021E637C(data) != 0) {
            PokeathlonCourse_SetStateField07(data, 1U);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
