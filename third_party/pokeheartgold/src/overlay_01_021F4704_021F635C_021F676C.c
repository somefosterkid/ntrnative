#include "global.h"

#include "heap.h"
#include "overlay_01_021F4704.h"
#include "sys_task_api.h"

SysTask *ov01_021F676C(NARC *narc, u32 modelNo, int modelSize, NNSG3dRenderObj *renderObj, NNSG3dResFileHeader **modelFile, NNSG3dResTex *texture, struct FieldSystemUnkSubCC *areaMapAnimation, BOOL *modelLoaded, BOOL *loading) {
    MapModelLoadTask *task;

    task = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(MapModelLoadTask));
    task->narc = narc;
    task->modelNo = modelNo;
    task->modelSize = modelSize;
    task->renderObj = renderObj;
    task->modelFile = modelFile;
    task->texture = texture;
    task->areaMapAnimation = areaMapAnimation;
    task->state = 0;
    task->modelLoaded = modelLoaded;
    *modelLoaded = 0;
    task->loading = loading;
    task->cancel = 0;
    return SysTask_CreateOnMainQueue(ov01_021F6620, task, 1U);
}
