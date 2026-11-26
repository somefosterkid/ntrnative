#include "global.h"

#include "overlay_80_02229EE0.h"
#include "party.h"
#include "sprite.h"
#include "unk_0208805C.h"

void ov80_0222A334(SaveData *, Pokemon *);

enum PokemonIconAnim {
    POKEICON_ANIM_FAINTED = 0,
    POKEICON_ANIM_HP_MAX,
    POKEICON_ANIM_HP_GREEN,
    POKEICON_ANIM_HP_YELLOW,
    POKEICON_ANIM_HP_RED,
    POKEICON_ANIM_STATUSED,
};
enum BarColor {
    BARCOLOR_EMPTY = 0,
    BARCOLOR_RED,
    BARCOLOR_YELLOW,
    BARCOLOR_GREEN,

    BARCOLOR_MAX,
};

void ov80_0222A3BC(SaveData *arg0, Party *party, Pokemon *mon) {
    ov80_0222A334(arg0, mon);
    Party_AddMon(party, mon);
}

void ov80_0222A3D4(Sprite *sprite, u8 animID) {
    if (Sprite_GetAnimationNumber(sprite) == animID) {
        return;
    }

    Sprite_SetAnimationFrame(sprite, 0);
    Sprite_SetAnimCtrlSeq(sprite, animID);
    Sprite_UpdateAnim(sprite, ((fx32)0x0000000000001000L));
}

void ov80_0222A400(Sprite *sprite, s16 x, s16 y, u8 isSelected) {
    VecFx32 position;

    position.x = x * ((fx32)0x0000000000001000L);
    position.y = y * ((fx32)0x0000000000001000L);
    position.z = 0;

    if (isSelected == 1) {
        if (Sprite_GetAnimationFrame(sprite) == 0) {
            position.y = (y - 3) * ((fx32)0x0000000000001000L);
        } else {
            position.y = (y + 1) * ((fx32)0x0000000000001000L);
        }
    }

    Sprite_SetMatrix(sprite, &position);
}

u8 ov80_0222A43C(u16 hp, u16 maxHp) {
    switch (CalculateHpBarColor(hp, maxHp, 48)) {
    case BARCOLOR_MAX:
        return POKEICON_ANIM_HP_MAX;
    case BARCOLOR_GREEN:
        return POKEICON_ANIM_HP_GREEN;
    case BARCOLOR_YELLOW:
        return POKEICON_ANIM_HP_YELLOW;
    case BARCOLOR_RED:
        return POKEICON_ANIM_HP_RED;
    }

    return POKEICON_ANIM_HP_MAX;
}
