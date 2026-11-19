#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

u32 ov96_021E75E4(PokeathlonCourseData *arg0);

u32 ov96_021E75E4(PokeathlonCourseData *arg0) {
    u8 *sp4;
    u8 *sp0;
    s32 var_r4;
    u16 *var_r5;
    u32 temp_r6;
    u8 *temp_r1;
    void *temp_r3;
    void *temp_r4;

    sp0 = PokeathlonCourse_GetDataCopyArea(arg0);
    if (ov96_021E5F24(arg0) != 0) {
        GF_AssertFail();
        return 0;
    }
    var_r5 = arg0->field_5E0[0];
    var_r4 = 0;
    sp4 = sp0;
    sp4 = sp0 + 0x28;
    do {
        temp_r6 = PokeathlonCourse_GetUnkConstant4(arg0);
        memcpy(ov96_021E8A20(sp4) + (var_r4 * temp_r6), var_r5, temp_r6);
        var_r4 += 1;
        var_r5 += 2;
    } while (var_r4 < 4);
    temp_r4 = ov96_021E9A14();
    temp_r3 = PokeathlonCourse_GetSystem(arg0);
    temp_r1 = sp0 + 0x28;
    sp0 = temp_r1;
    return ov96_021E87B4(0x1E, temp_r1, temp_r4, (s32)temp_r3);
}
