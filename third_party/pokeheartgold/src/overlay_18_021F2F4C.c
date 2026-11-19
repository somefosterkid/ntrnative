#include "global.h"
struct PokedexAppData;

#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1324(void *, s32);

s32 ov18_021F2270(void *);

s32 ov18_021F2648(void *);

void ov18_021F2F4C(struct PokedexAppData *arg0);

void ov18_021F2F4C(PokedexAppData *arg0) {
    ov18_021F1324(arg0, 0x3C);
    ov18_021F2648(arg0);
    ov18_021F2270(arg0);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x48, 1, 1, 0xC5A0);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_MAIN_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x4B, 0, 1, 1, 0xC561);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x49, 1, 0xC55E);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x4A, 1, 0xC55E);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x48, 1, 2, 0xC59F);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x4B, 0, 1, 2, 0xC560);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x49, 1, 0xC55D);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x4A, 1, 0xC55D);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x17, 1, 2, 0xC59E);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x20, 0, 1, 2, 0xC55F);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x18, 1, 0xC55C);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x19, 1, 0xC55C);
}

s32 ov18_021F13DC(void *, s32);

s32 ov18_021F2308(void *);

s32 ov18_021F26E4(void *);

void ov18_021F310C(PokedexAppData *arg0);

void ov18_021F310C(PokedexAppData *arg0) {
    ov18_021F13DC(arg0, 0x3C);
    ov18_021F26E4(arg0);
    ov18_021F2308(arg0);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC59FU);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC560U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC55DU);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC55DU);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC5A0U);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC561U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC55EU);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC55EU);
}
