#include "global.h"

#include "heap.h"
#include "overlay_59_cutscene_tasks.h"
#include "overlay_59_juice_feed_app.h"
#include "pokepic.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov59_0223C09C(SysTask *param_1, void *param_2);

void ov59_0223C09C(SysTask *param_1, void *param_2) {
    Ov59ShakeTask *task = param_2;
    u8 bVar1;
    Ov59JuiceFeedApp *work;

    work = task->work;
    if ((s32) task->phase % 2 == 0) {
        Pokepic_AddAttr(work->monSprite->unk_20, 0xc, (u32) task->amount);
        Pokepic_AddAttr(work->monSprite->unk_20, 0xd, -(u32) task->amount);
    } else {
        Pokepic_AddAttr(work->monSprite->unk_20, 0xc, -(u32) task->amount);
        Pokepic_AddAttr(work->monSprite->unk_20, 0xd, (u32) task->amount);
    }
    bVar1 = task->counter;
    task->counter = bVar1 + 1;
    if (bVar1 >= 8) {
        task->counter = 0;
        bVar1 = task->phase;
        task->phase = bVar1 + 1;
        if (bVar1 >= 4) {
            Pokepic_SetAttr(work->monSprite->unk_20, 0xc, 0x100);
            Pokepic_SetAttr(work->monSprite->unk_20, 0xd, 0x100);
            work->taskCount = work->taskCount - 1;
            Heap_Free(task);
            SysTask_Destroy(param_1);
        }
    }
    return;
}
