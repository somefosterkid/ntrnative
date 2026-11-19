#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021F6830.h"
#include "sys_task.h"

void ov01_021F6A9C(FieldSystem *fieldSystem, s32 a1, void *a2);

void ov01_021F6A9C(FieldSystem *fieldSystem, s32 a1, void *a2) {
    SubAppTaskData *temp_r0;

    temp_r0 = SysTask_GetData(fieldSystem->unk_D8);
    if (temp_r0->state == 1) {
        temp_r0->state = 2U;
        temp_r0->nextAppIdx = (s8)a1;
        temp_r0->args = a2;
    }
}
