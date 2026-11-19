#include "global.h"

#include "sys_task.h"
#include "sys_task_api.h"

SysTask * ov102_021E93D4(SysTaskFunc func, void *data, u32 priority);

void ov102_021E93DC(SysTask *task, void *data);

SysTask * ov102_021E93D4(SysTaskFunc func, void *data, u32 priority) {
    return SysTask_CreateOnVBlankQueue(func, data, priority);
}

void ov102_021E93DC(SysTask *task, void *data) {
}
