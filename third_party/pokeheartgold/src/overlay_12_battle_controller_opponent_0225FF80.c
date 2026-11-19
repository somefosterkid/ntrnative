#include "global.h"

#include "battle/battle_controller.h"

#include "heap.h"
#include "pokepic.h"
#include "sys_task_api.h"
#include "battle/battle_hp_bar.h"
#include "overlay_12_flicker_battler_data.h"

void ov12_0225FF80(SysTask *arg0, void *arg1);

void ov12_0225FFDC(SysTask *task, void *param);

void ov12_0225FF80(SysTask *task, void *ptr) {
    u8 v0;

    if ((u32) ((FlickerBattlerData *)ptr)->counter < 6U) {
        v0 = ((FlickerBattlerData *)ptr)->delay;
        if (v0 == 0) {
            ((FlickerBattlerData *)ptr)->delay = 2U;
            ((FlickerBattlerData *)ptr)->counter = (u8)(((FlickerBattlerData *)ptr)->counter + 1);
            Pokepic_SetAttr(((Pokepic *)((FlickerBattlerData *)ptr)->monSprite), 6, 1 ^ Pokepic_GetAttr(((Pokepic *)((FlickerBattlerData *)ptr)->monSprite), 6));
            return;
        }
        ((FlickerBattlerData *)ptr)->delay = (u8)(v0 - 1);
        return;
    }
    Pokepic_SetAttr(((Pokepic *)((FlickerBattlerData *)ptr)->monSprite), 6, 0);
    BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32) ((FlickerBattlerData *)ptr)->battler, 0x17);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}

void ov12_0225FFDC(SysTask *task, void *param) {
    BattleHpBar *hpBar = param;
    u8 v0;

    v0 = hpBar->script;
    switch (v0) {
    case 0:
        ov12_02264DCC(hpBar, hpBar->gainedHp);
        hpBar->script += 1;
    case 1:
        if (ov12_02264E00(hpBar) == -1) {
            hpBar->script += 1;
            return;
        }
        return;
    default:
        BattleController_EmitClearCommand(hpBar->battleSystem, (s32)hpBar->battlerId, (s32)hpBar->unk4C);
        hpBar->unk10 = NULL;
        SysTask_Destroy(task);
        break;
    }
}
