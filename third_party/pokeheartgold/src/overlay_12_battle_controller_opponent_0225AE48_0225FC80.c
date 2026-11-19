#include "global.h"

#include "battle/battle_controller.h"

#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "sys_task_api.h"
#include "overlay_12_menu_task_data.h"

void ov12_0225FC80(SysTask *task, void *ptr) {
    BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32)((YesNoMenuData *)ptr)->battler, (s32)((YesNoMenuData *)ptr)->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}

void ov12_0225FCA0(SysTask *task, void *ptr) {
    BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32)((YesNoMenuData *)ptr)->battler, (s32)((YesNoMenuData *)ptr)->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}
