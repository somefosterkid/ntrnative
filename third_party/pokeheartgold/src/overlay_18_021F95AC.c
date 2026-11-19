#include "global.h"
struct PokedexEntryPopup;

#include "overlay_18.h"
#include "overlay_manager.h"
#include "sprite.h"
#include "pokemon.h"
#include "pokepic.h"
#include "overlay_18_pokedex_entry_popup.h"



void ov18_021F95AC(PokedexEntryPopup *popup) {
    u32 uVar1;

    uVar1 = 0;
    do {
        Sprite_SetDrawFlag(popup->objs[uVar1].sprite, 0);
        uVar1 = uVar1 + 1;
    } while (uVar1 < 4);
    return;
}

void ov18_021F95CC(PokedexEntryPopup *arg0) {
    PokepicTemplate v0;

    GetPokemonSpriteCharAndPlttNarcIds(&v0, arg0->mon, 2U);
    arg0->pokepic = PokepicManager_CreatePokepic(arg0->pokepicManager, &v0, 0x30, 0x48, 0, 0, NULL, NULL);
}
