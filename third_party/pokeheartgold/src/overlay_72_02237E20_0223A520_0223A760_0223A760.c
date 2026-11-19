#include "global.h"

extern u8 ov72_0223B464;

extern u8 ov72_0223B466;

extern u8 ov72_0223B468;

extern u8 ov72_0223B46A;

extern u8 ov72_0223B48C;

u32 ov72_0223A760(s32 param_1, s32 param_2, u16 *param_3);

u32 ov72_0223A760(s32 param_1, s32 param_2, u16 *param_3) {
    u32 cVar1;
    int new_var;
    new_var = param_1 * 4;
    cVar1 = (&ov72_0223B48C)[new_var + param_2];
    if (cVar1 == 0xd) {
        cVar1 = (&ov72_0223B464)[*param_3];
    } else if (cVar1 == 0xe) {
        cVar1 = (&ov72_0223B466)[*param_3];
    } else if (cVar1 == 0xf) {
        cVar1 = (&ov72_0223B468)[param_3[1]];
    } else if (cVar1 == 0x10) {
        cVar1 = (&ov72_0223B46A)[param_3[1]];
    } else if (cVar1 == 0xa) {
        if ((param_1 == 5) || (param_1 == 0)) {
            *param_3 = 0;
        } else if ((param_1 == 1) || (param_1 == 6)) {
            *param_3 = 1;
        }
    } else if (cVar1 == 0xc) {
        if ((param_1 == 3) || (param_1 == 8)) {
            param_3[1] = 0;
        } else if ((param_1 == 4) || (param_1 == 9)) {
            param_3[1] = 1;
        }
    }
    return cVar1;
}
