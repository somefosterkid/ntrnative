#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"
#include "math_util.h"

s32 ov96_021E637C(PokeathlonCourseData *);

s32 ov96_021E667C(PokeathlonCourseData *);

s32 ov96_021F5A88(PokeathlonCourseData *);

s32 ov96_021F5B60(PokeathlonCourseData *);

s32 ov96_021F5BA0(PokeathlonCourseData *);

BOOL ov96_021F5650(PokeathlonCourseData *arg0, u8 *arg1);

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_021E8228.c (tools/pokehg_abi.py)
void ov96_021E8228(PokeathlonCourseData *, u32, u32, s32, s32);
#else
void ov96_021E8228(PokeathlonCourseData *, u8, u8, s32, s32);
#endif
BOOL ov96_021F5730(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_021F5650(PokeathlonCourseData *data, u8 *arg1) {
    u8 v0;
    UnkStruct_ov96_021F5018 *v1;

    v1 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        ov96_021F5B60(data);
        PokeathlonCourse_SetStateTransitionType(data, 0x14U);
        *arg1 += 1;
        break;
    case 1:
        ov96_021F5B60(data);
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0U, 6, 1, v1->heapID);
        *arg1 += 1;
        break;
    case 2:
        if (IsPaletteFadeFinished() != 0) {
            *arg1 += 1;
        }
        ov96_021F5B60(data);
        break;
    case 3:
        ov96_021F5B60(data);
        if (ov96_021E637C(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 4:
        ov96_021F5BA0(data);
        if (ov96_021F5A88(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 5:
        if (ov96_021E667C(data) != 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, v1->heapID);
            PokeathlonCourse_SetStateField07(data, 1U);
        }
        break;
    }
    return 0;
}

BOOL ov96_021F5730(PokeathlonCourseData *data, u8 *arg1) {
    s32 index;
    s32 value;
    s32 v1;
    s32 v2;
    s32 v4;
    s32 v7;
    s32 v8;
    u16 v0;
    u8 v5;
    u8 v6;
    u32 v10;
    PokeathlonMonKinds *v3;
    UnkStruct_ov96_021F5018 *v9;

    v9 = PokeathlonCourse_GetHeapAllocPtr4(data);
    if (*arg1 == 0) {
        if (IsPaletteFadeFinished() != 0) {
            *arg1 = 1;
        }
        return 0;
    }
    v4 = ov96_021E5F24(data);
    v8 = 0;
    v7 = 0;
    do {
        v0 = v9->unk_90[v7].unk_20;
        v7 += 1;
        v8 += v0;
    } while (v7 < 3);
    if (v8 > 0x3E7) {
        v8 = 0x3E7;
    }
    PokeathlonCourse_SetField5E0_AtIndex(data, (s32)(u8)v4, (u16)v8);
    if (v4 == 0) {
        v1 = (s32)PokeathlonCourse_GetParticipantCount(data);
        index = v1;
        if (v1 < 4) {
            do {
                v10 = 0;
                value = 0;
                v6 = (u8)index;
loop_11:
                v3 = ov96_021E60D8(data, index, v10);
                if (v3->statIdx[2] != 0) {
                    value += (v3->statIdx[3] * 2) + ((6 * v3->statIdx[2]) + 0x50 + (v3->statIdx[4] * 2)) + ((s32)LCRandom() % 21);
                    LCRandom();
                }
                ov96_021E8228(data, v6, v10, 2, 0);
                v5 = (u8)((s32)LCRandom() % 21);
                ov96_021E8228(data, v6, v10, 4, (s32)v5);
                ov96_021E8228(data, v6, v10, 1, (s32)v5);
                v10 += 1;
                if ((s32)v10 < 3) {
                    goto loop_11;
                }
                PokeathlonCourse_SetField5E0_AtIndex(data, (s32)(u8)index, (u16)value);
                v2 = index + 1;
                index = v2;
            } while (v2 < 4);
        }
    }
    return 1;
}
