#include "global.h"

#include "math_util.h"
#include "overlay_80_02236B78.h"
#include "filesystem_files_def.h"

typedef struct FrontierPokemonBase {
    u16 species;
    u16 moves[4];
    u8 evFlags;
    u8 nature;
    u16 item;
    u16 form;
} FrontierPokemonBase;

void ov80_02229EF4(FrontierPokemonBase *monData, int narcIdx, enum NarcId narcID);

typedef struct {
    u16 trainerIDMax;
    u16 setIDMin;
    u16 setIDMax;
    u8 ivs;
    u8 canBeUpgraded;
} BattleFactoryPokemonPool;

extern const int ov80_0223C464[5];

extern BOOL ov80_02236C9C(const u16 excludedSpecies[], const u16 excludedItems[], int numExcluded, int numMons, u16 monSetIDs[], int unused, const BattleFactoryPokemonPool *pool, u16 tradeCount, u8 ivList[]);

BOOL ov80_02236C9C(const u16 excludedSpecies[], const u16 excludedItems[], int numExcluded, int numMons, u16 monSetIDs[], int unused, const BattleFactoryPokemonPool *pool, u16 tradeCount, u8 ivList[]) {
    u8 ivs;
    int slot, setIDRange, setID, i;
    FrontierPokemonBase newMon[6];

    ((numMons <= 6) ? (void)0 : GF_AssertFail());

    setIDRange = pool->setIDMax - pool->setIDMin;
    slot = 0;

    int numUpgradable = 0;
    if (tradeCount != 0) {
        numUpgradable = (sizeof(ov80_0223C464) / sizeof(*(ov80_0223C464)));

        for (i = 0; i < (sizeof(ov80_0223C464) / sizeof(*(ov80_0223C464))); i++) {
            if (tradeCount <= ov80_0223C464[i]) {
                numUpgradable = i;
                break;
            }
        }
    }

    while (slot != numMons) {
        if (slot >= numMons - numUpgradable && pool->canBeUpgraded == 1) {
            setIDRange = pool[1].setIDMax - pool[1].setIDMin;
            setID = pool[1].setIDMax - (LCRandom() % (setIDRange + 1));
            ivs = pool[1].ivs;
        } else {
            setID = pool->setIDMax - (LCRandom() % (setIDRange + 1));
            ivs = pool->ivs;
        }

        ov80_02229EF4(&newMon[slot], setID, (enum NarcId)205);

        for (i = 0; i < slot; i++) {
            if (newMon[i].species == newMon[slot].species || newMon[i].item == newMon[slot].item) {
                break;
            }
        }

        if (i != slot) {
            continue;
        }

        for (i = 0; i < numExcluded; i++) {
            if (newMon[slot].species == excludedSpecies[i] || newMon[slot].item == excludedItems[i]) {
                break;
            }
        }

        if (i != numExcluded) {
            continue;
        }

        monSetIDs[slot] = setID;
        ivList[slot] = ivs;

        slot++;
    }

    return 0;
}

u8 ov80_02236DD4(u8 challengeType) {
    switch (challengeType) {
    case 0:
    case 1:
        return 3;
    case 2:
    case 3:
        return 2;
    }

    return 0;
}

u8 ov80_02236DF8(u8 challengeType, BOOL includePartnersMons) {
    switch (challengeType) {
    case 0:
    case 1:
        return 3;
    case 2:
    case 3:
        if (includePartnersMons == 0) {
            return 2;
        } else {
            return 2 * 2;
        }
    }

    return 0;
}
