#include "global.h"

#include "pokeathlon/pokeathlon.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_021E8318(PokeathlonCourseData *, s16);
#else
s32 ov96_021E8318(PokeathlonCourseData *, u16);
#endif

s32 ov96_021FFD80(void *, s32, s32);

void ov96_021FFEE8(PokeathlonCourseData *arg0);

void ov96_021FFEE8(PokeathlonCourseData *arg0) {
    void *temp_r7;
    u8 var_r5;
    PokeathlonTally *temp_r4;
    u16 temp_r4_2;
    u8 temp_r0;

    temp_r4 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0xF0);
    temp_r7 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = (u8)ov96_021E5F24(arg0);
    var_r5 = 0x7F & (&temp_r4->u8s[temp_r0])[0x10];
    temp_r4_2 = temp_r4->u16s[temp_r0];
    if ((u32)var_r5 > 0x3CU) {
        var_r5 = 0x3C;
    }
    ov96_021E8318(arg0, (u16)(temp_r4_2 + (var_r5 << 0xA)));
    ov96_021FFD80(temp_r7, var_r5, temp_r4_2);
}
