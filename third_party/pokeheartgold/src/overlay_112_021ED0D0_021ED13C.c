#include "global.h"

#include "sys_task.h"
#include "sys_task_api.h"

void ov112_021ED13C(SysTask *param_1, void *param);

void ov112_021ED13C(SysTask *param_1, void *param) {
    u8 *param_2 = param;
    int new_var;
    *((s16 *)(param_2 + 0x1f2d6)) = (*((u16 *)(param_2 + 0x1f2d6))) + 2;
    if ((0x1E + 1) <= (*((u16 *)(param_2 + 0x1f2d6)))) {
        new_var = 0x1f2d6;
        *((u16 *)(param_2 + new_var)) = 0x1f;
    }
    G2_SetBlendAlpha(0, 8, (u32)(*((u16 *)(param_2 + 0x1f2d6))), 0x1f - ((u32)(*((u16 *)(param_2 + 0x1f2d6)))));
    if ((*((u16 *)(param_2 + 0x1f2d6))) == 0x1f) {
        SysTask_Destroy(param_1);
    }
    return;
}
