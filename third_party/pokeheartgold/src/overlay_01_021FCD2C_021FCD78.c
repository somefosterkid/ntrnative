#include "global.h"

#include "heap.h"
#include "overlay_01_021FCD2C.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov01_021FCD78(SysTask *task) {
    void *data = SysTask_GetData(task);

    Heap_Free(data);
    SysTask_Destroy(task);
}
