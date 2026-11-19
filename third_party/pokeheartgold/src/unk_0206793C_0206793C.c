#include "global.h"

#include "field_types_def.h"
#include "heap.h"
#include "npc_trade.h"
#include "pokemon_types_def.h"
#include "task.h"
#include "trade_anim.h"
#include "unk_02055244.h"
#include "unk_020552A4.h"
#include "unk_0206793C.h"

extern OverlayManagerTemplate _020FE79C;

BOOL Task_NPCTrade(TaskManager *param_1);

void sub_0206793C(TaskManager *taskManager) {
    NPCTradeTaskEnv *env;

    TaskManager_GetFieldSystem(taskManager);
    env = TaskManager_GetEnvironment(taskManager);
    CallApplicationAsTask(taskManager, &_020FE79C, &env->tradeAnim);
}

BOOL Task_NPCTrade(TaskManager *param_1) {
    FieldSystem *fieldSystem;
    NPCTradeTaskEnv *ptr;

    fieldSystem = TaskManager_GetFieldSystem(param_1);
    ptr = TaskManager_GetEnvironment(param_1);
    switch (ptr->state) {
    case 0:
        NPCTrade_CreateTradeAnim(fieldSystem, ptr->tradeWork, ptr->slot, &ptr->tradeAnim, ptr->myMon, ptr->tradeMon);
        NPCTrade_ReceiveMonToSlot(fieldSystem, ptr->tradeWork, ptr->slot);
        ptr->state = ptr->state + 1;
        break;
    case 1:
        PaletteFadeUntilFinished(param_1);
        ptr->state = ptr->state + 1;
        break;
    case 2:
        CallTask_LeaveOverworld(param_1);
        ptr->state = ptr->state + 1;
        break;
    case 3:
        sub_0206793C(param_1);
        ptr->state = ptr->state + 1;
        break;
    case 4:
        CallTask_RestoreOverworld(param_1);
        ptr->state = ptr->state + 1;
        break;
    case 5:
        CallTask_FadeFromBlack(param_1);
        ptr->state = ptr->state + 1;
        break;
    case 6:
        Heap_Free(ptr->myMon);
        Heap_Free(ptr->tradeMon);
        Heap_Free(ptr);
        return 1;
    }
    return 0;
}
