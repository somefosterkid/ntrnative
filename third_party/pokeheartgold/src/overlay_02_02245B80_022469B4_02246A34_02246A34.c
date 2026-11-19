#include "global.h"

#include "heap.h"
#include "overlay_02.h"
#include "overlay_02_02245B80.h"
#include "sys_task_api.h"
#include "task.h"

SysTask *ov02_022494C4(FieldSystem *fieldSystem, Pokemon *mon, LocalMapObject *playerObject, LocalMapObject *followMonObject);

s32 ov02_02246A34(TaskManager *arg0);

s32 ov02_02246A34(TaskManager *taskManager) {
    s32 v0;
    FollowMonCutInTask *env;

    env = TaskManager_GetEnvironment(taskManager);
    v0 = env->state;
    switch (v0) {
    case 0:
        env->cutInTask = ov02_022494C4(env->fieldSystem, env->mon, env->playerObject, env->followMonObject);
        env->state = (s32)(env->state + 1);
    default:
        break;
    case 1:
        if (ov02_0224953C(env->cutInTask) != 0) {
            ov02_02249548(env->cutInTask);
            Heap_Free(env->mon);
            Heap_Free(env);
            return 1;
        }
        break;
    }
    return 0;
}
