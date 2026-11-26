#include "global.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"
struct PokemonSummary;

s32 sub_0208A2C0(void *, s8);

void sub_0208B118(PokemonSummary *);

s32 sub_02089794(struct PokemonSummary *arg0);

s32 sub_02089794(PokemonSummary *arg0) {
    if (arg0->flashY == 5) {
        sub_0208A2C0(arg0, -1);
    } else {
        sub_0208A2C0(arg0, 1);
    }
    sub_0208B118(arg0);
    return 0x13;
}

s32 sub_020897F0(void *, void *, void *);

s32 sub_0208981C(void *, void *, void *);

void *sub_0208A520(PokemonSummary *summaryScreen);

void sub_020897C0(PokemonSummary *arg0);

void sub_020897C0(PokemonSummary *arg0) {
    void *v0;

    v0 = sub_0208A520(arg0);
    if (arg0->args->unk11 == 2) {
        sub_020897F0(arg0, v0, &arg0->mon);
        return;
    }
    sub_0208981C(arg0, v0, &arg0->mon);
}
