#include "global.h"

void ov74_02235728(u32, u32, u32, u32, u32, u32);

void ov74_0223589C(u32, u32);

void *ov74_02235930(u32, void *, u32, u32, u32);

void ov74_02235BD0(void *param_1);

void ov74_02235BD0(void *param_1) {
    void *uVar1;

    ov74_02235728(0x71, 0x27, 0x24, 0x26, 0x25, 1);
    ov74_0223589C(0, 0x100000);
    uVar1 = ov74_02235930(1, *(void **)(param_1 + 0x208), 0x80, 0, 0);
    *(void **)(param_1 + 0x208) = uVar1;
    return;
}
