#include "global.h"

#include "field_types_def.h"
#include "overlay_02.h"
#include "task.h"


s32 ov02_0224E0EC(TaskManager *taskman);

void ov02_0224E0BC(LocalMapObject *obj1, LocalMapObject *obj2, TaskManager *taskManager);

void ov02_0224E0BC(LocalMapObject *obj1, LocalMapObject *obj2, TaskManager *taskManager) {
    TaskManager_Call(taskManager, (TaskFunc)(ov02_0224E0EC), ov02_0224E0D4(obj1, obj2));
}
