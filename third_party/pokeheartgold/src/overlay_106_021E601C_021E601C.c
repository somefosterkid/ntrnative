#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "sys_task.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

struct MotionBlur;

struct MotionBlur;

struct MotionBlurParams;

struct SysTask;

struct SysTaskQueue;

MotionBlur *ov106_021E5E0C(MotionBlurParams *params);

MotionBlur *ov106_021E601C(int coeffA, int coeffB);

extern const MotionBlurParams ov106_021E6DD8;

MotionBlur *ov106_021E601C(int coeffA, int coeffB) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);

    GX_ResetBankForBG();

    {
        MotionBlurParams motionBlurParams = ov106_021E6DD8;

        motionBlurParams.blendCoeffA = coeffA;
        motionBlurParams.blendCoeffB = coeffB;

        return ov106_021E5E0C(&motionBlurParams);
    }
}
