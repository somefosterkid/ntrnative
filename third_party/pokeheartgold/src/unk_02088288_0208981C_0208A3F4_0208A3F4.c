#include "global.h"

#include "pokemon_summary_app.h"
#include "unk_02088288.h"

#include "pokemon.h"
#include "pokemon_types_def.h"

BOOL sub_0208A3CC(PokemonSummary *summaryScreen);

s8 sub_0208A3F4(PokemonSummary *summaryScreen, s8 delta);

s8 sub_0208A3F4(PokemonSummary *summaryScreen, s8 delta) {
    s8 monIndex = (s8)summaryScreen->args->partySlot;
    Pokemon *mon = summaryScreen->args->monData;

    while (TRUE) {
        monIndex += delta;

        if (monIndex < 0 || monIndex >= summaryScreen->args->partyCount) {
            return -1;
        }

#if defined(SDK_PORT) || defined(NONMATCHING)
        Pokemon *mons = summaryScreen->args->monData;

        mon = &mons[monIndex];
#else
        mon = (Pokemon *)((u8 *)summaryScreen->args->monData + sub_02070D90() * monIndex);
#endif

        if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE) {
            if (GetMonData(mon, MON_DATA_IS_EGG, NULL) != FALSE) {
                if (sub_0208A3CC(summaryScreen) == TRUE) {
                    break;
                }
            } else {
                break;
            }
        }
    }

    return monIndex;
}
