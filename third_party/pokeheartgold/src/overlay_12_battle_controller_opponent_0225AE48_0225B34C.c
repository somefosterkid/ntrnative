#include "global.h"

#include "battle/battle_display.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "sys_task_api.h"

typedef struct UnkStruct_ov12_0225B34C_Heap_Alloc {
    BattleSystem *unk_0;
    s8 unk_4;
    u8 unk_5;
    s8 unk_6;
    u8 unk_7[0x1];
} UnkStruct_ov12_0225B34C_Heap_Alloc;

void ov12_022605D0(SysTask *task, void *data);

s32 ov12_02261AD4(BattleSystem *, OpponentData *, BattleMessage *);

void ov12_0225B34C(BattleSystem *battleSystem, OpponentData *arg1) {
    BattleMessage msg;
    MsgData *msgData;
    UnkStruct_ov12_0225B34C_Heap_Alloc *v0;

    ov12_02261AD4(battleSystem, arg1, &msg);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    v0 = Heap_Alloc(HEAP_ID_BATTLE, sizeof(UnkStruct_ov12_0225B34C_Heap_Alloc));
    v0->unk_0 = battleSystem;
    v0->unk_4 = 0x3D;
    v0->unk_5 = (u8)arg1->battler;
    v0->unk_6 = (s8)BattleSystem_PrintBattleMessage(battleSystem, msgData, &msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(ov12_022605D0, v0, 0U);
}
