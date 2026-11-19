#include "global.h"

#include "heap.h"
#include "overlay_02_02245B80.h"
#include "task.h"

s32 ov02_02246798(TaskManager *arg0);

s32 ov02_02246798(TaskManager *taskManager) {
    LegendCameraShakeTask *env;

    env = TaskManager_GetEnvironment(taskManager);
    if (ov02_0224663C(env->shake) == 0) {
        return 0;
    }
    ov02_0224662C(env->shake);
    ov02_02246534(env->shake);
    Heap_Free(env);
    return 1;
}
