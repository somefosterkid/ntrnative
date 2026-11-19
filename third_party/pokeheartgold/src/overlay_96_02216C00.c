#include "global.h"

u32 ov96_02216C00(u8 *param_1, u8 *param_2);

u32 ov96_02216C00(u8 *param_1, u8 *param_2) {
    if (*param_1 == *param_2) {
        return 0;
    }
    if (*param_1 < *param_2) {
        return 1;
    }
    return 0xffffffff;
}

u32 ov96_02216C1C(u8 *param_1, u8 *param_2);

u32 ov96_02216C1C(u8 *param_1, u8 *param_2) {
    if (*param_1 == *param_2) {
        return 0;
    }
    if (*param_1 < *param_2) {
        return 1;
    }
    return 0xffffffff;
}
