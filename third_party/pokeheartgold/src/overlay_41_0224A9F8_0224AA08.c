#include "global.h"

#include "bg_window.h"
#include "options.h"
#include "render_window.h"

void ov41_0224ACA4(void *, u32);

void ov41_0224ACDC(u32, u32);

void ov41_0224AD0C(u8 *, u32, u32, u32, u32, u32, u32, u32);

void ov41_0224AE24(Window *, u32, u32, u32, u32, u32, u32, u32);

void ov41_0224AED8(void *, u32, u32, u32, u32);

void ov41_0224B118(void *, u32, u32, u32, u32, u32, u32);

void ov41_0224AA08(void *param_1, u32 *param_2, u32 param_3);

void ov41_0224AA08(void *param_1, u32 *param_2, u32 param_3) {
    u8 *new_var;
    u8 bVar1;
    u32 uVar2;
    if ((param_3 & 1) != 0) {
        memset(param_1, 0, 0x148);
    }
    if ((param_3 & 2) != 0) {
        ov41_0224ACA4(param_1, *param_2);
    }
    if ((param_3 & 4) != 0) {
        ov41_0224AD0C(((u8 *)param_1) + 0x2c, *param_2, 2, 0x13, 0x1b, 4, 0x1f, 1);
    }
    if ((param_3 & 8) != 0) {
        ov41_0224AED8(((u8 *)param_1) + 0x38, param_2[1], param_2[2], param_2[4], param_2[9]);
    }
    if ((param_3 & 0x10) != 0) {
        ov41_0224B118(((u8 *)param_1) + 0xa0, param_2[1], param_2[2], param_2[7], *param_2, param_2[8], param_2[9]);
    }
    new_var = (u8 *)param_1;
    if ((param_3 & 0x20) != 0) {
        ov41_0224AD0C(new_var + 0x134, *param_2, 2, 1, 0x1b, 2, 0x8b, 1);
        FillWindowPixelBuffer(*((Window **)(((u8 *)param_1) + 0x134)), 0xf);
        ov41_0224AE24(*((void **)(((u8 *)param_1) + 0x134)), 0x1b, param_2[5], 6, 0, 0, 0x1020f, 0xff);
        ov41_0224AE24(*((void **)(((u8 *)param_1) + 0x134)), 0x1b, param_2[5], param_2[6], 0x48, 0, 0x1020f, 0xff);
        DrawFrameAndWindow2(*((Window **)(((u8 *)param_1) + 0x134)), 0, 1, 1);
    }
    *((u32 *)(new_var + 0x30)) = param_2[1];
    *((u32 *)(((u8 *)param_1) + 0x34)) = param_2[2];
    ov41_0224ACDC(*param_2, param_2[3]);
    uVar2 = Options_GetFrame((Options *)param_2[3]);
    *((u32 *)(new_var + 0x13c)) = uVar2;
    bVar1 = Options_GetTextFrameDelay((Options *)param_2[3]);
    *((u32 *)(new_var + 0x140)) = (u32)bVar1;
    *((u32 *)(new_var + 0x138)) = (*((u32 *)(new_var + 0x138))) | param_3;
    return;
}
