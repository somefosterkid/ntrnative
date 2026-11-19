#include "global.h"
struct UnkStruct_ov104_0223C634;

#include "error_handling.h"
#include "heap.h"
#include "sys_task_api.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"

void ov80_0222EEC8(SysTask *task, void *data);

void ov80_0222C7B0(s16 arg0, struct UnkStruct_ov104_0223C634 *arg1, void *arg2, void *arg3, void *arg4, enum HeapID arg5);

struct ItemFieldUseContext;
typedef struct ItemFieldUseContext ItemFieldUseContext;
void *ov80_0222C80C(void *fieldSystem);
void FrontierScriptContext_Pause(ItemFieldUseContext *usageContext, void *param1);
BOOL FrtCmd_041(ItemFieldUseContext *usageContext);

void ov80_0222C7B0(s16 arg0, UnkStruct_ov104_0223C634 *arg1, void *arg2, void *arg3, void *arg4, enum HeapID heapID) {
    void *v0;
    void *v1;

    v0 = arg3;
    v1 = Heap_Alloc(heapID, 0x18U);
    MI_CpuFill8(v1, 0U, 0x18U);
    *(void **)((u8 *)v1 + 0x14) = arg1;
    *(void **)((u8 *)v1 + 0xC) = arg2;
    *(void **)((u8 *)v1 + 0x10) = v0;
    *(s16 *)((u8 *)v1 + 4) = arg0;
    *(void **)((u8 *)v1 + 8) = arg4;
    if (arg1->movementTask != NULL) {
        GF_AssertFail();
    }
    arg1->movementTask = SysTask_CreateOnMainQueue(ov80_0222EEC8, v1, 0xEAC4U);
}

BOOL FrtCmd_041(ItemFieldUseContext *usageContext) {
    FrontierScriptContext_Pause(usageContext, ov80_0222C80C);
    return 1;
}
