#include "global.h"
struct UnkStruct_ov104_0223C634;

#include "error_handling.h"
#include "heap.h"
#include "sys_task_api.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "overlay_80_frontier_anim.h"

void ov80_0222EEC8(SysTask *task, void *data);

void ov80_0222C7B0(s16 localID, UnkStruct_ov104_0223C634 *object, const FrontierAnimCmd *animCmd, struct UnkStruct_ov63_0222D77C_t *param3, u8 *movementCount, enum HeapID heapID);

struct ItemFieldUseContext;
typedef struct ItemFieldUseContext ItemFieldUseContext;
void *ov80_0222C80C(void *fieldSystem);
void FrontierScriptContext_Pause(ItemFieldUseContext *usageContext, void *param1);
BOOL FrtCmd_041(ItemFieldUseContext *usageContext);

void ov80_0222C7B0(s16 localID, UnkStruct_ov104_0223C634 *object, const FrontierAnimCmd *animCmd, struct UnkStruct_ov63_0222D77C_t *param3, u8 *movementCount, enum HeapID heapID) {
    FrontierAnimData *animData = Heap_Alloc(heapID, sizeof(FrontierAnimData));
    MI_CpuFill8(animData, 0, sizeof(FrontierAnimData));
    animData->frontierObj = object;
    animData->animCmd = animCmd;
    animData->unk_10 = param3;
    animData->localID = localID;
    animData->totalMovementCount = movementCount;
    if (object->movementTask != NULL) {
        GF_AssertFail();
    }
    object->movementTask = SysTask_CreateOnMainQueue(ov80_0222EEC8, animData, 0xEAC4U);
}

BOOL FrtCmd_041(ItemFieldUseContext *usageContext) {
    FrontierScriptContext_Pause(usageContext, ov80_0222C80C);
    return 1;
}
