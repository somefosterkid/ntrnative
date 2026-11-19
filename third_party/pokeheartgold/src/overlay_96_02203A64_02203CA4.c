#include "global.h"

#include "error_handling.h"

s32 ov96_021EB570(void *, s32);

void ov96_02203CA4(void *arg0, u32 arg1, s32 arg2);

void ov96_02203CA4(void *arg0, u32 arg1, s32 arg2) {
    if (arg1 >= 3U) {
        GF_AssertFail();
    }
    ov96_021EB570(*(void **)((u8 *)(arg0 + (arg1 * 4)) + 0x54), arg2 + arg1);
}

void ov96_021EB57C(void *param_1);

void ov96_02203CC4(void *param_1, s32 param_2);

void ov96_02203CC4(void *param_1, s32 param_2) {
    ov96_021EB57C(*(void **)(param_1 + param_2 * 4 + 0x54));
    return;
}

void ov96_02203CD4(void *param_1);

void ov96_02203CD4(void *param_1) {
    *(u8 *)(param_1 + 0x70) = 1;
    *(u8 *)(param_1 + 0x71) = 0;
    return;
}
