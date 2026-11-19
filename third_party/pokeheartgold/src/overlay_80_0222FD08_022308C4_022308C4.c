#include "global.h"

#include "frontier/overlay_80_02229EE0.h"
#include "overlay_80_battle_factory.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_12_field_battle_dto.h"

struct BattleFactorySave;

struct Party;

struct SysTask;

struct SysTaskQueue;

int Party_GetCount(const Party *party);

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

u32 GetMonData(Pokemon *mon, int param, void *dest);

typedef struct BattleFactorySave BattleFactorySave;

typedef struct {
    u16 trainerIDMax;
    u16 setIDMin;
    u16 setIDMax;
    u8 ivs;
    u8 canBeUpgraded;
} BattleFactoryPokemonPool;

const BattleFactoryPokemonPool *ov80_02236C2C(int trainerID, BOOL isOpenLevel);

extern BOOL ov80_02236C9C(const u16 excludedSpecies[], const u16 excludedItems[], int numExcluded, int numMons, u16 monSetIDs[], int unused, const BattleFactoryPokemonPool *pool, u16 tradeCount, u8 ivList[]);

u8 ov80_02236DF8(u8 challengeType, BOOL param1);

void ov80_022308C4(BattleFactory *factory);

void ov80_022308C4(BattleFactory *factory) {
    int i, partySize, playersPartySize;
    u16 usedSpecies[(((3) >= (2 * 2)) ? (3) : (2 * 2)) * 2];
    u16 usedItems[(((3) >= (2 * 2)) ? (3) : (2 * 2)) * 2];

    for (i = 0; i < (((3) >= (2 * 2)) ? (3) : (2 * 2)) * 2; i++) {
        usedSpecies[i] = 0;
        usedItems[i] = 0;
    }

    u8 opponentPartySize = ov80_02236DF8(factory->challengeType, 1);
    partySize = Party_GetCount(factory->playersParty);

    for (i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(factory->playersParty, i);
        usedSpecies[i] = GetMonData(mon, MON_DATA_SPECIES, 0L);
        usedItems[i] = GetMonData(mon, MON_DATA_HELD_ITEM, 0L);
    }

    playersPartySize = partySize;
    partySize = Party_GetCount(factory->opponentsParty);

    for (i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(factory->opponentsParty, i);
        usedSpecies[i + playersPartySize] = GetMonData(mon, MON_DATA_SPECIES, 0L);
        usedItems[i + playersPartySize] = GetMonData(mon, MON_DATA_HELD_ITEM, 0L);

        factory->initialRentalSetIDs[i] = factory->opponentMonSetIDs[i];
    }

    const BattleFactoryPokemonPool *pool = ov80_02236C2C(factory->trainerIDs[factory->currentBattle], factory->isOpenLevel);

    ov80_02236C9C(usedSpecies, usedItems, playersPartySize + partySize, opponentPartySize, factory->opponentMonSetIDs, HEAP_ID_FIELD2, pool, 0, factory->opponentMonIVs);
    ov80_0222A52C(factory->opponentMons, factory->opponentMonSetIDs, factory->opponentMonIVs, 0L, factory->opponentMonPersonalities, opponentPartySize, HEAP_ID_FIELD2, (enum NarcId)205);
}
