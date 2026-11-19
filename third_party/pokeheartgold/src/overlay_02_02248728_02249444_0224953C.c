#include "global.h"

#include "field_types_def.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_02_hmcut_in.h"

BOOL ov02_0224953C(SysTask *task);

BOOL ov02_0224953C(SysTask *task) {
    HMCutIn *cutIn = SysTask_GetData(task);

    return cutIn->isFinished;
}

void ov02_0224957C(void *ptr);

void ov02_02249548(SysTask *task);

void ov02_02249548(SysTask *task) {
    HMCutIn *cutIn = SysTask_GetData(task);

    ov02_0224957C(cutIn);
    SysTask_Destroy(task);
}

HMCutIn *ov02_0224955C(FieldSystem *fieldSystem);

HMCutIn *ov02_0224955C(FieldSystem *fieldSystem) {
    HMCutIn *cutIn = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(HMCutIn));

    memset(cutIn, 0, sizeof(HMCutIn));
    cutIn->fieldSystem = fieldSystem;
    return cutIn;
}
