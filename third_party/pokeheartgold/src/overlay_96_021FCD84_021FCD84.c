#include "global.h"

#include "pokeathlon/pokeathlon.h"

void ov96_021E93B4(s32 *param_1, s32 param_2);

void ov96_021E9510(u32 *param_1);

void ov96_021E952C(s32 *param_1);

void ov96_021E9570(s32 *param_1, u8 param_2);

void ov96_021E966C(PokeathlonCourseData *param_1, s32 *param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6, u8 param_7, u8 param_8, u8 param_9, u8 param_10, u32 param_11);

void ov96_0221A56C(u32 param_1, s32 param_2);

BOOL ov96_021FCD84(PokeathlonCourseData *param_1, int unused);

BOOL ov96_021FCD84(PokeathlonCourseData *param_1, int unused) {
    u16 field;
    s32 *gfx;
    int i;

    gfx = PokeathlonCourse_GetGraphicsSystem(param_1);
    ov96_021E9510(gfx);
    for (i = 0; i < 4; i++) {
        field = (int)PokeathlonCourse_GetField5F0_AtIndex(param_1, (u8)i);
        ov96_021E966C(param_1, gfx, i, field, (u32)field >> 10, ((field % 1024) * 10) / 1024, 0, 1, 2, 1, 0x129);
    }
    ov96_021E93B4(gfx, 1);
    ov96_0221A56C((u32)(gfx), 6);
    ov96_021E952C(gfx);
    ov96_021E9570(gfx, 6);
    return 1;
}
