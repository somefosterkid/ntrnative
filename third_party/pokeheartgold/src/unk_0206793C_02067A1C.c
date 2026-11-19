#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "heap.h"
#include "npc_trade.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "task.h"
#include "unk_0206793C.h"

BOOL Task_NPCTrade(TaskManager *param_1);

void CallTask_NPCTrade(TaskManager *taskManager, NPCTradeAppData *tradeWork, u16 arg, enum HeapID heapID) {
    NPCTradeTaskEnv *env;
    Pokemon *pPVar1;

    env = Heap_Alloc(heapID, sizeof(NPCTradeTaskEnv));
    memset(env, 0, sizeof(NPCTradeTaskEnv));
    env->state = 0;
    env->tradeWork = tradeWork;
    env->slot = arg;
    pPVar1 = AllocMonZeroed(heapID);
    env->myMon = pPVar1;
    pPVar1 = AllocMonZeroed(heapID);
    env->tradeMon = pPVar1;
    TaskManager_Call(taskManager, Task_NPCTrade, env);
    return;
}
