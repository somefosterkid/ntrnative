#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "screen_fade.h"

s32 ov96_022021D4(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_022021D4(PokeathlonCourseData *arg0, u8 *arg1) {
    unsigned char new_var;
    u8 *temp_r5;
    u32 var_r4;
    u8 *var_r5;
    temp_r5 = PokeathlonCourse_GetDataCopyArea(arg0);
    PokeathlonCourse_GetHeapAllocPtr4(arg0);
    if ((*arg1) == 0) {
        if (IsPaletteFadeFinished() != 0) {
            *arg1 = 1;
        }
        return 0;
    }
    if (ov96_021E5F24(arg0) != 0) {
        return 1;
    }
    var_r5 = ov96_021E8A20(temp_r5 + 0x28);
    var_r4 = 0;
    do {
        PokeathlonCourse_SetField5E0_AtIndex(arg0, (s32)(new_var = var_r4), (u16)((*(u16 *)var_r5) & 0x3FF));
        var_r4 += 1;
        var_r5 += 2;
    } while (((s32)var_r4) < 4);
    return 1;
}
