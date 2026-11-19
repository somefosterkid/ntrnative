#include "global.h"

#include "sys_task.h"

SysTask *sub_02035DE0(SysTaskFunc callback, void *param, u32 priority);

SysTask *sub_02035ED0(SysTaskFunc callback, void *param, int priority);

SysTask *sub_02035ED0(SysTaskFunc callback, void *param, int priority) {
    return sub_02035DE0(callback, param, priority);
}
