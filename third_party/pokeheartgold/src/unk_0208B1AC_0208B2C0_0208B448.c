#include "global.h"

#include "sprite.h"
#include "unk_0208B1AC.h"
#include "pokemon_summary_app.h"

void sub_0208B448(PokemonSummary *param_1) {
    Sprite **new_var;
    s32 uVar1;
    uVar1 = 0;
    new_var = param_1->sprites;
    do {
        Sprite_SetDrawFlag(new_var[uVar1], 0);
        uVar1 = (uVar1 + 1) & 0xff;
    } while (uVar1 < (8 & 0xFFFFu));
    Sprite_SetDrawFlag(param_1->sprites[21], 0);
    Sprite_SetDrawFlag(param_1->sprites[22], 0);
    return;
}
