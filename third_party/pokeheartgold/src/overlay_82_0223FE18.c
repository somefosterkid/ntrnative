#include "global.h"

#include "overlay_82.h"
#include "sys_task.h"

SysTask *YesNoPrompt_HandleInput(SysTaskFunc callback, void *param, u32 priority);

SysTask *ov82_0223FE18(SysTaskFunc callback, void *param, int priority) {
    return YesNoPrompt_HandleInput(callback, param, priority);
}
