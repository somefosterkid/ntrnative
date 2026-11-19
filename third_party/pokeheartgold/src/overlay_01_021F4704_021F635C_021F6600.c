#include "global.h"

#include "field/field_control.h"

#include "overlay_01_021F4704.h"
#include "sys_task.h"

SoundplateStruct *ov01_021F6600(MapLoadManager *mapLoadManager, u8 mapSlot) {
    return (SoundplateStruct *)&mapLoadManager->maps[mapSlot]->soundplates;
}

void ov01_021F6614(SysTask *arg0);

void ov01_021F6614(SysTask *task) {
    MapModelLoadTask *loadTask = SysTask_GetData(task);
    loadTask->cancel = 1;
}
