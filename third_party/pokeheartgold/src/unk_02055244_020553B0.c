#include "global.h"

#include "field_types_def.h"
#include "task.h"
#include "unk_02055244.h"

BOOL sub_02055370(TaskManager *task);

void sub_020553B0(TaskManager *taskManager) {
    TaskManager_Call(taskManager, sub_02055370, NULL);
}
