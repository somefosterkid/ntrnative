#include "global.h"

#include "sys_task.h"
#include "sys_task_api.h"

#include "overlay_96_app_02210C98.h"

void ov96_02214234(Ov96App_02210C98 *param_1);

void ov96_02214258(Ov96App_02210C98 *param_1, s32 param_2, u8 param_3, u8 param_4);

void ov96_02214234(Ov96App_02210C98 *param_1) {
    u32 uVar1;

    uVar1 = 0;
    do {
        SysTask_Destroy(param_1->unk_7D4[uVar1].unk_4);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 4);
    return;
}

void ov96_02214258(Ov96App_02210C98 *param_1, s32 param_2, u8 param_3, u8 param_4) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    Ov96App_02210C98_Task *iVar1;

    param_1->unk_7D4[param_2].unk_0 = 1;
    iVar1 = &param_1->unk_7D4[param_2];
    iVar1->unk_E = param_3;
    iVar1->unk_F = param_4;
#else
    u8 *iVar1;

    *((u8 *)param_1 + (param_2 * 0x10) + 0x7d4) = 1;
    iVar1 = (u8 *)param_1 + (param_2 * 0x10);
    *(iVar1 + 0x7e2) = param_3;
    *(iVar1 + 0x7e3) = param_4;
#endif
    return;
}
