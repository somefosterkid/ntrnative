#include "global.h"

#include "pokepic.h"
#include "pokemon_anim.h"

void sub_02017B2C(PokemonAnim *monAnim);

void sub_02017B2C(PokemonAnim *monAnim) {
    if (Pokepic_ResumePaletteFade(monAnim->sprite) != 0) {
        monAnim->fadeActive = 1;
        monAnim->waitFrame = 1;
    }
}

void sub_02017B48(PokemonAnim *monAnim);

void sub_02017B48(PokemonAnim *monAnim) {
    monAnim->waitForTransform = 1;
}
