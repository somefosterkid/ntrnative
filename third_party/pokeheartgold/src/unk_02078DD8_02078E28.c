#include "global.h"

#include "sys_task.h"
#include "unk_02078DD8.h"

SysTask *Heap_Free(SysTaskFunc callback, void *param, u32 priority);

SysTask *sub_02078E28(SysTaskFunc callback, void *param, int priority) {
    return Heap_Free(callback, param, priority);
}
