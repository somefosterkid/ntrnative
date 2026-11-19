#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_hp_bar.h"

#include "sys_task_api.h"

void ov12_0225DA8C(SysTask *task, void *param);

void ov12_0225DA8C(SysTask *task, void *param) {
    BattleHpBar *hpBar = param;
    u8 v0;

    v0 = hpBar->script;
    if (v0 == 0) {
        if (hpBar->unk_4F_1 == 1) {
            hpBar->script = v0 + 1;
        }
    } else {
        BattleController_EmitClearCommand(hpBar->battleSystem, (s32)hpBar->battlerId, (s32)hpBar->unk4C);
        hpBar->unk10 = NULL;
        SysTask_Destroy(task);
        BattleHpBar_SetEnabled(hpBar, 0);
    }
}
