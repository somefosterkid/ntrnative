#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "task.h"
#include "unk_02097B78.h"

s32 sub_02097CD8(TaskManager *taskManager);

void sub_02097CB4(FieldSystem *fieldSystem, LegendaryCinematicTask *task);

void sub_02097CB4(FieldSystem *fieldSystem, LegendaryCinematicTask *task) {
    LegendaryCinematicLauncher *env;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(LegendaryCinematicLauncher));
    env->task = task;
    env->state = 0;
    TaskManager_Call(fieldSystem->taskman, (TaskFunc)(sub_02097CD8), env);
    return;
}
