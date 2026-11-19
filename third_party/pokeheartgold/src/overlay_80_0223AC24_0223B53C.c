#include "global.h"

#include "overlay_80_0223AC24.h"
#include "sys_task.h"

SysTask *ov80_0223B504(SysTaskFunc callback, void *param, u32 priority);

SysTask *ov80_0223B53C(SysTaskFunc callback, void *param, int priority) {
    return ov80_0223B504(callback, param, priority);
}
