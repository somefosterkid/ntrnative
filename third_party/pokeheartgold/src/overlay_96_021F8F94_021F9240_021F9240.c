#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

s32 ov96_021FB60C(UnkStruct_ov96_021F94A8 *work);

void ov96_021FBDBC(UnkStruct_ov96_021F94A8 *work);

s32 ov96_021E637C(PokeathlonCourseData *);

BOOL ov96_021E667C(PokeathlonCourseData *);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_021E8318(PokeathlonCourseData *, s16);
#else
s32 ov96_021E8318(PokeathlonCourseData *, u16);
#endif

s32 ov96_021FA6D0(PokeathlonCourseData *, UnkStruct_ov96_021F94A8 *);

s32 ov96_021FAF1C(PokeathlonCourseData *);

s32 ov96_021FB630(PokeathlonCourseData *);

void ov96_021FB808(UnkStruct_ov96_021F94A8 *, s32);

BOOL ov96_021F9240(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_021F9240(PokeathlonCourseData *arg0, u8 *arg1) {
    s32 temp_r5;
    u16 temp_r2;
    u32 var_r5;
    u8 temp_r1;
    u8 temp_r5_2;
    u8 var_r0;
    UnkStruct_ov96_021F94A8 *temp_r0;
    UnkStruct_ov96_021F94A8 *temp_r4;
    temp_r0 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r1 = *arg1;
    temp_r4 = temp_r0;
    switch ((u32)temp_r1) {
    case 0:
        if (ov96_021E637C(arg0) != 0) {
            temp_r4->unk_3C7 = 1;
            *arg1 += 1;
        }
        break;

    case 1:
        ov96_021FBDBC(temp_r0);
        temp_r5 = ov96_021FA6D0(arg0, temp_r4);
        ov96_021FB630(arg0);
        if (temp_r5 != 0) {
            var_r5 = 0;
            var_r0 = 0;
            do {
                temp_r2 = temp_r4->rows[var_r0].unk_2;
                var_r5 += temp_r2;
                var_r0 += 1;
            } while (((u32)var_r0) < 3U);
            ov96_021E8318(arg0, var_r5);
            ov96_021FB808(temp_r4, var_r5);
            *arg1 += 1;
        }
        break;

    case 2:
        temp_r5_2 = ov96_021E667C(arg0);
        if ((ov96_021FB60C(temp_r4) != 0) && (temp_r5_2 != 0)) {
            *arg1 += 1;
        }
        ov96_021FBDBC(temp_r4);
        ov96_021FA6D0(arg0, temp_r4);
        ov96_021FB630(arg0);
        break;

    case 3:
        ov96_021FBDBC(temp_r0);
        ov96_021FB630(arg0);
        *((s8 *)(((u8 *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0))) + 9)) = 1;
        if ((*((s32 *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0xF0))) != 0) {
            *arg1 += 1;
            temp_r4->unk_234 = 0U;
        }
        break;

    case 4:
        temp_r4->unk_234 = temp_r4->unk_234 + 1;
        if (temp_r4->unk_234 > 0x5AU) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, temp_r4->heapID);
            PokeathlonCourse_SetStateField07(arg0, 2U);
        }
        break;
    }

    ov96_021FAF1C(arg0);
    return 0;
}
