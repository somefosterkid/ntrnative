#include "global.h"

#include "error_handling.h"
#include "pokepic.h"
#include "pokemon_anim.h"

void sub_020176CC(PokemonAnim *monAnim);

void sub_020176CC(PokemonAnim *monAnim) {
    u8 destIndex;

    sub_02017294(monAnim, &destIndex);
    monAnim->scriptPtr++;
    monAnim->vars[destIndex] = sub_02017214(monAnim->scriptPtr);
}

void sub_020176F0(PokemonAnim *monAnim);

void sub_020176F0(PokemonAnim *monAnim) {
    if (monAnim->loopStart != NULL) {
        GF_AssertFail();
    }
    monAnim->scriptPtr++;
    monAnim->loopStart = monAnim->scriptPtr;
    monAnim->loopMax = sub_02017214(monAnim->scriptPtr);
    monAnim->loopCounter = 0;
}

void sub_02017714(PokemonAnim *monAnim);

void sub_02017714(PokemonAnim *monAnim) {
    monAnim->loopCounter++;
    if (monAnim->loopCounter >= monAnim->loopMax) {
        monAnim->loopStart = NULL;
        monAnim->loopCounter = 0;
        monAnim->loopMax = 0;
        return;
    }
    monAnim->scriptPtr = monAnim->loopStart;
}

void sub_02017730(PokemonAnim *monAnim);

void sub_02017730(PokemonAnim *monAnim) {
    int attr;
    u8 varIndex;

    sub_0201726C(monAnim, &attr);
    sub_02017294(monAnim, &varIndex);
    Pokepic_SetAttr(monAnim->sprite, attr, monAnim->vars[varIndex]);
}

void sub_0201775C(PokemonAnim *monAnim);

void sub_0201775C(PokemonAnim *monAnim) {
    int attr;
    u8 varIndex;

    sub_0201726C(monAnim, &attr);
    sub_02017294(monAnim, &varIndex);
    Pokepic_AddAttr(monAnim->sprite, attr, monAnim->vars[varIndex]);
}
