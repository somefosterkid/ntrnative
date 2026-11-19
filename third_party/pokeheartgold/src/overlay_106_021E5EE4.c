#include "global.h"

#include "constants/heap.h"

#include "overlay_106.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"


void ov106_021E5F84(MotionBlurParams *params);

void ov106_021E5EE4(SysTask *task, void *param) {
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

void ov106_021E5F24(SysTask *task, void *param) {
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

void ov106_021E5F84(MotionBlurParams *params) {
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
