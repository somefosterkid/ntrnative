#include "global.h"

#include "field_types_def.h"
#include "task.h"
#include "unk_02055244.h"

s32 sub_020553C0(TaskManager *taskman);

void sub_02055408(TaskManager *taskManager) {
    TaskManager_Call(taskManager, (TaskFunc)(sub_020553C0), NULL);
}
