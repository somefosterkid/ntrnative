#include "global.h"

void ov43_0222DF90(u8 *, u32);

extern void (*_0222F0E0[3])(void *, u32, u32, u32);

extern void (*ov43_0222F12C[8])(void *, u32, u32, u32);

void ov43_0222DF4C(void *param_1, u32 param_2, u32 param_3, s32 param_4, void *param_5, u32 param_6, u32 param_7);

void ov43_0222DF4C(void *param_1, u32 param_2, u32 param_3, s32 param_4, void *param_5, u32 param_6, u32 param_7) {
    ov43_0222DF90(param_1, param_3);
    if (param_6 != 0) {
        ov43_0222F12C[param_4](param_1, param_2, param_3, param_7);
        return;
    }
    _0222F0E0[param_4](param_1, param_2, param_3, param_7);
    return;
}
