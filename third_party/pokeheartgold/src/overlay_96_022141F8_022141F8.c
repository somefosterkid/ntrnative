#include "global.h"

#include "sys_task.h"
#include "sys_task_api.h"

#include "overlay_96_app_02210C98.h"

void ov96_02214278(SysTask *task, void *param);

void ov96_022141F8(Ov96App_02210C98 *param_1);

void ov96_022141F8(Ov96App_02210C98 *param_1) {
    SysTask *pSVar1;
    u32 uVar2;
    Ov96App_02210C98_Task *data;

    uVar2 = 0;
    do {
        data = &param_1->unk_7D4[uVar2];
        data->unk_0 = 0;
        data->unk_1 = (s8)uVar2;
        data->unk_2 = 0;
        data->unk_3 = -1;
        data->unk_8 = param_1;
        pSVar1 = SysTask_CreateOnMainQueue(ov96_02214278, data, 1);
        data->unk_4 = pSVar1;
        uVar2 = uVar2 + 1 & 0xff;
    } while (uVar2 < 4);
    return;
}
