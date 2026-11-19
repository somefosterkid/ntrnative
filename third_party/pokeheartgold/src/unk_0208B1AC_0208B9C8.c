#include "global.h"
struct PokemonSummary;

#include "sprite.h"
#include "unk_0208B1AC.h"
#include "pokemon_summary_app.h"


s32 sub_0208BA88(PokemonSummary *, s32, s32, u8);
void sub_0208BA60(struct PokemonSummary *arg0);

void sub_0208B9C8(PokemonSummary *arg0) {
    s32 v0;
    s32 v2;
    s32 v4;

    if (arg0->page == 0) {
        if (arg0->mon.shinyLeaves[5] != 0) {
            Sprite_SetDrawFlag(arg0->sprites[114], 1);
            v0 = 0;
            do {
                Sprite_SetDrawFlag(arg0->sprites[v0 + 109], 0);
                v0 += 1;
            } while (v0 < 5);
            return;
        }
        v4 = 0;
        do {
            if (arg0->mon.shinyLeaves[v4] != 0) {
                Sprite_SetDrawFlag(arg0->sprites[v4 + 109], 1);
            } else {
                Sprite_SetDrawFlag(arg0->sprites[v4 + 109], 0);
            }
            v4 += 1;
        } while (v4 < 5);
        Sprite_SetDrawFlag(arg0->sprites[114], 0);
        return;
    }
    v2 = 0;
    do {
        Sprite_SetDrawFlag(arg0->sprites[v2 + 109], 0);
        v2 += 1;
    } while (v2 < 6);
}

void sub_0208BA60(PokemonSummary *arg0) {
    sub_0208BA88(arg0, 0xB, 3, arg0->mon.type1);
    sub_0208BA88(arg0, 0xC, 4, arg0->mon.type2);
}
