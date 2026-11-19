#include "global.h"
#include "screen_fade_internal.h"
#include "sys_task.h"

struct SysTask;
struct SysTaskQueue;
void sub_02013364(int left, int top, int right, int bottom, enum HardwareWindow windowID, enum DSScreen screen);
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
void SysTask_Destroy(SysTask *task);

void sub_02013504(SysTask *task, void *data);

void sub_02013504(SysTask *task, void *data) {
    HardwareWindowDimensions *dimensions = data;
    sub_02013364(
        dimensions->left,
        dimensions->top,
        dimensions->right,
        dimensions->bottom,
        dimensions->windowID,
        dimensions->screen);
    SysTask_Destroy(task);
}
