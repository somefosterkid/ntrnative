#include "global.h"
struct PokemonSummary;

#include "sprite.h"
#include "pokemon_summary_app.h"

s32 sub_0208AB58(void *, s32);

s32 sub_0208942C(struct PokemonSummary *arg0);

s32 sub_0208942C(PokemonSummary *arg0) {
    if (Sprite_IsAnimated(arg0->sprites[64]) == 0) {
        sub_0208AB58(arg0, -9);
        return 0xC;
    }
    return 0xD;
}

s32 sub_02089454(struct PokemonSummary *arg0);

s32 sub_02089454(PokemonSummary *arg0) {
    if (Sprite_IsAnimated(arg0->sprites[65]) == 0) {
        sub_0208AB58(arg0, 9);
        return 0xC;
    }
    return 0xE;
}
