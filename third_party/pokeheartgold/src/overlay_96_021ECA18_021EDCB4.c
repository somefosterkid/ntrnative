#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

u8 ov96_021EDCB4(PokeathlonCourseData *arg0);

u8 ov96_021EDCB4(PokeathlonCourseData *data) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    return (u8)((u32)(v1->pairs[0].word1 << 0x1D) >> 0x1D);
}
