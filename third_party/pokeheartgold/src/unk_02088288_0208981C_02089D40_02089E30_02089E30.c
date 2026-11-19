#include "global.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

s32 sub_02089E14(void *, s32 arg1);

void sub_02089E30(PokemonSummary *arg0, s32 arg1);

void sub_02089E30(PokemonSummary *arg0, s32 arg1) {
    s8 temp_ip;
    s8 var_r1;

    var_r1 = arg0->page;
    temp_ip = var_r1;
    if (arg0->mon.isEgg == 0) {
        do {
            var_r1 += arg1;
            if ((s32)var_r1 < 0) {
                var_r1 = 2;
            } else if ((s32)var_r1 > 2) {
                var_r1 = 0;
            }
        } while (!((1 << var_r1) & arg0->args->unk15));
        if (var_r1 != temp_ip) {
            sub_02089E14(arg0, var_r1);
        }
    }
}
