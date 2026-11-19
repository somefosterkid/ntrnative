#include "global.h"

#include "overlay_01_021FC66C.h"
#include "sys_task.h"

s32 ov01_021FC76C(SysTask *task) {
    return *(s32 *)((u8 *)(SysTask_GetData(task)) + 4);
}

s32 ov01_021FC778(SysTask *task) {
    return *(s32 *)((u8 *)(SysTask_GetData(task)) + 8);
}
