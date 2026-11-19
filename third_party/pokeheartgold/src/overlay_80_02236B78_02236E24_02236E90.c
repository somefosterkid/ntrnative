#include "global.h"

#include "frontier/overlay_80_02229EE0.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "overlay_80_frontier_opponent.h"

typedef struct {
    u16 trainerIDMax;
    u16 setIDMin;
    u16 setIDMax;
    u8 ivs;
    u8 canBeUpgraded;
} BattleFactoryPokemonPool;

const BattleFactoryPokemonPool *ov80_02236C2C(int trainerID, BOOL isOpenLevel);

extern BOOL ov80_02236C9C(const u16 excludedSpecies[], const u16 excludedItems[], int numExcluded, int numMons, u16 monSetIDs[], int unused, const BattleFactoryPokemonPool *pool, u16 tradeCount, u8 ivList[]);

void ov80_02236E90(u8 numMons, u16 trainerID, u8 isOpenLevel, FrontierPokemon *excludedMons, u16 *monSetIDs, FrontierPokemon *mons, u8 *ivList, u32 *personalities, int numExcluded);

void ov80_02236E90(u8 numMons, u16 trainerID, u8 isOpenLevel, FrontierPokemon *excludedMons, u16 *monSetIDs, FrontierPokemon *mons, u8 *ivList, u32 *personalities, int numExcluded) {
    int i;
    FrontierPokemon mon;
    u16 excludedSpecies[6 * 2];
    u16 excludedItems[6 * 2];

    const BattleFactoryPokemonPool *pool = ov80_02236C2C(trainerID, isOpenLevel);

    for (i = 0; i < numExcluded; i++) {
        mon = excludedMons[i];
        excludedSpecies[i] = mon.species;
        excludedItems[i] = mon.item;
    }

    ov80_02236C9C(excludedSpecies, excludedItems, numExcluded, numMons, monSetIDs, HEAP_ID_FIELD2, pool, 0, ivList);
    ov80_0222A52C(mons, monSetIDs, ivList, 0L, personalities, numMons, HEAP_ID_FIELD2, (enum NarcId)205);
}
