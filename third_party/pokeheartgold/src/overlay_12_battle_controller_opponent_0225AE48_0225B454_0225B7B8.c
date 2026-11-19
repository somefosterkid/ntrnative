#include "global.h"

#include "battle/battle.h"
#include "battle/battle_controller.h"
#include "battle/battle_message_structs.h"
#include "battle/battle_system.h"

#include "filesystem.h"
#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "overlay_12_mon_encounter_data.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02016EDC.h"

s32 ov07_0221C394(void *arg0);

s32 ov07_0221C3B0(void *arg0);

s32 ov07_0221C3C0(void *arg0);

s32 ov12_02261B80(BattleSystem *, OpponentData *, u32 *, MoveAnimation *);

s32 sub_02005B58(u8);

void ov12_0225B7B8(SysTask *task, void *data);

void ov12_0225B7B8(SysTask *task, void *data) {
    MonEncounterData *monEncounterData = data;
    MoveAnimation sp18;
    s16 x, y;
    u32 *temp_r0;
    u32 *temp_r5;
    u8 temp_r1_2;

    temp_r0 = ov12_0223A8DC(monEncounterData->battleSys);
    temp_r5 = temp_r0;
    switch (monEncounterData->state) {
    case 0:
        monEncounterData->delay = 0x1C;
        monEncounterData->state++;
    case 1:
        if (--monEncounterData->delay != 0) {
            return;
        }
        monEncounterData->state++;
    case 2:
        ManagedSprite_GetPositionXY(monEncounterData->terrain->unk0, &x, &y);
        if ((monEncounterData->battlerType == 1) || (monEncounterData->battlerType == 3)) {
            if (x < 0xC0) {
                ManagedSprite_OffsetPositionXY(monEncounterData->terrain->unk0, 8, 0);
            } else {
                ManagedSprite_SetPositionXY(monEncounterData->terrain->unk0, 0xC0, 0x58);
            }
        }
        ManagedSprite_GetPositionXY(monEncounterData->terrain->unk0, &x, &y);
        Pokepic_AddAttr(monEncounterData->sprite, 1, 4);
        if (Pokepic_GetAttr(monEncounterData->sprite, 1) >= monEncounterData->targetPos) {
            Pokepic_SetAttr(monEncounterData->sprite, 0x2C, 0);
            Pokepic_SetAttr(monEncounterData->sprite, 0x2D, 0);
            Pokepic_SetAttr(monEncounterData->sprite, 1, monEncounterData->targetPos);
            ov12_02261F38(monEncounterData->battleSys, monEncounterData->battler, monEncounterData->battlerType, monEncounterData->sprite, monEncounterData->battlerData->narc, monEncounterData->species, monEncounterData->formNum, monEncounterData->face, monEncounterData->cryMod);
            ManagedSprite_SetPositionXY(monEncounterData->terrain->unk0, 0xC0, 0x58);
            Pokepic_StartPaletteFade(monEncounterData->sprite, 8, 0, 0, 0);
            monEncounterData->state++;
            return;
        }
        return;
    case 3:
        if ((sub_02017068(ov12_0223B750(monEncounterData->battleSys), monEncounterData->battler) == 1) && (Pokepic_IsAnimActive(monEncounterData->sprite) == 0)) {
            if (monEncounterData->isShiny != 0) {
                temp_r1_2 = monEncounterData->battler;
                BattleController_SetMoveAnimation(monEncounterData->battleSys, NULL, &sp18, 1, 0xB, (s32)temp_r1_2, (s32)temp_r1_2, 0U);
                ov12_02261B80(monEncounterData->battleSys, monEncounterData->battlerData, temp_r5, &sp18);
                monEncounterData->state = 4U;
                return;
            }
            monEncounterData->state = 0xFFU;
            return;
        }
        break;
    case 4:
        ov07_0221C394(temp_r0);
        if (ov07_0221C3B0(temp_r5) == 0) {
            ov07_0221C3C0(temp_r5);
            monEncounterData->state = 0xFFU;
            return;
        }
        break;
    default:
        sub_02005B58(0);
        BattleController_EmitClearCommand(monEncounterData->battleSys, monEncounterData->battler, monEncounterData->command);
        Heap_Free(monEncounterData);
        SysTask_Destroy(task);
        break;
    }
}
