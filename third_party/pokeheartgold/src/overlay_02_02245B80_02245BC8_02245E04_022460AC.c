#include "global.h"

#include "field_types_def.h"
#include "overlay_02.h"
#include "task.h"

void *ov02_022460CC(FieldSystem *, s16);

BOOL ov02_022460FC(TaskManager *param_1);

void ov02_022460AC(TaskManager *taskMan, u8 a1) {
    TaskManager_Call(taskMan, ov02_022460FC, ov02_022460CC(TaskManager_GetFieldSystem(taskMan), a1));
}
