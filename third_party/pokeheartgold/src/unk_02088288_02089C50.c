#include "global.h"
struct PokemonSummary;

#include "pokemon.h"
#include "sound_chatot.h"
#include "unk_02005D10.h"
#include "unk_02088288.h"
#include "pokemon_summary_app.h"

void sub_02089C50(PokemonSummary *arg0);

void sub_02089C50(PokemonSummary *arg0) {
    u8 v0;
    u16 species;

    if (arg0->mon.isEgg == 0) {
        sub_020729A4(arg0->pokepicAnimNarc, &v0, arg0->mon.species, 1U);
        species = arg0->mon.species;
        if (species == 0x1B9) {
            sub_02006EA0(arg0->args->unk28, 0U, 0x64U, 0, (u8)(s32)v0);
            return;
        }
        sub_020062E0(species, v0, (u8) arg0->mon.form);
    }
}

s32 sub_02089F98(void *);

s32 sub_0208A8F4(PokemonSummary *);

s32 sub_0208B400(void *);

s32 sub_0208B448(void *);

s32 sub_0208B5A8(PokemonSummary *);

s32 sub_0208B9C8(PokemonSummary *);

s32 sub_0208BCD4(PokemonSummary *);

s32 sub_0208BD38(void *);

s32 sub_0208BF9C(PokemonSummary *);

s32 sub_0208BFD0(void *);

s32 sub_0208C2A0(PokemonSummary *);

s32 sub_0208C42C(void *);

s32 sub_0208CBD4(void *);

void sub_02089CB4(struct PokemonSummary *arg0);

void sub_02089CB4(PokemonSummary *arg0) {
    u8 v0;

    v0 = arg0->args->unk12;
    switch (v0) {
    case 0:
    case 1:
        arg0->page = 0;
        break;
    case 2:
        arg0->page = 1;
        break;
    }
    sub_0208B448(arg0);
    sub_0208B5A8(arg0);
    sub_0208C2A0(arg0);
    sub_0208BD38(arg0);
    sub_0208BCD4(arg0);
    sub_0208C42C(arg0);
    sub_0208CBD4(arg0);
    sub_02089F98(arg0);
    sub_0208B9C8(arg0);
    if (arg0->args->unk12 == 2) {
        sub_0208A8F4(arg0);
        return;
    }
    sub_0208B400(arg0);
    sub_0208BF9C(arg0);
    sub_0208BFD0(arg0);
}

void sub_02089D40(PokemonSummaryArgs *arg0, const u8 *arg1) {
    u8 v0;
    u8 v1;

    v1 = 0;
    arg0->unk15 = 0;
loop_1:
    v0 = arg1[v1];
    if (v0 != 4) {
        arg0->unk15 |= 1 << v0;
        v1 += 1;
        if ((u32)v1 < 4U) {
            goto loop_1;
        }
    }
}
