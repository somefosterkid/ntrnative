#include "global.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sys_task.h"
#include "transform_data.h"
#include "pokemon_anim.h"

void sub_020174B4(PokemonAnim *monAnim);

void sub_0201752C(PokemonAnim *monAnim);
void sub_02017550(PokemonAnim *monAnim);
void sub_02017578(PokemonAnim *monAnim);
void sub_0201759C(PokemonAnim *monAnim);
void sub_020175C4(PokemonAnim *monAnim);

void sub_020174B4(PokemonAnim *monAnim) {
    monAnim->waitFrame = 1;
}

void sub_020174BC(PokemonAnim *monAnim) {
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_X, monAnim->originalX);
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_Y, monAnim->originalY);
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_ZROT, 0);
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_XPIVOT, 0);
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_AFFINEW, 0x100);
    Pokepic_SetAttr(monAnim->sprite, POKEPIC_AFFINEH, 0x100);
}

void sub_02017504(PokemonAnim *monAnim);

void sub_02017504(PokemonAnim *monAnim) {
    u8 destIndex, originIndex;

    sub_020172B4(monAnim, &destIndex, &originIndex);
    monAnim->vars[destIndex] = monAnim->vars[originIndex];
}

void sub_0201752C(PokemonAnim *monAnim) {
    int v2;
    int v1;
    u8 v0;

    sub_020172C8(monAnim, &v0, &v2, &v1);
    monAnim->vars[v0] = (v2 + v1);
}

void sub_02017550(PokemonAnim *monAnim) {
    int v2;
    int v1;
    u8 v0;

    sub_020172C8(monAnim, &v0, &v2, &v1);
    monAnim->vars[v0] = (v2 * v1);
}

void sub_02017578(PokemonAnim *monAnim) {
    int v2;
    int v1;
    u8 v0;

    sub_0201733C(monAnim, &v0, &v2, &v1);
    monAnim->vars[v0] = (v2 - v1);
}

void sub_0201759C(PokemonAnim *monAnim) {
    int v2;
    int v1;
    u8 v0;

    sub_0201733C(monAnim, &v0, &v2, &v1);
    monAnim->vars[v0] = (v2 / v1);
}

void sub_020175C4(PokemonAnim *monAnim) {
    int v2;
    int v1;
    u8 v0;

    sub_0201733C(monAnim, &v0, &v2, &v1);
    monAnim->vars[v0] = (v2 % v1);
}
