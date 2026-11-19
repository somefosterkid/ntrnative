#include "global.h"

#include "pokemon_summary_app.h"

s32 sub_0208A564(PokemonSummary *arg0);

void sub_0208AFA0(PokemonSummary *, s32);

s32 sub_02088D18(PokemonSummary *arg0);

s32 sub_02088D18(PokemonSummary *arg0) {
    if (sub_0208A564(arg0) == 1) {
        sub_0208AFA0(arg0, 1);
        return 5;
    }
    return 3;
}

s32 sub_0208A63C();

s32 sub_02088D34(void);

s32 sub_02088D34(void) {
    if (sub_0208A63C() == 1) {
        return 2;
    }
    return 4;
}
