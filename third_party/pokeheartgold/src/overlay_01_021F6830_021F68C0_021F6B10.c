#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021F6830.h"
#include "overlay_27_ov27task_data.h"
#include "sys_task.h"

s32 ov01_021F6B10(FieldSystem *fieldSystem);

s32 ov01_021F6B10(FieldSystem *fieldSystem) {
    SubAppTaskData *data = SysTask_GetData(fieldSystem->unk_D8);
    return data->state;
}

struct FieldMenuManager **ov01_021F6B20(FieldSystem *fieldSystem);

struct FieldMenuManager **ov01_021F6B20(FieldSystem *fieldSystem) {
    SubAppTaskData *data = SysTask_GetData(fieldSystem->unk_D8);
    Ov27TaskData *taskData = SysTask_GetData(data->appTask);
    return &taskData->menu;
}
