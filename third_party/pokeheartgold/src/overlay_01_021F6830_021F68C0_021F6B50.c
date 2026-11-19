#include "global.h"

#include "error_handling.h"
#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021F6830.h"
#include "sys_task.h"

s32 ov27_0225A2CC(SysTask *);

void ov01_021F6B50(FieldSystem *fieldSystem);

void ov01_021F6B50(FieldSystem *fieldSystem) {
    SubAppTaskData *data = SysTask_GetData(fieldSystem->unk_D8);
    ov27_0225A2CC(data->appTask);
}

s32 ov27_0225A2EC(SysTask *, s32);

void ov01_021F6B64(FieldSystem *fieldSystem, int arg1);

void ov01_021F6B64(FieldSystem *fieldSystem, int arg1) {
    SubAppTaskData *v0;

    v0 = SysTask_GetData(fieldSystem->unk_D8);
    if ((arg1 <= 0) || (arg1 >= 3)) {
        GF_AssertFail();
    }
    ov27_0225A2EC(v0->appTask, arg1);
}
