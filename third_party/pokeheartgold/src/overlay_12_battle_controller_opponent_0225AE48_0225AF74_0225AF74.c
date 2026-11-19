#include "global.h"

#include "battle/battle.h"
#include "battle/battle_controller.h"
#include "battle/battle_message_structs.h"
#include "battle/battle_system.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "overlay_07_ball_rotation.h"
#include "overlay_12_set_mosaic_data.h"
#include "battle/battle_party_context.h"
#include "battle/battle_hp_bar.h"

struct UnkBallData;

struct BattleMonOBJData;



void ov12_0226498C(BattleHpBar *healthbox, u32 hp, u32 flags);







typedef struct BattleMonOBJData BattleMonOBJData;

void ov12_0225AF74(BattleSystem *battleSys, OpponentData *battlerData, RefreshHPGaugeMessage *message);

void ov12_0225AF74(BattleSystem *battleSys, OpponentData *battlerData, RefreshHPGaugeMessage *message) {
    BattleHpBar *healthbox = &battlerData->healthbox;

    MI_CpuClearFast(&healthbox->script, sizeof(u8));

    healthbox->battleSystem = battleSys;
    healthbox->battlerId = battlerData->battler;
    healthbox->type = BattleHpBar_Util_GetBarTypeFromBattlerSide(battlerData->battlerType, BattleSystem_GetBattleType(battleSys));
    healthbox->unk4C = message->command;
    healthbox->hp = message->curHP;
    healthbox->maxHp = message->maxHP;
    healthbox->level = message->level;
    healthbox->unk49 = message->gender;
    healthbox->gainedHp = 0;
    healthbox->exp = message->curExp;
    healthbox->maxExp = message->maxExp;
    healthbox->monId = message->partySlot;
    healthbox->unk_4A = message->status;
    healthbox->unk4B = message->caughtSpecies;
    healthbox->unk27 = message->numSafariBalls;

    ov12_0226498C(healthbox, healthbox->hp, ~(1 << 5));
    BattleController_EmitClearCommand(healthbox->battleSystem, healthbox->battlerId, healthbox->unk4C);
}

struct BattlePartyContext;

void ov12_022609F8(SysTask *task, void *data);

typedef struct ForgetMoveData {
    BattleSystem *battleSys;
    BattlePartyContext *battlePartyCtx;
    u8 command;
    u8 battler;
    u8 state;
    u8 unused_0B;
    u16 move;
    u8 slot;
    u8 unused_0F;
} ForgetMoveData;

void ov12_0225B028(BattleSystem *battleSys, OpponentData *battlerData, ForgetMoveMessage *message);

void ov12_0225B028(BattleSystem *battleSys, OpponentData *battlerData, ForgetMoveMessage *message) {
    ForgetMoveData *forgetMoveData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(ForgetMoveData));

    forgetMoveData->state = 0;
    forgetMoveData->battleSys = battleSys;
    forgetMoveData->command = message->command;
    forgetMoveData->battler = battlerData->battler;
    forgetMoveData->move = message->move;
    forgetMoveData->slot = message->slot;

    SysTask_CreateOnMainQueue(ov12_022609F8, forgetMoveData, 0);
}

void ov12_02260B30(SysTask *task, void *data);

void ov12_0225B060(BattleSystem *battleSys, OpponentData *battlerData, MosaicSetMessage *message);

void ov12_0225B060(BattleSystem *battleSys, OpponentData *battlerData, MosaicSetMessage *message) {
    SetMosaicData *setMosaicData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(SetMosaicData));

    setMosaicData->state = 0;
    setMosaicData->battleSys = battleSys;
    setMosaicData->monSprite = battlerData->monSprite;
    setMosaicData->command = message->command;
    setMosaicData->battler = battlerData->battler;
    setMosaicData->intensity = message->intensity;
    setMosaicData->counter = 0;
    setMosaicData->wait = message->wait;

    SysTask_CreateOnMainQueue(ov12_02260B30, setMosaicData, 0);
}

void ov12_02260BA0(SysTask *task, void *data);

typedef struct PartyGaugeTask {
    BattleSystem *battleSys;
    u8 command;
    u8 battler;
    u8 battlerType;
    u8 state;
    u8 status[6];
    u8 midBattle;
} PartyGaugeTask;

void ov12_0225B0A0(BattleSystem *battleSys, OpponentData *battlerData, PartyGaugeData *partyGauge);

void ov12_0225B0A0(BattleSystem *battleSys, OpponentData *battlerData, PartyGaugeData *partyGauge) {
    PartyGaugeTask *task = Heap_Alloc(HEAP_ID_BATTLE, sizeof(PartyGaugeTask));

    task->state = 0;
    task->battleSys = battleSys;
    task->command = partyGauge->command;
    task->battler = battlerData->battler;
    task->battlerType = battlerData->battlerType;

    for (int i = 0; i < 6; i++) {
        task->status[i] = partyGauge->status[i];
    }

    task->midBattle = 0;
    SysTask_CreateOnMainQueue(ov12_02260BA0, task, 0);
}

void ov12_02260C58(SysTask *task, void *data);

void ov12_0225B0E8(BattleSystem *battleSys, OpponentData *battlerData, PartyGaugeData *partyGauge);

void ov12_0225B0E8(BattleSystem *battleSys, OpponentData *battlerData, PartyGaugeData *partyGauge) {
    PartyGaugeTask *task = Heap_Alloc(HEAP_ID_BATTLE, sizeof(PartyGaugeTask));

    task->state = 0;
    task->battleSys = battleSys;
    task->command = partyGauge->command;
    task->battler = battlerData->battler;
    task->battlerType = battlerData->battlerType;
    task->midBattle = FALSE;

    SysTask_CreateOnMainQueue(ov12_02260C58, task, 0);
}

void ov12_0225B120(BattleSystem *battleSys, OpponentData *battlerData, PartyGaugeData *partyGauge);

void ov12_0225B120(BattleSystem *battleSys, OpponentData *battlerData, PartyGaugeData *partyGauge) {
    PartyGaugeTask *task = Heap_Alloc(HEAP_ID_BATTLE, sizeof(PartyGaugeTask));

    task->state = 0;
    task->battleSys = battleSys;
    task->command = partyGauge->command;
    task->battler = battlerData->battler;
    task->battlerType = battlerData->battlerType;

    for (int i = 0; i < 6; i++) {
        task->status[i] = partyGauge->status[i];
    }

    task->midBattle = 1;
    SysTask_CreateOnMainQueue(ov12_02260BA0, task, 0);
}

void ov12_0225B16C(BattleSystem *battleSys, OpponentData *battlerData, PartyGaugeData *partyGauge);

void ov12_0225B16C(BattleSystem *battleSys, OpponentData *battlerData, PartyGaugeData *partyGauge) {
    PartyGaugeTask *task = Heap_Alloc(HEAP_ID_BATTLE, sizeof(PartyGaugeTask));

    task->state = 0;
    task->battleSys = battleSys;
    task->command = partyGauge->command;
    task->battler = battlerData->battler;
    task->battlerType = battlerData->battlerType;
    task->midBattle = TRUE;

    SysTask_CreateOnMainQueue(ov12_02260C58, task, 0);
}
