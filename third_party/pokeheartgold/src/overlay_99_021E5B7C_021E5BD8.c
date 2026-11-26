#include "global.h"

#include "error_handling.h"
#include "overlay_99.h"

u16 ov99_021E5BD8(PokeathlonCourseRecordArgs *param_1, s32 param_2, s32 param_3) {
    u16 *p;
    u8 *new_var;
    if (param_1 == NULL) {
        GF_AssertFail();
    }
    if (param_3 == 3) {
        u32 off = param_2 * 0x2c;
        p = (u16 *)(off + (u8 *)param_1->pokeathlon);
        new_var = (u8 *)param_1->pokeathlon;
        return (u16)(((u16)(((u16)(((u16 *)(off + new_var))[3] - ((u16 *)(new_var + off))[0])) - ((u16 *)(off + (u8 *)param_1->pokeathlon))[1])) - p[2]);
    }
    return *((u16 *)(((u8 *)param_1->pokeathlon + (param_2 * 0x2c)) + (param_3 * 2)));
}
