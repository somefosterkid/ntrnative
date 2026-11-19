#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_display.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "sys_task_api.h"

typedef struct UnkStruct_ov12_0225AED8_Heap_Alloc {
    BattleSystem *unk_0;
    u8 unk_4;
    u8 unk_5;
    s8 unk_6;
    s8 unk_7;
    u8 unk_8[0x4];
} UnkStruct_ov12_0225AED8_Heap_Alloc;

void ov12_02260614(SysTask *task, void *data);

void ov12_0225AED8(BattleSystem *battleSystem, OpponentData *arg1, AlertMsgMessage *arg2) {
    MsgData *msgData;
    u8 v1;
    UnkStruct_ov12_0225AED8_Heap_Alloc *v0;

    v1 = arg1->bootState;
    if (v1 == 0) {
        msgData = BattleSystem_GetMessageLoader(battleSystem);
        v0 = Heap_Alloc(HEAP_ID_BATTLE, sizeof(UnkStruct_ov12_0225AED8_Heap_Alloc));
        v0->unk_0 = battleSystem;
        v0->unk_4 = (u8)arg2->command;
        v0->unk_5 = (u8)arg1->battler;
        v0->unk_7 = 0;
        v0->unk_6 = (s8)BattleSystem_PrintBattleMessage(battleSystem, msgData, &arg2->msg, BattleSystem_GetTextFrameDelay(battleSystem));
        SysTask_CreateOnMainQueue(ov12_02260614, v0, 0U);
        return;
    }
    if (v1 == 1) {
        BattleController_EmitAlertMessageAck(battleSystem, (s32)arg1->battler);
        BattleController_EmitClearCommand(battleSystem, (s32)arg1->battler, (s32)arg2->command);
        return;
    }
    if (!(BattleSystem_GetBattleType(battleSystem) & 4)) {
        BattleController_EmitAlertMessageAck(battleSystem, (s32)arg1->battler);
    }
    BattleController_EmitClearCommand(battleSystem, (s32)arg1->battler, (s32)arg2->command);
}
