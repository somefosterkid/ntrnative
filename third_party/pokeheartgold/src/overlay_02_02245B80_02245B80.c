#include "global.h"

#include "overlay_02_02245B80.h"
#include "task.h"

void ov02_02245B80(TaskManager *taskManager) {
    FieldSystem *fieldSystem;
    LegendDemoScene *scene;

    fieldSystem = TaskManager_GetFieldSystem(taskManager);
    scene = ov02_02245B9C(fieldSystem);
    TaskManager_Call(taskManager, ov02_02245BC8, scene);
}
