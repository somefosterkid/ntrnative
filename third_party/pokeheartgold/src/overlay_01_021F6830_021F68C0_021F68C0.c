#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021F6830.h"
#include "poke_overlay.h"
#include "sys_task.h"
#include "systask_environment.h"

BOOL ov01_021F6930(FieldSystem *fieldSystem);

FS_EXTERN_OVERLAY(OVY_27);

BOOL ov01_021F68C0(FieldSystem *fieldSystem);

BOOL ov01_021F68C0(FieldSystem *fieldSystem) {
    if (ov01_021F6930(fieldSystem) != 0) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_27));
        return 1;
    }
    return 0;
}

SysTask *ov01_021F68DC(BgConfig *bgConfig, void *touchInput, FieldSystem *fieldSystem);

SysTask *ov01_021F68DC(BgConfig *bgConfig, void *touchInput, FieldSystem *fieldSystem) {
    SysTask *task;
    SubAppTaskData *data;

    task = CreateSysTaskAndEnvironment(ov01_021F69C0, sizeof(SubAppTaskData), 10, HEAP_ID_FIELD1);
    data = SysTask_GetData(task);
    data->appIdx = fieldSystem->unk1C;
    data->state = 0;
    data->appTask = NULL;
    data->nextAppIdx = 0;
    data->fieldSystem = fieldSystem;
    data->args = NULL;
    return task;
}
