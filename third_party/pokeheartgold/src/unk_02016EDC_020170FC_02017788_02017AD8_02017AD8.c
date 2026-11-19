#include "global.h"

#include "pokemon_anim.h"

void sub_02017AD8(PokemonAnim *monAnim);

void sub_02017AD8(PokemonAnim *monAnim) {
    sub_0201726C(monAnim, &monAnim->startDelay);
    monAnim->waitFrame = 1;
}
