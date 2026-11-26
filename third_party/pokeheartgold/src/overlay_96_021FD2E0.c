#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"
#include "overlay_96_ov96_rower_work.h"

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(void *, s32);

s32 ov96_021EB588(void *, s32 *);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021FEAEC(void *, s32);
#else
s32 ov96_021FEAEC(void *, s32);
#endif

void ov96_021FFB44(Ov96Obstacle *);

BOOL ov96_021FD2E0(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_021E637C(PokeathlonCourseData *);
s32 ov96_021E667C(PokeathlonCourseData *);
s32 ov96_021FD4D0(PokeathlonCourseData *);
s32 ov96_021FDB64(PokeathlonCourseData *);
BOOL ov96_021FD3EC(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_021FD2E0(PokeathlonCourseData *data, u8 *arg1) {
    s32 v4[3];
    s32 v3;
    Ov96RowerWork *v2;
    u8 *v0;
    PokeathlonCourseData *pokeathlonCourseData;
    s32 v7;
    u8 v10;
    u8 v5;
    u8 v8;
    u32 v9;

    v0 = arg1;
    pokeathlonCourseData = data;
    v2 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v5 = *v0;
    switch (v5) {
    case 0:
        for (v3 = 0; v3 < 0x1E; v3++) {
            v7 = v2->unk45C[v3];
            v8 = (u8)v7;
            v10 = (u8)(v7 >> 8);
            v9 = (u16)(v7 >> 0x10);
            if (v8 != 0) {
                v2->obstacles[v3].unk0 = 1;
                v2->obstacles[v3].unkB = v8;
                v2->obstacles[v3].unk8 = v9;
                v2->obstacles[v3].unkA = v10;
                ov96_021EB564(v2->obstacles[v3].unk4, v8 - 1);
                ov96_021EB52C(v2->obstacles[v3].unk4, 1, 1);
                v4[2] = 0;
                v4[0] = (v9 + 0x50) << 0xC;
                v4[1] = (v10 + 0x20) << 0xC;
                ov96_021EB588(v2->obstacles[v3].unk4, &v4[0]);
                ov96_021FFB44(&v2->obstacles[v3]);
            }
        }
        ov96_021FEAEC(v2, 0);
        *v0 = *v0 + 1;
        PokeathlonCourse_SetStateTransitionType(pokeathlonCourseData, 0x12U);
        break;
    case 1:
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0U, 6, 1, v2->heapID);
        *v0 = *v0 + 1;
        break;
    case 2:
        if (IsPaletteFadeFinished() != 0) {
            PokeathlonCourse_SetStateField07(pokeathlonCourseData, 1U);
        }
        break;
    }
    return 0;
}

BOOL ov96_021FD3EC(PokeathlonCourseData *data, u8 *arg1) {
    u8 v0;
    Ov96RowerWork *v1;

    v1 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        if (ov96_021E637C(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 1:
        ov96_021FD4D0(data);
        if (ov96_021FDB64(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 2:
        ov96_021FD4D0(data);
        if (ov96_021E667C(data) != 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, v1->heapID);
            PokeathlonCourse_SetStateField07(data, 2U);
        }
        break;
    }
    return 0;
}
