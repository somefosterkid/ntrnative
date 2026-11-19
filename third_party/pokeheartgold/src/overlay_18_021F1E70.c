#include "global.h"
struct PokedexAppData;

#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F1E70(struct PokedexAppData *arg0);

void ov18_021F1E70(PokedexAppData *arg0) {
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x24, 1, 2, 0xC593);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x24, 1, 2, 0xC594);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x24, 1, 2, 0xC595);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x24, 1, 2, 0xC596);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x23, 0, 4, 2, 0xC558);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x21, 1, 0xC555);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x22, 1, 0xC555);
}

void ov18_021F1F74(PokedexAppData *arg0);

void ov18_021F1F74(PokedexAppData *arg0) {
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC593U);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC594U);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC595U);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC596U);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC558U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC555U);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC555U);
}
