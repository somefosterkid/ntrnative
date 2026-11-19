#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "sys_task_api.h"
#include "battle/battle_party_context.h"
#include "battle/battle_bag_context.h"

typedef u16 charcode_t;

void ov12_02261ED4(BattleSystem *battleSys);

typedef struct PartyMenuData {
    BattleSystem *battleSys;
    BattlePartyContext *battlePartyCtx;
    u8 command;
    u8 battler;
    u8 state;
    u8 listMode;
    u8 partySlots[4];
    int canSwitch;
    u16 selectedBattleBagItem;
    u8 doublesSelection;
    u8 isCursorEnabled;
    u8 battlersSwitchingMask;
    u8 unused[3];
    u8 partyOrder[4][6];
} PartyMenuData;

typedef struct BagMenuData {
    BattleSystem *battleSys;
    BattleBagContext *battleBagCtx;
    PartyMenuData *partyMenuData;
    u8 command;
    u8 battler;
    u8 state;
    u8 battlerType;
    u8 isCursorEnabled;
    u8 msgIdx;
    u16 stateAfterDelay;
    u8 hasTwoOpponents;
    u8 semiInvulnerable;
    u8 substitute;
    u8 delay;
    u8 partyOrder[4][6];
    u8 embargoTurns[4];
} BagMenuData;

void ov12_0225F434(SysTask *task, void *data) {
    BagMenuData *bagMenuData = data;
    BattleItemUse used;
    u8 v2;

    if (ov12_0223BE0C(bagMenuData->battleSys, bagMenuData->battler, &v2) == 1) {
        ov12_02261ED4(bagMenuData->battleSys);
    }

    used.item = v2;

    if (ov12_0223BE0C(bagMenuData->battleSys, bagMenuData->battler, &v2) == 1) {
        ov12_02261ED4(bagMenuData->battleSys);
    }

    used.item |= (v2 << 8);

    if (ov12_0223BE0C(bagMenuData->battleSys, bagMenuData->battler, &v2) == 1) {
        ov12_02261ED4(bagMenuData->battleSys);
    }

    used.category = v2 & 0xF;
    used.target = (v2 & 0xF0) >> 8;

    BattleController_EmitSelectedBagItem(bagMenuData->battleSys, bagMenuData->battler, used);
    BattleController_EmitClearCommand(bagMenuData->battleSys, bagMenuData->battler, bagMenuData->command);

    Heap_Free(bagMenuData->partyMenuData->battlePartyCtx->party);
    Heap_Free(bagMenuData->partyMenuData->battlePartyCtx);
    Heap_Free(bagMenuData->partyMenuData);
    Heap_Free(data);
    SysTask_Destroy(task);
}
