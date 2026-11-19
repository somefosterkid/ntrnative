#include "global.h"

#include "pokemon_summary_app.h"
#include "unk_02088288.h"

s32 sub_0208A71C(PokemonSummary *summaryScreen, s32 arg1);

s32 sub_0208A71C(PokemonSummary *summaryScreen, s32 arg1) {
    s8 var_r2;
    s8 var_r3;
    s16 temp_r4;
    PokemonSummaryArgs *args;

    temp_r4 = summaryScreen->moveSlot;
    var_r2 = (s8)temp_r4;
    args = summaryScreen->args;
    if (args->unk12 == 2) {
        if (args->moveToLearn != 0) {
            var_r3 = 4;
        } else {
            var_r3 = 3;
        }
    } else {
        var_r3 = 3;
    }
loop_6:
    var_r2 += arg1;
    if ((s32)var_r2 < 0) {
        var_r2 = var_r3;
    } else if ((s32)var_r2 > (s32)var_r3) {
        var_r2 = 0;
    }
    if ((var_r2 != 4) && (summaryScreen->mon.moves[var_r2] == 0) && (var_r2 != temp_r4)) {
        goto loop_6;
    }
    if (var_r2 != temp_r4) {
        summaryScreen->moveSlot = var_r2;
        return 1;
    }
    return 0;
}
