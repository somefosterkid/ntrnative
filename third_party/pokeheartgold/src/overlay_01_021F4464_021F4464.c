#include "global.h"

#include "constants/heap.h"

#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "overlay_01_021F4464.h"
#include "overlay_106_motion_blur.h"

void ov01_021F4584(SysTask *task, void *param);
void ov01_021F4544(SysTask *task, void *param);
void ov01_021F45E4(MotionBlurParams *params);

MotionBlur *ov01_021F4464(MotionBlurParams *params);

void ov01_021F44B4(MotionBlur **motionBlur, GXDispMode displayMode, GXBGMode bgMode, GXBG0As bg0Mode);

MotionBlur *ov01_021F4464(MotionBlurParams *params) {
    SysTask *task = CreateSysTaskAndEnvironment(ov01_021F4544, sizeof(MotionBlur), 5, params->heapID);
    MotionBlur *motionBlur = SysTask_GetData(task);

    motionBlur->params = *params;
    motionBlur->task = task;
    motionBlur->initialized = 0;
    motionBlur->lcdcVRamBank = GX_GetBankForLCDC();

    ov01_021F45E4(&motionBlur->params);
    SysTask_CreateOnVWaitQueue(ov01_021F4584, motionBlur, 0);

    return motionBlur;
}

void ov01_021F44B4(MotionBlur **motionBlur, GXDispMode displayMode, GXBGMode bgMode, GXBG0As bg0Mode) {
    GX_SetGraphicsMode(displayMode, bgMode, bg0Mode);
    GX_SetBankForLCDC((*motionBlur)->lcdcVRamBank);

    switch ((*motionBlur)->params.displayMode) {
    case GX_DISPMODE_VRAM_A:
        MI_CpuClearFast((void *)HW_LCDC_VRAM_A, 0x20000);
        break;
    case GX_DISPMODE_VRAM_B:
        MI_CpuClearFast((void *)HW_LCDC_VRAM_B, 0x20000);
        break;
    case GX_DISPMODE_VRAM_C:
        MI_CpuClearFast((void *)HW_LCDC_VRAM_C, 0x20000);
        break;
    case GX_DISPMODE_VRAM_D:
        MI_CpuClearFast((void *)HW_LCDC_VRAM_D, 0x20000);
        break;
    default:
        GX_SetBankForLCDC(GX_VRAM_LCDC_NONE);
        break;
    }

    DestroySysTaskAndEnvironment((*motionBlur)->task);
    *motionBlur = 0L;
}

void ov01_021F4544(SysTask *task, void *param) {
    MotionBlur *motionBlur = param;

    if (motionBlur->initialized) {
        GX_SetCapture(
            motionBlur->params.captureSize,
            motionBlur->params.captureMode,
            motionBlur->params.captureSourceA,
            motionBlur->params.captureSourceB,
            motionBlur->params.captureDestination,
            motionBlur->params.blendCoeffA,
            motionBlur->params.blendCoeffB);
    }
}

void ov01_021F4584(SysTask *task, void *param) {
    MotionBlur *motionBlur = (MotionBlur *)param;

    switch (motionBlur->params.displayMode) {
    case GX_DISPMODE_VRAM_A:
        GX_SetBankForLCDC(GX_VRAM_LCDC_A);
        break;
    case GX_DISPMODE_VRAM_B:
        GX_SetBankForLCDC(GX_VRAM_LCDC_B);
        break;
    case GX_DISPMODE_VRAM_C:
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        break;
    case GX_DISPMODE_VRAM_D:
        GX_SetBankForLCDC(GX_VRAM_LCDC_D);
        break;
    default:
        GX_SetBankForLCDC(GX_VRAM_LCDC_NONE);
        break;
    }

    GX_SetGraphicsMode(motionBlur->params.displayMode, motionBlur->params.bgMode, motionBlur->params.bg0Mode);
    motionBlur->initialized = 1;
    SysTask_Destroy(task);
}

void ov01_021F45E4(MotionBlurParams *params) {
    switch (params->displayMode) {
    case GX_DISPMODE_VRAM_A:
        MI_CpuClearFast((void *)HW_LCDC_VRAM_A, 0x20000);
        break;
    case GX_DISPMODE_VRAM_B:
        MI_CpuClearFast((void *)HW_LCDC_VRAM_B, 0x20000);
        break;
    case GX_DISPMODE_VRAM_C:
        MI_CpuClearFast((void *)HW_LCDC_VRAM_C, 0x20000);
        break;
    case GX_DISPMODE_VRAM_D:
        MI_CpuClearFast((void *)HW_LCDC_VRAM_D, 0x20000);
        break;
    default:
        break;
    }

    GX_SetCapture(
        params->captureSize,
        params->captureMode,
        params->captureSourceA,
        params->captureSourceB,
        params->captureDestination,
        16,
        0);
}
