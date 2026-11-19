#include "global.h"

#include "heap.h"
#include "task.h"
#include "unk_02055244.h"
#include "unk_020552A4.h"
#include "legendary_cinematic_task.h"

void sub_02097CB4(FieldSystem *fieldSystem, LegendaryCinematicTask *task);

s32 sub_02097C50(TaskManager *arg0);

s32 sub_02097C50(TaskManager *taskManager) {
    FieldSystem *fieldSystem;
    u32 v0;
    LegendaryCinematicTask *env;

    fieldSystem = TaskManager_GetFieldSystem(taskManager);
    env = TaskManager_GetEnvironment(taskManager);
    v0 = env->state;
    switch (v0) {
    case 0:
        CallTask_LeaveOverworld(taskManager);
        env->state = (u32)(env->state + 1);
    default:
        break;
    case 1:
        sub_02097CB4(fieldSystem, env);
        env->state = (u32)(env->state + 1);
        break;
    case 2:
        CallTask_RestoreOverworld(taskManager);
        env->state = (u32)(env->state + 1);
        break;
    case 3:
        Heap_Free(env);
        return 1;
    }
    return 0;
}
