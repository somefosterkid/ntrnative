#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_02203468work.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_021E8318(PokeathlonCourseData *, s16);
#else
s32 ov96_021E8318(PokeathlonCourseData *, u16);
#endif

s32 ov96_0220337C(Ov96_02203468Work *, s32);

void ov96_022038D4(PokeathlonCourseData *arg0);

void ov96_022038D4(PokeathlonCourseData *arg0) {
    u16 var_r5;
    PokeathlonTally *temp_r5;
    Ov96_02203468Work *temp_r6;

    temp_r5 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0xF0);
    temp_r6 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    var_r5 = temp_r5->u16s[(u8)ov96_021E5F24(arg0)] & 0x3FF;
    if ((u32)var_r5 > 0x3E7U) {
        var_r5 = 0x3E7;
    }
    ov96_021E8318(arg0, var_r5);
    ov96_0220337C(temp_r6, var_r5);
}
