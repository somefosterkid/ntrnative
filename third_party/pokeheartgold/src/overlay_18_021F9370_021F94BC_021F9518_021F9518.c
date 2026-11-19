#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "overlay_18_pokedex_entry_popup.h"

void ov18_021F9310(void *, void *, SpriteResourcesHeader *, u32);

void *ov18_021F9694(u32, u32);


void ov18_021F9518(PokedexEntryPopup *param_1) {
    Sprite *pSVar1;
    void *startAddr;
    NNSG2dImageProxy *pImg;
    u32 offset;
    SimpleSpriteTemplate SStack_2c;
    SpriteResourcesHeader SStack_50;
    ov18_021F9310(param_1->objs[3].res, param_1->resMan, &SStack_50, 1);
    SStack_2c.spriteList = param_1->spriteList;
    SStack_2c.header = &SStack_50;
    SStack_2c.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    SStack_2c.priority = 0;
    SStack_2c.heapID = param_1->heapId;
    SStack_2c.position.x = 0x78000;
    SStack_2c.position.y = 0x50000;
    pSVar1 = Sprite_Create(&SStack_2c);
    param_1->objs[3].sprite = pSVar1;
    startAddr = (void *)ov18_021F9694(param_1->species, param_1->heapId);
    pImg = Sprite_GetImageProxy(param_1->objs[3].sprite);
    offset = NNS_G2dGetImageLocation(pImg, NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(startAddr, 0x80);
    GX_LoadOBJ(startAddr, offset, 0x80);
    Heap_Free(startAddr);
    return;
}
