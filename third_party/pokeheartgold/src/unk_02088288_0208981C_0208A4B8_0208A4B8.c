#include "global.h"
#include "sprite_resources_header.h"

#include "constants/species.h"

#include "party.h"
#include "pokemon.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

BOOL sub_0208A3CC(PokemonSummary *summaryScreen);

s8 sub_0208A4B8(PokemonSummary *summaryScreen, s8 delta);

s8 sub_0208A4B8(PokemonSummary *summaryScreen, s8 delta) {
    s8 monIndex = (s8)summaryScreen->args->partySlot;
    BoxPokemon *mon = summaryScreen->args->monData;

    while (1) {
        monIndex += delta;

        if (monIndex < 0 || monIndex >= summaryScreen->args->partyCount) {
            return -1;
        }

#if defined(SDK_PORT) || defined(NONMATCHING)
        BoxPokemon *boxMons = summaryScreen->args->monData;

        mon = &boxMons[monIndex];
#else
        mon = (BoxPokemon *)((u8 *)summaryScreen->args->monData + sub_02070D94() * monIndex);
#endif

        if (GetBoxMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE) {
            if (GetBoxMonData(mon, MON_DATA_IS_EGG, NULL) != 0) {
                if (sub_0208A3CC(summaryScreen) == 1) {
                    break;
                }
            } else {
                break;
            }
        }
    }

    return monIndex;
}

enum SummaryDataType {
    SUMMARY_DATA_MON = 0,
    SUMMARY_DATA_PARTY_MON,
    SUMMARY_DATA_BOX_MON,
};

void *sub_0208A520(PokemonSummary *summaryScreen);

void *sub_0208A520(PokemonSummary *summaryScreen) {
    switch (summaryScreen->args->unk11) {
    case SUMMARY_DATA_MON: {
#if defined(SDK_PORT) || defined(NONMATCHING)
        Pokemon *mons = summaryScreen->args->monData;

        return &mons[summaryScreen->args->partySlot];
#else
        return (u8 *)summaryScreen->args->monData + (sub_02070D90() * summaryScreen->args->partySlot);
#endif
    }
    case SUMMARY_DATA_PARTY_MON:
        return Party_GetMonByIndex(summaryScreen->args->party, summaryScreen->args->partySlot);
    case SUMMARY_DATA_BOX_MON: {
#if defined(SDK_PORT) || defined(NONMATCHING)
        BoxPokemon *boxMons = summaryScreen->args->monData;

        return &boxMons[summaryScreen->args->partySlot];
#else
        return (u8 *)summaryScreen->args->monData + (sub_02070D94() * summaryScreen->args->partySlot);
#endif
    }
    }

    return NULL;
}
