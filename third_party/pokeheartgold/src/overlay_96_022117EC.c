#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"

#include "overlay_96_app_02210C98.h"

s32 ov96_021E637C(PokeathlonCourseData *);

s32 ov96_021E667C(PokeathlonCourseData *);

s32 ov96_02211AF0(PokeathlonCourseData *);

s32 ov96_02211B94(PokeathlonCourseData *);

s32 ov96_0221236C(PokeathlonCourseData *);

s32 ov96_022117EC(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_02211870(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_022117EC(PokeathlonCourseData *data, u8 *arg1) {
    u8 v0;
    Ov96App_02210C98 *v1;

    v1 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        if (ov96_021E637C(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 1:
        ov96_02211B94(data);
        if (ov96_02211AF0(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 2:
        ov96_02211B94(data);
        ov96_0221236C(data);
        if (ov96_021E667C(data) != 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, v1->unk_58);
            PokeathlonCourse_SetStateField07(data, 1U);
        }
        break;
    }
    return 0;
}

s32 ov96_02211870(PokeathlonCourseData *data, u8 *arg1) {
    u32 v0;
    Ov96App_02210C98 *v1;
    int new_var;
    v1 = PokeathlonCourse_GetHeapAllocPtr4(data);
    new_var = 0;
    if ((*arg1) == new_var) {
        if (IsPaletteFadeFinished()) {
            *arg1 = 1;
        }
        return new_var;
    }
    if (ov96_021E5F24(data) != new_var) {
        return 1;
    }
    v0 = new_var;
    do {
        PokeathlonCourse_SetField5E0_AtIndex(data, (s32)((((((((v0 & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu), (u16)v1->unk_6F4[v0]);
        v0 += 1;
    } while (((s32)v0) < 4);
    return 1;
}
