#include "global.h"

#include "sprite.h"
#include "overlay_18_pokedex_entry_popup.h"


void ov18_021F94A0(PokedexEntryPopup *arg0) {
    Sprite_Delete(arg0->objs[1].sprite);
    Sprite_Delete(arg0->objs[2].sprite);
}
