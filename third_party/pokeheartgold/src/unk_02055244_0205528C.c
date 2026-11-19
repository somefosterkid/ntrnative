#include "global.h"

#include "task.h"

BOOL sub_0205528C(TaskManager *taskManager);

BOOL sub_0205528C(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);

    return !!sub_020505C8(fieldSystem);
}
