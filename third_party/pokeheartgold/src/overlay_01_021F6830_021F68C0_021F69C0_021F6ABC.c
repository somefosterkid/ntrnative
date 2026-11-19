#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "overlay_01.h"
#include "overlay_01_021F6830.h"
#include "overlay_27_ov27task_data.h"
#include "sys_task.h"

void ov01_021F6ABC(FieldSystem *fieldSystem, int a1, int a2, void *a3) {
    void *sp0;
    Ov27TaskData *temp_r4;
    SubAppTaskData *data;

    sp0 = a3;
    data = SysTask_GetData(fieldSystem->unk_D8);
    temp_r4 = SysTask_GetData(data->appTask);
    if (a1 != ov01_021F6B00(fieldSystem)) {
        GF_AssertFail();
    }
    temp_r4->state = a2;
    temp_r4->result = sp0;
}
