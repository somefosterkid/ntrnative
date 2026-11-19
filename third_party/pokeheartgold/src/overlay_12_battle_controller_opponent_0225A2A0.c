#include "global.h"

#include "battle/overlay_12_0225A2A0.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_display.h"
#include "battle/battle_message_structs.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "pokemon.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "trainer_data.h"

void ov12_0225D138(SysTask *task, void *data);

void ov12_0225D644(SysTask *task, void *data);

u8 ov12_02261EF0(BattleSystem *, s32, u8);

void ov12_0225A2A0(BattleSystem *battleSystem, OpponentData *arg1, TrainerThrowBallMessage *arg2) {
    Trainer *trainer;
    s32 v1;
    UnkStruct_ov12_0225A2A0 *v0;

    v0 = Heap_Alloc(HEAP_ID_BATTLE, sizeof(UnkStruct_ov12_0225A2A0));
    v1 = 0;
    v0->unkA = 0;
    v0->battleSystem = battleSystem;
    v0->unk8 = (u8)arg2->command;
    v0->unk10 = (s32)arg2->ballTypeIn;
    v0->unk9 = (u8)arg1->battler;
    v0->unk4 = arg1;
    if (1 & arg1->battlerType) {
        v0->unkB = 2;
    } else {
        trainer = BattleSystem_GetTrainer(battleSystem, (s32)arg1->battler);
        v0->unkB = 0;
        v1 = TrainerClassToBackpicID(ov12_02261EF0(battleSystem, arg1->battler, trainer->data.trainerClass), 0);
    }
    v0->unkC = v1;
    BattleSystem_GetBattleType(battleSystem);
    if (ov12_0223C140(battleSystem, (u32)arg1->battler) != 0xFF) {
        SysTask_CreateOnMainQueue(ov12_0225D644, v0, 0U);
        return;
    }
    SysTask_CreateOnMainQueue(ov12_0225D138, v0, 0U);
}
