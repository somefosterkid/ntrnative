#include "global.h"

#include "bg_window.h"

void ov70_0223F20C(Window *, void *, s32, u32, u32);

void ov70_0223F324(Window *, void *, s32, u32, u32, u32, u32);

void ov70_0223F38C(Window *, void *, u32, u32, u32, u32, u32, u32);

u32 ov70_0223F864(s32, s32, u32);

void ov70_0223B41C(Window *param_1, void *param_2, void *param_3, s16 *param_4);

void ov70_0223B41C(Window *param_1, void *param_2, void *param_3, s16 *param_4) {
    u32 uVar1;
    s16 *psVar2;

    psVar2 = param_4;
    FillWindowPixelBuffer(param_1, 0);
    ov70_0223F20C(param_1, param_3, (s32)*param_4, 0, 0x10200);
    ov70_0223F324(param_1, param_2, (s32)((s8 *)param_4)[2], 0, 0x49, 0, 0x10200);
    uVar1 = ov70_0223F864((s32) * (s8 *)((u8 *)param_4 + 3), (s32)((s8 *)param_4)[4], 0);
    ov70_0223F38C(param_1, param_2, uVar1, 0, 0x68, 0, 0x10200, 0);
    return;
}
