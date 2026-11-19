#include "global.h"

#include "overlay_02.h"
#include "task.h"

void *ov02_02246304(FieldSystem *arg0);
BOOL ov02_02246330(TaskManager *taskman);

void ov02_022462E8(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    void *env = ov02_02246304(fieldSystem);
    TaskManager_Call(taskMan, ov02_02246330, env);
}
