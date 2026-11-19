#include "global.h"

#include "brightness.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov12_02261E40(SysTask *arg0, void *param);

void ov12_02261E40(SysTask *arg0, void *param) {
    s32 *arg1 = param;
    s32 temp_r0;

    temp_r0 = *arg1;
    switch (temp_r0) {
    case 0:
        if (IsBrightnessTransitionActive(1U) == 0) {
            *arg1 = 2;
            return;
        }
        StartBrightnessTransition(4U, 0x10, 0, (GXBlendPlaneMask)(0x3D), 1U);
        *arg1 += 1;
        return;
    case 1:
        if (IsBrightnessTransitionActive(1U) == 1) {
            StartBrightnessTransition(4U, 0, 0x10, (GXBlendPlaneMask)(0x3D), 1U);
            *arg1 += 1;
            return;
        }
        return;
    case 2:
        if (IsBrightnessTransitionActive(1U) == 1) {
            Heap_Free(arg1);
            SysTask_Destroy(arg0);
        }
        break;
    }
}
