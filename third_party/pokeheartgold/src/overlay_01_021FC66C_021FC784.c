#include "global.h"

#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov01_021FC784(SysTask *task);

void ov01_021FC784(SysTask *task) {
    void *data = SysTask_GetData(task);

    Heap_Free(data);
    SysTask_Destroy(task);
}
