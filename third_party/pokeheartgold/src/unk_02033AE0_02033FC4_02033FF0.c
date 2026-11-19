#include "global.h"

#include "error_handling.h"

extern u8 _020F6955;

u8 sub_02033FF0(u32 arg0);

u8 sub_02033FF0(u32 arg0) {
    u8 sp0[41];
    u32 var_r1;
    u8 *var_r2;
    u8 *var_r3;
    u8 temp_r0;
    var_r3 = &_020F6955;
    var_r2 = sp0;
    var_r1 = 0x29;
    do {
        temp_r0 = *var_r3;
        var_r3 += 1;
        ;
        *var_r2 = temp_r0;
        var_r2 += 1;
        var_r1 -= 1;
    } while (0 != var_r1);
    if (arg0 >= 0x29U) {
        GF_AssertFail();
    }
    return sp0[arg0];
}
