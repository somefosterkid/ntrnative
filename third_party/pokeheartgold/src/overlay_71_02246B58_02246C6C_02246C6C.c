#include "global.h"
#include "sys_task.h"
#include "overlay_71_palette_shimmer_effect.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"

struct SysTask;

struct SysTaskQueue;

void *GfGfxLoader_GetPlttData(enum NarcId narcID, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

void *Heap_Alloc(u32 heapID, u32 size);

void Heap_Free(void *ptr);

SysTask * SysTask_CreateOnVWaitQueue(SysTaskFunc func, void *data, u32 priority);

void ov71_02246D9C(SysTask *task, void *param);

PaletteShimmerEffect *ov71_02246C6C(BOOL *isAlive, int direction);

PaletteShimmerEffect *ov71_02246C6C(BOOL *isAlive, int direction) {
    PaletteShimmerEffect *shimmerEffect = Heap_Alloc(HEAP_ID_56, sizeof(PaletteShimmerEffect));

    *isAlive = 0;

    if (shimmerEffect) {
        NNSG2dPaletteData *plttData;
        void *rawPaletteBuffer;

        rawPaletteBuffer = GfGfxLoader_GetPlttData((enum NarcId)89, 6, &plttData, HEAP_ID_56);

        if (rawPaletteBuffer) {
            MI_CpuFill16(shimmerEffect->paletteBuffer, 0x0, 96);
            Heap_Free(rawPaletteBuffer);
        }

        shimmerEffect->dirty = 0;
        shimmerEffect->stopping = 0;
        shimmerEffect->spawnTimer = 0;
        shimmerEffect->activeSparkCount = 0;
        shimmerEffect->isAlive = isAlive;
        shimmerEffect->nextSparkSlot = 0;
        shimmerEffect->direction = direction;

        for (int i = 0; i < 8; i++) {
            shimmerEffect->sparkTasks[i] = NULL;
        }

        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 16, 8);
        G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 16, 8);

        shimmerEffect->vblankTask = SysTask_CreateOnVWaitQueue(ov71_02246D9C, shimmerEffect, 20);

        if (shimmerEffect->vblankTask) {
            *(shimmerEffect->isAlive) = 1;
        }
    }

    return shimmerEffect;
}
