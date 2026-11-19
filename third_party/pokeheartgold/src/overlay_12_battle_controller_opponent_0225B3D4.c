#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_display.h"

#include "sound.h"
#include "unk_02005D10.h"
#include "constants/heap.h"
#include "battle/battle.h"
#include "battle/battle_message_structs.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

typedef struct UnkStruct_ov12_0225B454_Heap_Alloc {
    BattleSystem *unk_0;
    u8 unk_4;
    u8 unk_5;
    s8 unk_6;
    u8 unk_7;
    s8 unk_8;
    u8 unk_9[0x3];
} UnkStruct_ov12_0225B454_Heap_Alloc;


void ov12_02260D84(SysTask *task, void *data);

void ov12_0225B3D4(BattleSystem *battleSystem, OpponentData *arg1, MoveHitSoundMessage *arg2) {
    s32 v1;
    u8 v0;

    if (arg1->battlerType & 1) {
        v1 = 0x75;
    } else {
        v1 = -0x75;
    }
    v0 = arg2->effectiveness;
    switch (v0) {
    case 0:
        sub_0200602C(0x6FEU, v1);
        break;
    case 2:
        sub_0200602C(0x6FCU, v1);
        break;
    case 1:
        sub_0200602C(0x6FDU, v1);
        break;
    }
    BattleController_EmitClearCommand(battleSystem, (s32)arg1->battler, (s32)arg2->command);
}

void ov12_0225B434(BattleSystem *battleSystem, OpponentData *arg1, MusicPlayMessage *arg2) {
    PlayBGM(arg2->bgmID);
    BattleController_EmitClearCommand(battleSystem, (s32)arg1->battler, (s32)arg2->command);
}

void ov12_0225B454(BattleSystem *arg0, OpponentData *arg1, Message_022645C8 *arg2) {
    UnkStruct_ov12_0225B454_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_BATTLE, sizeof(UnkStruct_ov12_0225B454_Heap_Alloc));
    MI_CpuFill8(v0, 0U, sizeof(UnkStruct_ov12_0225B454_Heap_Alloc));
    v0->unk_6 = 0;
    v0->unk_0 = arg0;
    v0->unk_4 = (u8)arg2->command;
    v0->unk_7 = (u8)arg2->unk1;
    v0->unk_8 = 0;
    v0->unk_5 = (u8)arg1->battler;
    SysTask_CreateOnMainQueue(ov12_02260D84, v0, 0U);
}
