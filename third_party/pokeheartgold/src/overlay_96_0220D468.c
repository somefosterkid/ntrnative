#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "error_handling.h"
#include "sys_task.h"
#include "sys_task_api.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021EB10C(u32 *, s32, f32);
#else
s32 ov96_021EB10C(u32 *, s32, f32);
#endif

void ov96_0220D468(SysTask *arg0, void *arg1);

void ov96_0220D468(SysTask *task, void *arg1);
void ov96_0220D52C(UnkStruct_ov96_0220E3F0_ScaleAnim *param_1, void *param_2);

void ov96_0220D468(SysTask *task, void *data) {
    UnkStruct_ov96_0220E3F0_ScaleAnim *arg1 = data;
    u16 v0;

    v0 = arg1->phase;
    switch (v0) {
    case 0:
        arg1->scale = (f32)(1.0 - (0.3 * (double)arg1->counter));
        if ((u32)arg1->counter > 1U) {
            arg1->counter = 0U;
            arg1->phase = (u16)(arg1->phase + 1);
            return;
        }
        ov96_021EB10C(arg1->unk_0, 0x3F800000, arg1->scale);
        arg1->counter = (u16)(arg1->counter + 1);
        return;
    case 1:
        arg1->scale = (f32)(1.0 + (0.3 * (double)arg1->counter));
        if ((u32)arg1->counter > 1U) {
            arg1->task = NULL;
            ov96_021EB10C(arg1->unk_0, 0x3F800000, 1.0f);
            SysTask_Destroy(task);
            return;
        }
        ov96_021EB10C(arg1->unk_0, 0x3F800000, arg1->scale);
        arg1->counter = (u16)(arg1->counter + 1);
        return;
    default:
        GF_AssertFail();
        return;
    }
}

void ov96_0220D52C(UnkStruct_ov96_0220E3F0_ScaleAnim *param_1, void *param_2) {
    SysTask *pSVar1;

    if (param_1->task != NULL) {
        SysTask_Destroy(param_1->task);
    }
    param_1->unk_0 = param_2;
    param_1->counter = 0;
    param_1->phase = 0;
    pSVar1 = SysTask_CreateOnMainQueue(ov96_0220D468, param_1, 0);
    param_1->task = pSVar1;
    return;
}
