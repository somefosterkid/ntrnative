#include "global.h"
struct PokemonSummary;

#include "unk_0201956C.h"
#include "screen_fade.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

s32 sub_02089C50(PokemonSummary *);
s32 sub_0208E444(PokemonSummary *);
s32 sub_02088B08(struct PokemonSummary *arg0);

void sub_02088AF8(PokemonSummary *arg0) {
    sub_020195C0(arg0->cellManager);
}

s32 sub_02088B08(PokemonSummary *arg0) {
    u8 v0;

    if (IsPaletteFadeFinished() == 1) {
        sub_0208E444(arg0);
        sub_02089C50(arg0);
        v0 = arg0->args->unk12;
        if (v0 == 2) {
            return 8;
        }
        if (v0 == 4) {
            return 0x10;
        }
        return 2;
    }
    return 1;
}
