// Black's code at ov021 0x021c6c70-0x021c6cbc. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_task.h"
#include "gfl/heap.h"


FieldTask *FieldTask_Create(HeapID heapId, u32 dataSize, FieldTaskCallback callback) {
    FieldTask *task;

    task = GFL_HeapAllocate(heapId, sizeof(FieldTask), 0, "field_task.c", 0x3b);
    task->state = 0;
    task->data = GFL_HeapAllocate(heapId, dataSize, 1, "field_task.c", 0x3d);
    task->callback = callback;
    return task;
}

void FieldTask_Free(FieldTask *task) {
    GFL_HeapFree(task->data);
    GFL_HeapFree(task);
}
