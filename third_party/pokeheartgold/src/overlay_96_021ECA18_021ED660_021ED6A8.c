#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

s32 ov96_021ED6A8(PokeathlonCourseData *arg0, s32 arg1, s32 arg2);

s32 ov96_021ED6A8(PokeathlonCourseData *arg0, s32 arg1, s32 arg2) {
    PokeathlonCourseData *var_r6;
    s32 var_r4;
    u8 *temp_r0;
    PokeathlonTally *var_r5;
    u8 *var_r7;

    var_r6 = arg0;
    var_r4 = arg2 + (arg1 * 3);
    temp_r0 = PokeathlonCourse_GetDataCopyArea(arg0);
    var_r7 = temp_r0;
    var_r5 = ov96_021E8A20(temp_r0 + 0xF0);
    if (var_r6 == NULL) {
        GF_AssertFail();
    }
    if (var_r7 == NULL) {
        GF_AssertFail();
    }
    if (var_r5 == NULL) {
        GF_AssertFail();
    }
    return ((u32)(var_r5->pairs[0].word1 << 0x11) >> 0x14) & (1 << var_r4);
}
