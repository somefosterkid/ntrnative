#include "global.h"
#include "sys_task.h"

void ov85_021E8374(u8 *param_1, s32 *param_2);

void ov85_021E83C0(SysTask *task, void *param);

void ov85_021E83C0(SysTask *task, void *param) {
    u8 *param_2 = param;
    s32 iVar1;
    s32 *piVar2;

    piVar2 = (s32 *)(param_2 + 0xc4c);
    iVar1 = 0;
    do {
        ov85_021E8374(param_2, piVar2);
        iVar1 = iVar1 + 1;
        piVar2 = piVar2 + 4;
    } while (iVar1 < 5);
    return;
}
