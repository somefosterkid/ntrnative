#include "global.h"

#include "sys_task.h"
#include "unk_0202068C.h"

SysTask *sub_02020770(SysTaskFunc callback, void *param, u32 priority);

SysTask *sub_02020738(SysTaskFunc callback, void *param, int priority) {
    return sub_02020770(callback, param, priority);
}
