#include "global.h"

void ov48_0225AEDC(void *);

void ov48_0225AFB4(void *, void *);

void ov48_0225AEA8(u8 *param_1, void *param_2);

void ov48_0225AEC4(void *arg0, void *arg1);

void ov48_0225AEA8(u8 *param_1, void *param_2) {
    *(u16 *)(param_1 + 4) = 0;
    *(u16 *)(param_1 + 6) = 0;
    ov48_0225AEDC(param_1);
    ov48_0225AFB4(param_1, param_2);
    return;
}

void ov48_0225AEC4(void *arg0, void *arg1) {
    *(s32 *)arg1 = (s32)(*(s16 *)arg0 << 0xC);
    *(s32 *)((u8 *)arg1 + 4) = (s32)((*(s16 *)((u8 *)arg0 + 2) << 0xC) + 0x100000);
}
