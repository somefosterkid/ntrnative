#include "global.h"

#include "sys_task_api.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "overlay_80_brightness_fade_task.h"
#include "overlay_01_unk_struct_ov83_0223_d9_ec.h"

s32 ov80_0223AD5C(s32, s32);

void ov80_0223ADA4(SysTask *arg0, void *arg1);

void ov80_0223ADB8(UnkStruct_ov83_0223D9EC *param0, fx32 param1, fx32 param2, int param3);

void ov80_0223ADA4(SysTask *task, void *arg1) {
    ov80_0223AD5C(*((s32 *)&((BrightnessFadeTask *)arg1)->screen), *(s32 *)arg1);
    SysTask_Destroy(task);
}

void ov80_0223ADB8(UnkStruct_ov83_0223D9EC *param0, fx32 param1, fx32 param2, int param3) {
    param0->unk_00 = param1;
    param0->unk_04 = param1;
    param0->unk_08 = param2 - param1;
    param0->unk_10 = param3;
    param0->unk_0C = 0;
}
