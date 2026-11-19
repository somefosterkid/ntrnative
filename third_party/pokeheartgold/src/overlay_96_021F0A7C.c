#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"
#include "overlay_96_ov96_arena_work.h"

s32 ov96_021F0A7C(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_021F0B58(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_021F0A7C(PokeathlonCourseData *data, u8 *arg1) {
    u8 v1;
    Ov96ArenaWork *work;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    v1 = *arg1;
    switch (v1) {
    case 0:
        PokeathlonCourse_SetStateTransitionType(data, 0x15U);
        *arg1 += 1;
        break;
    case 1:
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0U, 6, 1, work->heapId);
        *arg1 += 1;
        break;
    case 2:
        if (IsPaletteFadeFinished() != 0) {
            PokeathlonCourse_SetStateField07(data, 1U);
        }
        break;
    }
    return 0;
}

s32 ov96_021E637C(PokeathlonCourseData *);

s32 ov96_021E667C(PokeathlonCourseData *);

s32 ov96_021F107C(PokeathlonCourseData *);

s32 ov96_021F1170(PokeathlonCourseData *);

s32 ov96_021F0AE0(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_021F0AE0(PokeathlonCourseData *data, u8 *arg1) {
    u8 v0;
    Ov96ArenaWork *work;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        if (ov96_021E637C(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 1:
        ov96_021F1170(data);
        if (ov96_021F107C(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 2:
        if (ov96_021E667C(data) != 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, work->heapId);
            PokeathlonCourse_SetStateField07(data, 2U);
        }
        break;
    }
    return 0;
}

s32 ov96_021F0B58(PokeathlonCourseData *data, u8 *arg1) {
    s32 index;
    Ov96ArenaWork *work;
    PokeathlonCourseData *pokeathlonCourseData;
    s32 value;
    s32 v1;
    pokeathlonCourseData = data;
    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    if ((*arg1) == 0) {
        if (IsPaletteFadeFinished() != 0) {
            *arg1 = 1;
        }
        return 0;
    }
    if (ov96_021E5F24(pokeathlonCourseData) != 0) {
        return 1;
    }
    for (index = 0; index < 4; index++) {
        value = 0;
        for (v1 = 0; v1 < 3; v1++) {
            value += work->mon[index][v1].unk_42;
            PokeathlonCourse_SetField5E0_AtIndex(pokeathlonCourseData, (s32)((u8)index), value);
        }
    }
    return 1;
}
