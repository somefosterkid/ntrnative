#include "global.h"

#include "battle/battle_controller.h"
#include "battle/command_set_data.h"

#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "sys_task_api.h"

s32 ov10_022205BC(BattleSystem *, int);

void ov12_0225E104(SysTask *task, void *ptr) {
    CommandSetData *commandSetData = ptr;

    BattleController_EmitSelectedCommand(commandSetData->battleSys, (s32)commandSetData->battler, ov10_022205BC(commandSetData->battleSys, commandSetData->battler));
    BattleController_EmitClearCommand(commandSetData->battleSys, (s32)commandSetData->battler, (s32)commandSetData->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}

void ov12_0225E134(SysTask *task, void *ptr) {
    CommandSetData *commandSetData = ptr;

    BattleController_EmitClearCommand(commandSetData->battleSys, (s32)commandSetData->battler, (s32)commandSetData->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}
