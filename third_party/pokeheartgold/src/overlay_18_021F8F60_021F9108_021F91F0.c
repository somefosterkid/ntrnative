#include "global.h"

#include "sprite.h"
#include "overlay_18_pokedex_entry_popup.h"










void ov18_021F91F0(PokedexEntryPopup *arg0) {
    ov18_021F959C(arg0);
    ov18_021F94A0(arg0);
    ov18_021F9508(arg0);
    ov18_021F91DC(arg0);
    ov18_021F9108(arg0);
    ov18_021F9054(arg0);
    ov18_021F8F84(arg0);
    ov18_021F8F58();
    SpriteList_Delete(arg0->spriteList);
}
