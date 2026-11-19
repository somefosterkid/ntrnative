#include "global.h"
struct PokemonSummary;

#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokemon_summary_app.h"
#include "pokemon_summary_args.h"

s32 sub_0208A3CC(void *);

s32 sub_0208A310(struct PokemonSummary *arg0, s32 arg1);

s32 sub_0208A310(PokemonSummary *arg0, s32 arg1) {
    Pokemon *mon;
    BoxPokemon *boxMon;

    switch (arg0->args->unk11) {
    case 0:
#if defined(SDK_PORT) || defined(NONMATCHING)
        Pokemon *mons = arg0->args->monData;

        mon = &mons[arg1];
#else
        mon = (Pokemon *)((u8 *)arg0->args->monData + arg1 * sub_02070D90());
#endif
        if (GetMonData(mon, 5, NULL) != 0 && GetMonData(mon, 0x4C, NULL) != 0) {
            return 1;
        }
        break;
    case 1:
        if (arg1 < arg0->args->partyCount) {
            mon = Party_GetMonByIndex(arg0->args->party, arg1);
            if (GetMonData(mon, 5, NULL) != 0) {
                if (GetMonData(mon, 0x4C, NULL) != 0) {
                    if (sub_0208A3CC(arg0) == 1) {
                        return 1;
                    }
                } else {
                    return 1;
                }
            }
        }
        break;
    case 2:
#if defined(SDK_PORT) || defined(NONMATCHING)
        BoxPokemon *boxMons = arg0->args->monData;

        boxMon = &boxMons[arg1];
#else
        boxMon = (BoxPokemon *)((u8 *)arg0->args->monData + arg1 * sub_02070D94());
#endif
        if (GetBoxMonData(boxMon, 5, NULL) != 0 && GetBoxMonData(boxMon, 0x4C, NULL) != 0) {
            return 1;
        }
        break;
    }
    return 0;
}
