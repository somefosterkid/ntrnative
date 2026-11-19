#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "sys_task_api.h"
#include "overlay_12_menu_task_data.h"


void ov12_02261EB8(BattleSystem *battleSys);
void ov12_02261ED4(BattleSystem *battleSys);

void ov12_0225E6FC(SysTask *task, void *ptr) {
    s32 command;

    command = ov12_022581D4(*(BattleSystem **)ptr, BattleSystem_GetBattleContext(*(BattleSystem **)ptr), 0xB, (s32)((TargetSelectMenuData *)ptr)->battler) + 1;
    if (command >= 5) {
        GF_AssertFail();
    }
    BattleController_EmitSelectedTarget(*(BattleSystem **)ptr, (s32)((TargetSelectMenuData *)ptr)->battler, command);
    BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32)((TargetSelectMenuData *)ptr)->battler, (s32)((TargetSelectMenuData *)ptr)->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}

void ov12_0225E740(SysTask *task, void *ptr) {
    BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32)((TargetSelectMenuData *)ptr)->battler, (s32)((TargetSelectMenuData *)ptr)->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}

void ov12_0225E760(SysTask *task, void *data) {
    TargetSelectMenuData *targetSelectMenuData = data;
    u8 v1;

    if (ov12_0223BE0C(targetSelectMenuData->battleSys, targetSelectMenuData->battler, &v1) == 1) {
        ov12_02261ED4(targetSelectMenuData->battleSys);
    }

    if (!v1 || v1 > 4) {
        ov12_02261EB8(targetSelectMenuData->battleSys);
    } else {
        int battler = v1 - 1;

        switch (targetSelectMenuData->range) {
        case 0x0:

            if (targetSelectMenuData->battler == battler) {
                ov12_02261EB8(targetSelectMenuData->battleSys);
            }
            break;
        case 0x200:

            if (BattleSystem_GetFieldSide(targetSelectMenuData->battleSys, targetSelectMenuData->battler) != BattleSystem_GetFieldSide(targetSelectMenuData->battleSys, battler)) {
                ov12_02261EB8(targetSelectMenuData->battleSys);
            }
            break;
        case 0x1:
        case 0x2:
        case 0x4:
        case 0x8:
        case 0x10:
        case 0x20:
        case 0x40:
        case 0x80:
        case 0x100:
        case 0x400:
            break;
        }
    }

    BattleController_EmitSelectedTarget(targetSelectMenuData->battleSys, targetSelectMenuData->battler, v1);
    BattleController_EmitClearCommand(targetSelectMenuData->battleSys, targetSelectMenuData->battler, targetSelectMenuData->command);

    Heap_Free(data);
    SysTask_Destroy(task);
}
