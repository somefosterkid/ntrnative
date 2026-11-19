#include "global.h"

s32 ov41_02248998(void *);

void ov41_02247598(u32 param_1, u8 *param_2);

void ov41_02247598(u32 param_1, u8 *param_2) {
    s32 iVar1;

    iVar1 = ov41_02248998(param_2 + 0x368);
    if (iVar1 != 0) {
        *(u32 *)(param_2 + 0x6b0) = 3;
    }
    return;
}

void ov41_02248790(u8 *, u32, u32);

u32 ov41_0224894C(void *);

void ov41_022475B4(u32 param_1, u8 *param_2);

void ov41_022475B4(u32 param_1, u8 *param_2) {
    u32 uVar1;

    uVar1 = ov41_0224894C(param_2 + 0x368);
    ov41_02248790(param_2 + 0x368, uVar1, 0);
    return;
}

void ov41_022475D4(u32 param_1, u8 *param_2);

void ov41_022475D4(u32 param_1, u8 *param_2) {
    u32 uVar1;

    uVar1 = ov41_0224894C(param_2 + 0x368);
    ov41_02248790(param_2 + 0x368, uVar1, 1);
    return;
}

void ov41_022487F8(void *, u32, u32);

u32 ov41_0224895C(u8 *, u32);

void ov41_022475F4(u32 param_1, u8 *param_2);

void ov41_022475F4(u32 param_1, u8 *param_2) {
    u32 uVar1;

    if (*(s32 *)(param_2 + 0x6b0) != 0) {
        uVar1 = ov41_0224895C(param_2 + 0x368, 0);
        ov41_022487F8(param_2 + 0x368, 0, uVar1);
        *(u32 *)(param_2 + 0x6b0) = 0;
    }
    return;
}

void ov41_02247628(u32 param_1, u8 *param_2);

void ov41_02247628(u32 param_1, u8 *param_2) {
    u32 uVar1;

    if (*(s32 *)(param_2 + 0x6b0) != 1) {
        uVar1 = ov41_0224895C(param_2 + 0x368, 1);
        ov41_022487F8(param_2 + 0x368, 1, uVar1);
        *(u32 *)(param_2 + 0x6b0) = 1;
    }
    return;
}
