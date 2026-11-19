#include "global.h"
#include "sys_task.h"

struct SysTask;
struct SysTaskQueue;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
SysTask *ov46_022592E0(SysTaskFunc callback, void *param, u32 priority);

SysTask *ov46_02258F70(SysTaskFunc callback, void *param, int priority);

SysTask *ov46_02258F70(SysTaskFunc callback, void *param, int priority)
{
    return ov46_022592E0(callback, param, priority);
}
