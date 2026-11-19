#include "global.h"

#include "sys_task.h"
#include "heap.h"
#include "sys_task_api.h"
#include "overlay_71_palette_shimmer_effect.h"

void ov71_02246E84(PaletteShimmerEffect *shimmerEffect, int clearIndex, int setIndex, int color);

void ov71_02246F90(SysTask *task, void *data);

void ov71_02246EAC(PaletteShimmerEffect *shimmerEffect);

void ov71_02246F60(PaletteShimmerEffect *shimmerEffect, int slotIdx);

typedef struct PaletteShimmerSpark {
    PaletteShimmerEffect *shimmerEffect;
    int paletteIndex;
    int framesPerStep;
    int frameCounter;
    int slotIndex;
    int direction;
} PaletteShimmerSpark;

void ov71_02246F90(SysTask *unused, void *param);

void ov71_02246E84(PaletteShimmerEffect *shimmerEffect, int clearIndex, int setIndex, int color) {
    if ((clearIndex >= 0) && (clearIndex < 96)) {
        shimmerEffect->paletteBuffer[clearIndex] = 0x0;
    }

    if ((setIndex >= 0) && (setIndex < 96)) {
        shimmerEffect->paletteBuffer[setIndex] = color;
    }

    shimmerEffect->dirty = 1;
}

void ov71_02246EAC(PaletteShimmerEffect *shimmerEffect) {
    s32 v1;
    PaletteShimmerSpark *v0;

    if ((shimmerEffect->sparkTasks[shimmerEffect->nextSparkSlot] == NULL) && (v0 = Heap_Alloc(HEAP_ID_56, sizeof(PaletteShimmerSpark)), (v0 != NULL))) {
        v0->shimmerEffect = shimmerEffect;
        if (shimmerEffect->direction == 1) {
            v1 = -1;
        } else {
            v1 = 0xF;
        }
        v0->paletteIndex = v1;
        v0->frameCounter = 0;
        v0->framesPerStep = 2;
        v0->slotIndex = shimmerEffect->nextSparkSlot;
        v0->direction = shimmerEffect->direction;
        shimmerEffect->sparkTasks[shimmerEffect->nextSparkSlot] = SysTask_CreateOnMainQueue(ov71_02246F90, v0, 0xAU);
        if (shimmerEffect->sparkTasks[shimmerEffect->nextSparkSlot] != NULL) {
            shimmerEffect->activeSparkCount++;
            shimmerEffect->nextSparkSlot++;
            if (shimmerEffect->nextSparkSlot >= 8) {
                shimmerEffect->nextSparkSlot = 0;
            }
        } else {
            Heap_Free(v0);
        }
    }
}

void ov71_02246F60(PaletteShimmerEffect *shimmerEffect, int slotIdx) {
    if (shimmerEffect->sparkTasks[slotIdx]) {
        Heap_Free(SysTask_GetData(shimmerEffect->sparkTasks[slotIdx]));
        SysTask_Destroy(shimmerEffect->sparkTasks[slotIdx]);
        shimmerEffect->sparkTasks[slotIdx] = NULL;
        shimmerEffect->activeSparkCount--;
    }
}

void ov71_02246F90(SysTask *unused, void *param) {
    PaletteShimmerSpark *spark = param;

    if (++(spark->frameCounter) >= spark->framesPerStep) {
        spark->frameCounter = 0;

        if (spark->direction == 1) {
            if (spark->paletteIndex + 1 >= 15) {
                ov71_02246E84(spark->shimmerEffect, spark->paletteIndex, spark->paletteIndex + 1, 0x0);
                ov71_02246F60(spark->shimmerEffect, spark->slotIndex);
            } else {
                ov71_02246E84(spark->shimmerEffect, spark->paletteIndex, spark->paletteIndex + 1, 0x7fff);
                spark->paletteIndex++;
            }
        } else {
            if (spark->paletteIndex < 0) {
                ov71_02246E84(spark->shimmerEffect, spark->paletteIndex, spark->paletteIndex + 1, 0x0);
                ov71_02246F60(spark->shimmerEffect, spark->slotIndex);
            } else {
                ov71_02246E84(spark->shimmerEffect, spark->paletteIndex + 1, spark->paletteIndex, 0x7fff);
                spark->paletteIndex--;
            }
        }
    }
}
