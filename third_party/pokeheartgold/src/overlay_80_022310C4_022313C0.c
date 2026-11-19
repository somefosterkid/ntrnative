#include "global.h"

#include "overlay_80_022310C4.h"
#include "sys_task.h"

SysTask *ov80_022313C8(SysTaskFunc callback, void *param, u32 priority);

SysTask *ov80_022313C0(SysTaskFunc callback, void *param, int priority) {
    return ov80_022313C8(callback, param, priority);
}
