#include "global.h"

#include "constants/heap.h"

#include "sprite.h"
#include "unk_02009D48.h"
#include "overlay_18_pokedex_entry_popup.h"

void ov18_021F9310(void *, void *, SpriteResourcesHeader *, u32);



void ov18_021F94BC(PokedexEntryPopup *param_1) {
    Sprite *pSVar1;
    SimpleSpriteTemplate SStack_2c;
    SpriteResourcesHeader SStack_50;
    ov18_021F9310(param_1->objs[0].res, param_1->resMan, &SStack_50, 1);
    SStack_2c.spriteList = param_1->spriteList;
    SStack_2c.header = &SStack_50;
    SStack_2c.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    SStack_2c.priority = 0;
    SStack_2c.heapID = param_1->heapId;
    SStack_2c.position.x = 0x70000;
    SStack_2c.position.y = 0x20000;
    pSVar1 = Sprite_Create(&SStack_2c);
    param_1->objs[0].sprite = pSVar1;
    return;
}

void ov18_021F9508(PokedexEntryPopup *arg0) {
    Sprite_Delete(arg0->objs[0].sprite);
}
