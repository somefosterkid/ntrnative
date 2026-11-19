#include "global.h"

#include "field_types_def.h"
#include "overlay_02_02245B80.h"
#include "task.h"

s32 ov02_02246A34(TaskManager *taskman);

void ov02_022469B4(TaskManager *man, LocalMapObject *obj1, LocalMapObject *obj2);

void ov02_022469B4(TaskManager *man, LocalMapObject *obj1, LocalMapObject *obj2) {
    TaskManager_Call(man, (TaskFunc)(ov02_02246A34), ov02_022469D8(TaskManager_GetFieldSystem(man), obj1, obj2));
}
