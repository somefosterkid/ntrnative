#include "global.h"
#include "sys_task.h"

void ov45_0222A1F8(u32);

void ov45_0222CDC0(u32);

void ov45_02229FE0(SysTask *task, void *param);

void ov45_02229FE0(SysTask *task, void *param) {
    u8 *param_2 = param;
    ov45_0222CDC0(*(u32 *)(param_2 + 8));
    ov45_0222A1F8(*(u32 *)(param_2 + 4));
    return;
}
