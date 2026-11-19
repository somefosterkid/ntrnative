#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "sys_task_api.h"

void ov12_02261EB8(BattleSystem *battleSys);

void ov12_02261ED4(BattleSystem *battleSys);

typedef struct YesNoMenuData {
    BattleSystem *battleSys;
    void *healthbox;
    int input;
    u8 command;
    u8 battler;
    u8 state;
    u8 yesNoType;
    int promptMsg;
    int nickname;
    u16 move;
    u16 msgIdx;
} YesNoMenuData;

void ov12_0225FCC0(SysTask *task, void *data) {
    YesNoMenuData *yesNoMenuData = data;
    u8 v1;

    if (ov12_0223BE0C(yesNoMenuData->battleSys, yesNoMenuData->battler, &v1) == 1) {
        ov12_02261ED4(yesNoMenuData->battleSys);
    }

    if (!v1 || (v1 != 0xFF && v1 != 1)) {
        ov12_02261EB8(yesNoMenuData->battleSys);
    }

    BattleController_EmitSelectedCommand(yesNoMenuData->battleSys, yesNoMenuData->battler, v1);
    BattleController_EmitClearCommand(yesNoMenuData->battleSys, yesNoMenuData->battler, yesNoMenuData->command);

    Heap_Free(data);
    SysTask_Destroy(task);
}
