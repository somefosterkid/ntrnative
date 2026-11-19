#include "global.h"

#include "heap.h"
#include "sys_task_api.h"
#include "vram_transfer_manager.h"
#include "field_system.h"
#include "map_object_tex_tint_task.h"

void ov01_0220596C(SysTask *task, void *param);

void ov01_02205990(s32 a0, s32 x, s32 z, FollowMon *mon);

void ov01_0220596C(SysTask *task, void *param) {
    MapObjectTexTintTask *work = param;
    if (GF_GetNumPendingVramTransferTasks() == 0) {
        SysTask_Destroy(task);
        Heap_Free(work->resFile);
        Heap_Free(work);
    }
}

void ov01_02205990(s32 a0, s32 x, s32 z, FollowMon *mon) {
    mon->unk4 = a0;
    mon->unk8 = x;
    mon->unkC = z;
    if (mon->unk1C == 3) {
        mon->unk1C = 2;
        return;
    }
    if (mon->unk1C == 0) {
        mon->unk1C = 1;
    }
    return;
}
