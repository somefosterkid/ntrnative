#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

u32 ov96_021EDCEC(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, u32 param_4);

u32 ov96_021EDCEC(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, u32 param_4) {
    PokeathlonFieldData *pPVar1;
    pPVar1 = PokeathlonCourse_GetFieldData(param_1);
    if (param_3 >= 4) {
        GF_AssertFail();
    }
    if (param_4 >= 3) {
        GF_AssertFail();
    }
    switch (param_2) {
    case 0:
        return *((u32 *)((pPVar1[param_3].data + 4) + (param_4 * 0x20)));

    case 1:
        return *((u32 *)((pPVar1[param_3].data + 0xc) + (param_4 * 0x20)));

    case 2:
        return *((u32 *)(pPVar1[param_3].data + (param_4 * 0x20)));

    case 3:
        return *((u32 *)((pPVar1[param_3].data + 0x14) + (param_4 * 0x20)));

    default:
        GF_AssertFail();
        GF_AssertFail();
        return 0;
    }
}
