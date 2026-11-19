#include "global.h"
#include "constants/heap.h"
#include "sys_task.h"
#include "overlay_106_motion_blur.h"

struct FieldMotionBlur;

struct MotionBlur;

struct MotionBlurParams;

struct SysTask;

struct SysTaskQueue;

MotionBlur *ov01_021F4464(MotionBlurParams *params);

typedef struct FieldMotionBlur {
    MotionBlur *motionBlur;
} FieldMotionBlur;

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void *Heap_Alloc(u32 heapID, u32 size);

FieldMotionBlur *ov01_021F467C(int coeffA, int coeffB);

FieldMotionBlur *ov01_021F467C(int coeffA, int coeffB) {
    FieldMotionBlur *fieldMotionBlur;

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);

    GX_ResetBankForBG();

    fieldMotionBlur = Heap_Alloc(HEAP_ID_FIELD1, sizeof(FieldMotionBlur));
    MI_CpuClear32(fieldMotionBlur, sizeof(FieldMotionBlur));

    {
        MotionBlurParams motionBlurParams = {
            GX_DISPMODE_VRAM_C,
            GX_BGMODE_0,
            GX_BG0_AS_3D,
            GX_CAPTURE_SIZE_256x192,
            GX_CAPTURE_MODE_AB,
            GX_CAPTURE_SRCA_2D3D,
            GX_CAPTURE_SRCB_VRAM_0x00000,
            GX_CAPTURE_DEST_VRAM_C_0x00000,
            0,
            0,
            (enum HeapID)4
        };

        motionBlurParams.blendCoeffA = coeffA;
        motionBlurParams.blendCoeffB = coeffB;

        fieldMotionBlur->motionBlur = ov01_021F4464(&motionBlurParams);
    }

    return fieldMotionBlur;
}
