#include "global.h"

u32 ov02_02250738(s32 param_1, u8 param_2);

u32 ov02_02250738(s32 param_1, u8 param_2) {
    u8 buf[5] = { 1, 2, 4, 8, 16 };
    s32 i;

    for (i = 0; i < 5; i++) {
        if ((buf[param_1 - 1] & param_2) == 0) {
            return 1;
        }
    }
    return 0;
}
