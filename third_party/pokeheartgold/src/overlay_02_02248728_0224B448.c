#include "global.h"

#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov02_0224B448(SysTask *task);

void ov02_0224B448(SysTask *task) {
    void *data = SysTask_GetData(task);

    Heap_Free(data);
    SysTask_Destroy(task);
}
