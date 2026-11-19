#include "global.h"

#include "overlay_47.h"
#include "sys_task.h"

SysTask *ov47_02258A8C(SysTaskFunc callback, void *param, u32 priority);

SysTask *ov47_02258A14(SysTaskFunc callback, void *param, int priority) {
    return ov47_02258A8C(callback, param, priority);
}
