#include "global.h"

#include "sprite.h"
#include "unk_0208805C.h"
#include "pokemon_types_def.h"
#include "overlay_08_battle_party.h"

void ManagedSprite_SetAnim(Sprite *sprite, u32 animID);

u16 ManagedSprite_GetActiveAnim(const Sprite *sprite);

void ManagedSprite_SetAnimationFrame(Sprite *sprite, u16 frame);

void ov08_022213E4(Sprite *pSprite, u8 anim);

void ov08_022213E4(Sprite *pSprite, u8 anim) {
    if (ManagedSprite_GetActiveAnim(pSprite) == anim) {
        return;
    }

    ManagedSprite_SetAnimationFrame(pSprite, 0);
    ManagedSprite_SetAnim(pSprite, anim);
}

struct Pokemon;

enum SummaryStatus {
    SUMMARY_CONDITION_POKERUS = 0,
    SUMMARY_CONDITION_PARALYSIS,
    SUMMARY_CONDITION_FREEZE,
    SUMMARY_CONDITION_SLEEP,
    SUMMARY_CONDITION_POISON,
    SUMMARY_CONDITION_BURN,
    SUMMARY_CONDITION_FAINTED,
    SUMMARY_CONDITION_NONE,
};

enum BarColor {
    BARCOLOR_EMPTY = 0,
    BARCOLOR_RED,
    BARCOLOR_YELLOW,
    BARCOLOR_GREEN,

    BARCOLOR_MAX,
};

u8 ov08_02221404(BattlePartyPokemon *battleParty);

u8 ov08_02221404(BattlePartyPokemon *battleParty) {
    if (battleParty->curHP == 0) {
        return 0;
    }

    if ((battleParty->summaryStatus != SUMMARY_CONDITION_NONE) && (battleParty->summaryStatus != SUMMARY_CONDITION_FAINTED)) {
        return 5;
    }

    switch (CalculateHpBarColor(battleParty->curHP, battleParty->maxHP, 48)) {
    case BARCOLOR_MAX:
        return 1;
    case BARCOLOR_GREEN:
        return 2;
    case BARCOLOR_YELLOW:
        return 3;
    case BARCOLOR_RED:
        return 4;
    }

    return 0;
}
