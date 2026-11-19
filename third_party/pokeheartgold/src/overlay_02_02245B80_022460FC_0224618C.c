#include "global.h"

#include "field_types_def.h"
#include "overlay_02.h"
#include "task.h"

void *ov02_022461AC(FieldSystem *, u8);

BOOL ov02_022461DC(TaskManager *param_1);

void ShowLegendaryWing(TaskManager *taskMan, u8 a1) {
    TaskManager_Call(taskMan, ov02_022461DC, ov02_022461AC(TaskManager_GetFieldSystem(taskMan), a1));
}
