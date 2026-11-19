#include "global.h"

#include "pokemon_summary_app.h"
#include "system.h"
#include "unk_02088288.h"

#include "unk_02005D10.h"

s32 sub_0208AE08(PokemonSummary *summary);

s32 sub_0208A71C(PokemonSummary *summary, s32 arg1);

void sub_0208A79C(PokemonSummary *summary);

void sub_0208AFA0(PokemonSummary *summary, s32 arg1);

s32 sub_0208B044(PokemonSummary *summary, s32 arg1);

s32 sub_0208B08C(PokemonSummary *summary, s32 arg1);

s32 sub_02088D48(PokemonSummary *summary);

s32 sub_02088D48(PokemonSummary *summary) {
    s32 iVar1;
    s32 sVar3;
    if ((gSystem.newKeys & 0x40) != 0) {
        iVar1 = sub_0208A71C(summary, -1);
        if (iVar1 == 1) {
            PlaySE(0x5dc);
            sub_0208A79C(summary);
        }
        return 5;
    }
    if ((gSystem.newKeys & 0x80) != 0) {
        iVar1 = sub_0208A71C(summary, 1);
        if (iVar1 == 1) {
            PlaySE(0x5dc);
            sub_0208A79C(summary);
        }
        return 5;
    }
    if (((gSystem.newKeys & 1) != 0) && (summary->args->unk12 != 1)) {
        PlaySE(0x5dd);
        return sub_0208B08C(summary, 7);
    }
    if ((gSystem.newKeys & 2) != 0) {
        PlaySE(0x940);
        sub_0208AFA0(summary, 0);
        return sub_0208B044(summary, 4);
    }
    sVar3 = sub_0208AE08(summary);
    if (sVar3 == 4) {
        if (summary->args->unk12 != 1) {
            PlaySE(0x5dd);
            return sub_0208B08C(summary, 7);
        }
    } else {
        if (sVar3 == 5) {
            PlaySE(0x940);
            sub_0208AFA0(summary, 0);
            return sub_0208B044(summary, 4);
        }
        if ((sVar3 != (-1)) && (summary->mon.moves[sVar3] != 0)) {
            summary->moveSlot = sVar3;
            PlaySE(0x5dc);
            sub_0208A79C(summary);
            return 5;
        }
    }
    return 5;
}
