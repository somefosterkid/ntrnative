#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "party.h"
#include "pokemon.h"
#include "sys_task_api.h"
#include "battle/battle_party_context.h"

struct BattlePartyContext;

int ov12_02258800(BattleSystem *battleSys, int battler);

int ov12_02258BA0(BattleSystem *battleSys, int battler);

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

void ov12_02261EB8(BattleSystem *battleSys);
void ov12_02261ED4(BattleSystem *battleSys);

void ov12_0225F8AC(SysTask *task, void *data) {
    PartyMenuData *partyMenuData = data;
    Party *party;
    Pokemon *mon;
    int partner;
    u32 battleType = BattleSystem_GetBattleType(partyMenuData->battleSys);
    int battler = partyMenuData->battler;

    if (battleType & (1 << 4) || battleType & (1 << 3)) {
        partner = battler;
    } else {
        partner = BattleSystem_GetBattlerIdPartner(partyMenuData->battleSys, battler);
    }

    int slot = ov12_02258BA0(partyMenuData->battleSys, battler);

    if (slot == 6) {
        slot = ov12_02258800(partyMenuData->battleSys, battler);

        if (slot == 6) {
            party = BattleSystem_GetParty(partyMenuData->battleSys, partyMenuData->battler);

            for (slot = 0; slot < Party_GetCount(party); slot++) {
                mon = BattleSystem_GetPartyMon(partyMenuData->battleSys, partyMenuData->battler, slot);

                if (GetMonData(mon, MON_DATA_HP, 0L) && partyMenuData->partySlots[battler] != slot && partyMenuData->partySlots[partner] != slot) {
                    break;
                }
            }
        }
    }

    BattleController_EmitPartyMenuResult(partyMenuData->battleSys, partyMenuData->battler, 1 + slot);
    BattleController_EmitClearCommand(partyMenuData->battleSys, partyMenuData->battler, partyMenuData->command);

    Heap_Free(data);
    SysTask_Destroy(task);
}

void ov12_0225F960(SysTask *task, void *ptr) {
    PartyMenuData *partyMenuData = ptr;

    BattleController_EmitClearCommand(partyMenuData->battleSys, (s32)partyMenuData->battler, (s32)partyMenuData->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}

void ov12_0225F980(SysTask *task, void *data) {
    PartyMenuData *partyMenuData = data;
    u8 v1;

    if (ov12_0223BE0C(partyMenuData->battleSys, partyMenuData->battler, &v1) == 1) {
        ov12_02261ED4(partyMenuData->battleSys);
    }

    if (!v1 || v1 > 6) {
        ov12_02261EB8(partyMenuData->battleSys);
    } else {
        Pokemon *mon;
        int v4 = v1 - 1;

        Party *party = BattleSystem_GetParty(partyMenuData->battleSys, partyMenuData->battler);

        if (v4 == partyMenuData->partySlots[partyMenuData->battler] || v4 == partyMenuData->doublesSelection) {
            ov12_02261EB8(partyMenuData->battleSys);
        } else if (v1 > Party_GetCount(party)) {
            ov12_02261EB8(partyMenuData->battleSys);
        } else {
            mon = BattleSystem_GetPartyMon(partyMenuData->battleSys, partyMenuData->battler, v1 - 1);

            if (GetMonData(mon, MON_DATA_HP, NULL) == 0) {
                ov12_02261EB8(partyMenuData->battleSys);
            }

            if (GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL) == SPECIES_EGG) {
                ov12_02261EB8(partyMenuData->battleSys);
            }
        }
    }

    BattleController_EmitPartyMenuResult(partyMenuData->battleSys, partyMenuData->battler, v1);
    BattleController_EmitClearCommand(partyMenuData->battleSys, partyMenuData->battler, partyMenuData->command);

    Heap_Free(data);
    SysTask_Destroy(task);
}
