#include "global.h"

#include "brightness.h"
#include "heap.h"
#include "sys_task_api.h"

s32 ov07_0221C69C(void);

s32 ov07_0221D2F4(void *, s32);

void ov07_0221D330(SysTask *arg0, void *arg1);

void ov07_0221D330(SysTask *task, void *ptr) {
    DoAllScreenBrightnessTransitionStep();
    if (IsBrightnessTransitionActive(1U) != 0) {
        if (((u32)(*(s32 *)((u8 *)ptr + 4) << 0x17) >> 0x1F) != 0) {
            Heap_Free(ptr);
            SysTask_Destroy(task);
            ov07_0221C69C();
            return;
        }
        ov07_0221D2F4(ptr, 0);
        *(s32 *)((u8 *)ptr + 4) = (s32)(0x100 | *(s32 *)((u8 *)ptr + 4));
    }
}
