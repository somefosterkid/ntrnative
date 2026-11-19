#include "global.h"

#include "party.h"
#include "pokemon.h"

struct Party;

struct Pokemon;

struct SaveData;

BOOL Save_PlayerHasAllRegisInParty(SaveData *saveData);

BOOL Save_PlayerHasAllRegisInParty(SaveData *saveData) {
    int i, j, titansInParty = 0;
    static const u16 titans[] = { SPECIES_REGIROCK, SPECIES_REGICE, SPECIES_REGISTEEL };
    u16 partySpecies[6];

    Party *party = SaveArray_Party_Get(saveData);
    int partyCount = Party_GetCount(party);

    for (i = 0; i < partyCount; i++) {
        partySpecies[i] = GetMonData(Party_GetMonByIndex(party, i), MON_DATA_SPECIES, 0L);
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < partyCount; j++) {
            if (partySpecies[j] == titans[i]) {
                ++titansInParty;
                break;
            }
        }
    }

    if (titansInParty == 3) {
        return 1;
    }

    return 0;
}
