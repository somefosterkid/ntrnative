#include "global.h"

#include "item.h"
#include "pokemon.h"
#include "sprite.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_0223E0BC(int species, int form, int isEgg, int iconIndex, Sprite *sprite, NARC *narc, PokemonIcon *monIcon);

void ov70_0223E170(BoxPokemon *boxMon, Sprite *iconSprite, Sprite *itemSprite, u16 *species, int iconIndex, NARC *narc, GTSPokemonCriteria *criteria, PokemonIcon *monIcon);

void ov70_0223E170(BoxPokemon *boxMon, Sprite *iconSprite, Sprite *itemSprite, u16 *species, int iconIndex, NARC *narc, GTSPokemonCriteria *criteria, PokemonIcon *monIcon) {
    int speciesExists, item, isEgg, form;

    AcquireBoxMonLock(boxMon);

    speciesExists = GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL);
    *species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);

    form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
    isEgg = GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL);
    item = GetBoxMonData(boxMon, MON_DATA_HELD_ITEM, NULL);

    criteria->species = *species;
    criteria->gender = GetBoxMonData(boxMon, MON_DATA_GENDER, NULL) + 1;

    if (isEgg) {
        criteria->level = 0;
    }

    ReleaseBoxMonLock(boxMon, 1);

    if (speciesExists) {
        ov70_0223E0BC(*species, form, isEgg, iconIndex, iconSprite, narc, monIcon);
        Sprite_SetDrawFlag(iconSprite, TRUE);

        if (item != 0) {
            Sprite_SetDrawFlag(itemSprite, TRUE);

            if (ItemIdIsMail(item)) {
                Sprite_SetAnimCtrlSeq(itemSprite, 41);
            } else {
                Sprite_SetAnimCtrlSeq(itemSprite, 40);
            }
        } else {
            Sprite_SetDrawFlag(itemSprite, FALSE);
        }
    } else {
        Sprite_SetDrawFlag(iconSprite, FALSE);
        Sprite_SetDrawFlag(itemSprite, FALSE);

        monIcon->sprite = NULL;
    }
}
