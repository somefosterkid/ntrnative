#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_display.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "sys_task_api.h"
#include "overlay_12_mon_return_data.h"

void ov12_0225C9BC(SysTask *task, void *data);

void ov12_0225CC58(SysTask *task, void *data);

void ov12_02259F30(BattleSystem *battleSys, OpponentData *opponentData, MonReturnMessage *message) {
    MonReturnData *data;
    int i;
    int iVar;

    data = Heap_Alloc(HEAP_ID_BATTLE, sizeof(MonReturnData));
    data->battleSys = battleSys;
    data->battlerData = opponentData;
    data->monSprite = opponentData->monSprite;
    data->command = message->command;
    data->battler = opponentData->battler;
    data->battlerType = opponentData->battlerType;
    data->state = 0;
    data->yOffset = message->yOffset;
    data->capturedBall = message->capturedBall;
    data->isSubstitute = message->isSubstitute;
    data->unk74 = message->unk2C;

    for (i = 0; i < BATTLER_MAX; i++) {
        data->moveAnim.species[i] = message->battleMonSpecies[i];
        data->moveAnim.genders[i] = message->battleMonGenders[i];
        data->moveAnim.isShiny[i] = message->battleMonIsShiny[i];
        data->moveAnim.formNums[i] = message->battleMonFormNums[i];
        data->moveAnim.personalities[i] = message->battleMonPersonalities[i];
    }

    iVar = ov12_0223C140(battleSys, opponentData->battler);
    if (iVar == 0xff || iVar != data->unk74) {
        SysTask_CreateOnMainQueue(ov12_0225C9BC, data, 0);
        return;
    }
    SysTask_CreateOnMainQueue(ov12_0225CC58, data, 0);
}
