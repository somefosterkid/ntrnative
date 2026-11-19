#include "global.h"

u16 *ov01_021F98B4(u32 param_1, u32 param_2, u16 *param_3);

u16 *ov01_021F98B4(u32 param_1, u32 param_2, u16 *param_3) {
    do {
        if (*param_3 == param_1) {
            return param_3;
        }
        param_3 = param_3 + 2;
    } while (*param_3 != param_2);
    return NULL;
}
