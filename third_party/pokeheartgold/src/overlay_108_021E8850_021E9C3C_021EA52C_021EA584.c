#include "global.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

#include "heap.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"

void ov108_021EA584(SafariDecorationApp *arg0, u8 arg1, s32 *arg2, u8 arg3) {
    u32 sp0;
    s32 *temp_r1;
    SafariDecorationSelectTask *temp_r0;

    sp0 = arg3;
    temp_r0 = Heap_Alloc(arg0->heapID, sizeof(SafariDecorationSelectTask));
    MI_CpuFill8(temp_r0, 0U, sizeof(SafariDecorationSelectTask));
    temp_r0->app = arg0;
    temp_r0->pendingTasks = arg2;
    temp_r0->slot = arg1;
    Sprite_SetAnimCtrlSeq(arg0->sprites[arg1 + 2], (arg1 * 3) + 6);
    PlaySE(0x5E1U);
    temp_r1 = temp_r0->pendingTasks;
    if (temp_r1 != NULL) {
        *temp_r1 += 1;
    }
    SysTask_CreateOnMainQueue(ov108_021EA5E4, temp_r0, sp0);
}
