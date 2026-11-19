#include "global.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "pokepic.h"

void sub_02017AEC(PokemonAnim *monAnim);

void sub_02017AEC(PokemonAnim *monAnim) {
    u8 initAlpha, targetAlpha, delay;
    int color;

    sub_02017280(monAnim, &initAlpha);
    sub_02017280(monAnim, &targetAlpha);
    sub_02017280(monAnim, &delay);
    sub_0201726C(monAnim, &color);
    Pokepic_StartPaletteFade(monAnim->sprite, initAlpha, targetAlpha, delay, color);
}
