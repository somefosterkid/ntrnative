#include "global.h"

#include "error_handling.h"

u32 ov02_022506D4(u32 param_1, u32 param_2);

u32 ov02_022506D4(u32 param_1, u32 param_2) {
    if (param_1 <= 0xF9) {
        if (param_1 == param_2) {
            return 1;
        }
    } else {
        switch (param_1) {
        case 0xfa:
            if (param_2 <= 0x13) {
                return 1;
            }
            break;
        case 0xfb:
            if (param_2 <= 0x82) {
                return 1;
            }
            break;
        case 0xfc:
            if ((param_2 >= 0x8C) && (param_2 <= 0x95)) {
                return 1;
            }
            break;
        case 0xfd:
            if (param_2 >= 0xA0) {
                return 1;
            }
            break;
        case 0xfe:
            if (param_2 >= 0xDC) {
                return 1;
            }
            break;
        default:
            GF_AssertFail();
            return 0;
        }
    }
    return 0;
}
