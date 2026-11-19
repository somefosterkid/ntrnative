#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"
#include "battle/command_set_data.h"

#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "sys_task_api.h"

void ov12_0225E1D4(SysTask *task, void *ptr) {
    CommandSetData *commandSetData = ptr;

    BattleController_EmitSelectedCommand(commandSetData->battleSys, (s32)commandSetData->battler, 4);
    BattleController_EmitClearCommand(commandSetData->battleSys, (s32)commandSetData->battler, (s32)commandSetData->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}

s32 ov12_02261EB8(BattleSystem *);

s32 ov12_02261ED4(BattleSystem *);

void ov12_0225E1FC(SysTask *task, void *ptr) {
    CommandSetData *commandSetData = ptr;
    u8 v0;
    u8 v1;

    if (ov12_0223BE0C(commandSetData->battleSys, (s32)commandSetData->battler, &v0) == 1) {
        ov12_02261ED4(commandSetData->battleSys);
    }
    v1 = v0;
    if ((v1 == 0) || ((u32)v1 > 4U) || (v1 == 2)) {
        ov12_02261EB8(commandSetData->battleSys);
    }
    BattleController_EmitSelectedCommand(commandSetData->battleSys, (s32)commandSetData->battler, (s32)v0);
    BattleController_EmitClearCommand(commandSetData->battleSys, (s32)commandSetData->battler, (s32)commandSetData->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}
