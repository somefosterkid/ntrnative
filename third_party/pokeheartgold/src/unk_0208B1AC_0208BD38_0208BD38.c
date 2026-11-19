#include "global.h"

#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "pokemon_summary_app.h"

void sub_0208BD38(PokemonSummary *arg0);

void sub_0208BD38(PokemonSummary *arg0) {
    u32 var_r4;
    var_r4 = 0;
    do {
        if ((arg0->mon.markings) & (1 << var_r4)) {
            Sprite_SetAnimCtrlSeq(arg0->sprites[var_r4 + 23], 1);
        } else {
            Sprite_SetAnimCtrlSeq(arg0->sprites[var_r4 + 23], 0);
        }
        var_r4 += 1;
    } while (var_r4 < 6U);
    if (arg0->mon.shiny == 1 && arg0->mon.isEgg == 0) {
        thunk_Sprite_SetDrawFlag(arg0->sprites[52], 1);
    } else {
        thunk_Sprite_SetDrawFlag(arg0->sprites[52], 0);
    }
    if (arg0->mon.pokerus == 2) {
        thunk_Sprite_SetDrawFlag(arg0->sprites[53], 1);
        return;
    }
    thunk_Sprite_SetDrawFlag(arg0->sprites[53], 0);
}
