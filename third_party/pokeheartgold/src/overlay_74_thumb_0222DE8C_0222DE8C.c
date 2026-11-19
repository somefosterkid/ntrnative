#include "global.h"

#include "constants/heap.h"

#include "overlay_74_wonder_cards_app_data.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov74_0222DE68(SysTask *task, void *param);
BufferManager *sub_02014A08(enum HeapID heapID, void *buffer1, void *buffer2);

void ov74_0222DE8C(WonderCardsAppData *appData, enum WonderCardFlipStage stage, fx32 startOffsetStep, fx32 startDeltaOffsetStep);

void ov74_0222DE8C(WonderCardsAppData *appData, enum WonderCardFlipStage stage, fx32 startOffsetStep, fx32 startDeltaOffsetStep) {
    WonderCardFlipAnimManager *animMan = &appData->flipAnimMan;

    animMan->animStage = stage;
    animMan->offsetStep = startOffsetStep;
    animMan->deltaOffsetStep = startDeltaOffsetStep;
    if (animMan->bufferManager == NULL) {
        animMan->bufferManager = sub_02014A08(HEAP_ID_85, animMan->scrollsBuffers[0], animMan->scrollsBuffers[1]);
    }
    if (animMan->sysTask == NULL) {
        animMan->sysTask = SysTask_CreateOnVBlankQueue(ov74_0222DE68, animMan, 0x400);
    }
    animMan->running = TRUE;
}
