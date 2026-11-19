#include "global.h"
#include "pokeathlon/pokeathlon.h"
#include "error_handling.h"
#include "screen_fade.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_02215914(s32 *arg0, s32 arg1);

void ov96_02215914(s32 *param_1, s32 param_2);
void ov96_02215944(s32 *param_1, s32 param_2, s32 param_3);
void ov96_02215958(void *arg0, s32 arg1);
s32 ov96_021E637C(PokeathlonCourseData *);
s32 ov96_02215984(PokeathlonCourseData *arg0, u8 *arg1);

void ov96_02215914(s32 *arg0, s32 arg1) {
    s32 v0;
    s32 v1;

    v0 = *arg0;
    if (v0 != 0) {
        if (v0 < 0) {
            v1 = 0 - v0;
        } else {
            v1 = v0;
        }
        if (v1 < arg1) {
            *arg0 = 0;
            return;
        }
        *arg0 -= (v0 > 0 ? 1 : -1) * arg1;
    }
}

void ov96_02215944(s32 *param_1, s32 param_2, s32 param_3) {
    ov96_02215914(param_1, param_2);
    ov96_02215914(param_1 + 1, param_3);
    return;
}

void ov96_02215958(void *arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    s32 v2;

    v0 = *(s32 *)arg0;
    v2 = 0 - arg1;
    if (v0 > 0) {
        if (v0 > arg1) {
            *(s32 *)arg0 = arg1;
        }
    } else if (v0 < v2) {
        *(s32 *)arg0 = v2;
    }
    v1 = *(s32 *)((u8 *)arg0 + 4);
    if (v1 > 0) {
        if (v1 > arg1) {
            *(s32 *)((u8 *)arg0 + 4) = arg1;
        }
    } else if (v1 < v2) {
        *(s32 *)((u8 *)arg0 + 4) = v2;
    }
}

s32 ov96_02215984(PokeathlonCourseData *data, u8 *arg1) {
    Ov96RdWork *work;
    u8 v0;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 6, 1, work->heapId);
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
