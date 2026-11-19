#include "global.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"
struct PokemonSummary;

s32 sub_0208A3CC(struct PokemonSummary *arg0);

s32 sub_0208A3CC(PokemonSummary *arg0) {
    s8 v0;

    if (arg0->args->unk12 == 3) {
        return 0;
    }
    v0 = arg0->page;
    if ((v0 != 0) && (v0 != 3)) {
        return 0;
    }
    return 1;
}
