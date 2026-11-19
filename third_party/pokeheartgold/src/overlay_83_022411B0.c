#include "global.h"

#include "bg_window.h"
#include "overlay_83_battle_castle_self_app.h"

struct BattleCastleSelfApp;
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

u32 ov83_022411B0(BattleCastleSelfApp *app, u8 barColor);
u32 ov83_022411DC(BattleCastleSelfApp *app, u8 barColor);

u32 ov83_022411B0(BattleCastleSelfApp *app, u8 barColor) {
    u32 animID;

    switch (barColor) {
    case BARCOLOR_MAX:
        animID = POKEICON_ANIM_HP_MAX;
        break;
    case BARCOLOR_GREEN:
        animID = POKEICON_ANIM_HP_GREEN;
        break;
    case BARCOLOR_YELLOW:
        animID = POKEICON_ANIM_HP_YELLOW;
        break;
    case BARCOLOR_RED:
        animID = POKEICON_ANIM_HP_RED;
        break;
    }

    return animID;
}

u32 ov83_022411DC(BattleCastleSelfApp *app, u8 barColor) {
    u32 animID;

    switch (barColor) {
    case BARCOLOR_MAX:
        animID = 15;
        break;
    case BARCOLOR_GREEN:
        animID = 15;
        break;
    case BARCOLOR_YELLOW:
        animID = 14;
        break;
    case BARCOLOR_RED:
        animID = 13;
        break;
    }

    return animID;
}
