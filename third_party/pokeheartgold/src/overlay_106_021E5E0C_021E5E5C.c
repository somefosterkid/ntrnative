#include "global.h"

#include "sys_task.h"
#include "systask_environment.h"
#include "overlay_106_motion_blur.h"

void ov106_021E5E5C(MotionBlur *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov106_021E5E5C(MotionBlur *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r0;

    GX_SetGraphicsMode((GXDispMode)(arg1), (GXBGMode)(arg2), (GXBG0As)(arg3));
    GX_SetBankForLCDC(arg0->lcdcVRamBank);
    temp_r0 = arg0->params.displayMode;
    switch (temp_r0) {
    case 2:
        MIi_CpuClearFast(0U, (void *)HW_LCDC_VRAM_A, 0x20000U);
        break;
    case 6:
        MIi_CpuClearFast(0U, (void *)HW_LCDC_VRAM_B, 0x20000U);
        break;
    case 10:
        MIi_CpuClearFast(0U, (void *)HW_LCDC_VRAM_C, 0x20000U);
        break;
    case 14:
        MIi_CpuClearFast(0U, (void *)HW_LCDC_VRAM_D, 0x20000U);
        break;
    default:
        GX_SetBankForLCDC(0);
        break;
    }
    DestroySysTaskAndEnvironment(arg0->task);
}
