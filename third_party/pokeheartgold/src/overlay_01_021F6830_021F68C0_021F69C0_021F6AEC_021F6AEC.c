#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021F6830.h"
#include "sys_task.h"

s32 ov01_021F6AEC(FieldSystem *fieldSystem);

s32 ov01_021F6AEC(FieldSystem *fieldSystem) {
    SubAppTaskData *pvVar1;
    s32 *piVar2;

    pvVar1 = SysTask_GetData(fieldSystem->unk_D8);
    piVar2 = SysTask_GetData(pvVar1->appTask);
    return *piVar2;
}

s32 ov01_021F6B00(FieldSystem *fieldSystem);

s32 ov01_021F6B00(FieldSystem *fieldSystem) {
    SubAppTaskData *pbVar1;

    pbVar1 = SysTask_GetData(fieldSystem->unk_D8);
    return pbVar1->appIdx;
}
