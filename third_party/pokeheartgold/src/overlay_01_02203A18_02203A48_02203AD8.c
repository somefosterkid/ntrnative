#include "global.h"

#include "heap.h"
#include "overlay_01_02203A18.h"
#include "overlay_01_021F1348.h"
#include "task.h"

void *ov01_02203A48(LocalMapObject *, s32);

s32 ov01_02203AD8(TaskManager *arg0);

s32 ov01_02203AD8(TaskManager *taskManager) {
    u32 *v1;
    u32 v0;
    UnkOv01_02203AB4Env *ptr;
    UnkOv01_02203A18 *ctx;

    v1 = TaskManager_GetStatePtr(taskManager);
    ptr = TaskManager_GetEnvironment(taskManager);
    v0 = *v1;
    switch (v0) {
    case 0:
        ov01_02203A48(ptr->partnerPokeObj, ptr->arg);
        *v1 += 1;
    default:
        break;
    case 1:
        ctx = ov01_021F1450(ov01_021F146C(ptr->partnerPokeObj), 0x12);
        if (ctx->busy == 0) {
            Heap_Free(ptr);
            return 1;
        }
        break;
    }
    return 0;
}
