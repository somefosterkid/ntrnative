#include "global.h"

#include "constants/heap.h"

#include "overlay_74_wonder_cards_app2_data.h"
#include "sys_task.h"
#include "sys_task_api.h"

BufferManager *sub_02014A08(enum HeapID heapID, void *buffer1, void *buffer2);
void sub_02014A38(BufferManager *bufferManager);
void sub_02014AA0(void);

void ov74_0222F600(SysTask *task, void *param);

void ov74_0222F624(WonderCardsApp2Data *appData, int stage, fx32 startOffsetStep, fx32 startDOfsStep);

void ov74_0222F688(WonderCardsApp2Data *appData);

void ov74_0222F624(WonderCardsApp2Data *appData, int stage, fx32 startOffsetStep, fx32 startDOfsStep) {
    WonderCardFlipAnimManager *animMan = &appData->flipAnimMan;

    animMan->animStage = stage;
    animMan->offsetStep = startOffsetStep;
    animMan->deltaOffsetStep = startDOfsStep;
    if (animMan->bufferManager == NULL) {
        animMan->bufferManager = sub_02014A08(HEAP_ID_85, animMan->scrollsBuffers[0], animMan->scrollsBuffers[1]);
    }
    if (animMan->sysTask == NULL) {
        animMan->sysTask = SysTask_CreateOnVBlankQueue(ov74_0222F600, animMan, 0x400);
    }
    animMan->running = TRUE;
}

void ov74_0222F688(WonderCardsApp2Data *appData) {
    WonderCardFlipAnimManager *animMan = &appData->flipAnimMan;

    if (animMan->bufferManager) {
        sub_02014A38(animMan->bufferManager);
    }
    if (animMan->sysTask) {
        SysTask_Destroy(animMan->sysTask);
    }
    animMan->bufferManager = NULL;
    animMan->sysTask = NULL;
    sub_02014AA0();
}
