#include "global.h"

#include "field_types_def.h"
#include "overlay_02_02245B80.h"
#include "task.h"

s32 ov02_02246818(TaskManager *taskman);

void OpenAlphHiddenRoom(TaskManager *man, u32 a1);

void OpenAlphHiddenRoom(TaskManager *man, u32 a1) {
    TaskManager_Call(man, (TaskFunc)(ov02_02246818), ov02_022467E8(TaskManager_GetFieldSystem(man), (u8)a1));
}
