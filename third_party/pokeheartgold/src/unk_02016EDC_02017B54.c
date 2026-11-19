#include "global.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sys_task.h"
#include "transform_data.h"
#include "pokemon_anim.h"

#include "constants/pokemon_anim.h"

void sub_02017B54(PokemonAnim *monAnim);

void sub_02017B8C(PokemonAnim *monAnim);
void sub_02017B98(PokemonAnim *monAnim);
void sub_02017BA4(PokemonAnim *monAnim);
void sub_02017BB0(PokemonAnim *monAnim);
void sub_02017BBC(PokemonAnim *monAnim);

void sub_02017B54(PokemonAnim *monAnim) {
    sub_02017280(monAnim, &monAnim->yNormalization);
    ((monAnim->yNormalization == Y_NORMALIZATION_NEGATIVE_SCALE || monAnim->yNormalization == Y_NORMALIZATION_ON || (monAnim->yNormalization == Y_NORMALIZATION_OFF && 1)) ? (void)0 : GF_AssertFail())

        ;
}

void sub_02017B8C(PokemonAnim *monAnim) {
    sub_02017C78(monAnim, 0);
}

void sub_02017B98(PokemonAnim *monAnim) {
    sub_02017C78(monAnim, 1);
}

void sub_02017BA4(PokemonAnim *monAnim) {
    sub_02017C78(monAnim, 2);
}

void sub_02017BB0(PokemonAnim *monAnim) {
    sub_02017C78(monAnim, 3);
}

void sub_02017BBC(PokemonAnim *monAnim) {
    sub_02017C78(monAnim, 4);
}
