#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "heap.h"
#include "launch_application.h"
#include "task.h"
#include "unk_02097B78.h"

s32 sub_02097CD8(TaskManager *taskManager);

s32 sub_02097CD8(TaskManager *taskManager) {
    FieldSystem *fieldSystem;
    LegendaryCinematicTask *task;
    LegendaryCinematicLauncher *env;
    u32 state;

    fieldSystem = TaskManager_GetFieldSystem(taskManager);
    env = TaskManager_GetEnvironment(taskManager);
    state = env->state;
    task = env->task;
    switch (state) {
    case 0:
        env->args = LegendaryCinematic_LaunchApp(fieldSystem, &task->unk_00, task->unk_48, task->unk_4c, HEAP_ID_FIELD2);
        env->state += 1;
        break;
    case 1:
        if (FieldSystem_ApplicationIsRunning(fieldSystem) == 0) {
            env->state += 1;
        }
        break;
    case 2:
        Heap_Free(env->args);
        Heap_Free(env);
        return 1;
    }
    return 0;
}
