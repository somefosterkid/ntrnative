#include "global.h"

#include "pokemon_summary_app.h"

#include "sprite.h"



void sub_0208BDC8(PokemonSummary *summaryScreen);

void sub_0208BDC8(PokemonSummary *summaryScreen) {
    for (u8 i = 0; i < 5; i++) {
        if (summaryScreen->unk7BE & (1 << i)) {
            Sprite_SetDrawFlag(summaryScreen->sprites[41 + i], 1);
        }
    }
}
