#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"

#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "sys_task_api.h"
#include "overlay_12_menu_task_data.h"

void ov12_0225E4CC(SysTask *task, void *ptr) {
    BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32)((MoveSelectMenuData *)ptr)->battler, (s32)((MoveSelectMenuData *)ptr)->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}

s32 ov12_02261EB8(BattleSystem *);

s32 ov12_02261ED4(BattleSystem *);

void ov12_0225E4EC(SysTask *task, void *ptr) {
    u8 v0;
    u16 v1;
    u8 v2;

    if (ov12_0223BE0C(*(BattleSystem **)ptr, (s32)((MoveSelectMenuData *)ptr)->battler, &v0) == 1) {
        ov12_02261ED4(*(BattleSystem **)ptr);
    }
    v2 = v0;
    if ((v2 == 0) || ((u32)v2 > 4U)) {
        ov12_02261EB8(*(BattleSystem **)ptr);
    } else {
        v1 = (u16)BattleMon_Get(BattleSystem_GetBattleContext(*(BattleSystem **)ptr), (s32)((MoveSelectMenuData *)ptr)->battler, v2 + 5, NULL);
        if ((v1 == 0) || ((u32)v1 > 0x1D3U)) {
            ov12_02261EB8(*(BattleSystem **)ptr);
        }
    }
    BattleController_EmitSelectedMove(*(BattleSystem **)ptr, (s32)((MoveSelectMenuData *)ptr)->battler, (s32)v0);
    BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32)((MoveSelectMenuData *)ptr)->battler, (s32)((MoveSelectMenuData *)ptr)->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}
