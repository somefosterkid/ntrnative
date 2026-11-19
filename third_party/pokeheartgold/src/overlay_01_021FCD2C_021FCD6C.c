#include "global.h"

#include "overlay_01_021FCD2C.h"
#include "sys_task.h"

s32 ov01_021FCD6C(SysTask *task) {
    return *(s32 *)((u8 *)(SysTask_GetData(task)) + 8);
}
