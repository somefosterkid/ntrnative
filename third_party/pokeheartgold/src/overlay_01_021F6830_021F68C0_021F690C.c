#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021F6830.h"
#include "sys_task.h"
#include "systask_environment.h"

void ov01_021F690C(FieldSystem *fieldSystem);

void ov01_021F690C(FieldSystem *fieldSystem) {
    SubAppTaskData *data;

    data = SysTask_GetData(fieldSystem->unk_D8);
    ov01_02206C60[data->appIdx].start(fieldSystem->bgConfig, data->appTask);
    return;
}

void ov01_021F69A4(SubAppTaskData *data);

BOOL ov01_021F6930(FieldSystem *fieldSystem);

BOOL ov01_021F6930(FieldSystem *fieldSystem) {
    SubAppTaskData *data;
    s32 iVar2;

    data = SysTask_GetData(fieldSystem->unk_D8);
    iVar2 = ov01_02206C60[data->appIdx].main(fieldSystem->bgConfig);
    if (iVar2 == 1) {
        ov01_021F69A4(data);
        DestroySysTaskAndEnvironment(fieldSystem->unk_D8);
        return 1;
    }
    return 0;
}
