#include "global.h"

u16 ov74_02233A88(u32 *param0, u16 param1);
void ov74_02233AB8(int param0, void *param1);

u16 ov74_02233A88(u32 *param0, u16 param1) {
    u16 v0;
    u32 v1 = 0;

    for (v0 = 0; v0 < (param1 / 4); v0++) {
        v1 += *param0;
        param0++;
    }

    return (u16)(((v1 >> 16) & 0xffff) + (v1 & 0xffff));
}

void ov74_02233AB8(int param0, void *param1) {
    CTRDG_ReadAgbFlash((u16)param0, 0, param1, 0x1000);
}
