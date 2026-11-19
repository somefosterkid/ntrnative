#include "global.h"
struct PokedexAppData;

#include "gf_gfx_loader.h"
#include "heap.h"
#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F111C(PokedexAppData *, s32, void *, s32, s32);

s32 ov18_021F967C(s32);

u8 ov18_021F9688(s32);

void ov18_021F21FC(PokedexAppData *arg0, s32 arg1, s32 arg2);

void ov18_021F21FC(PokedexAppData *arg0, s32 arg1, s32 arg2) {
    NNSG2dCharacterData *ppCharData;
    s32 v0;
    s32 v1;
    void *ptr;

    v0 = arg2;
    ptr = GfGfxLoader_GetCharDataFromOpenNarc(arg0->gfxNarc, ov18_021F967C(arg2), 1, &ppCharData, HEAP_ID_POKEDEX_APP);
    ov18_021F111C(arg0, arg1, ppCharData->pRawData, 0x180, 2);
    v1 = SpriteManager_FindPlttResourceOffset(arg0->unk_066C, 0xC558, 2);
    ManagedSprite_SetPaletteOverride(arg0->unk_0670[arg1], v1 + ov18_021F9688(v0));
    Heap_Free(ptr);
}

void ov18_021F2270(struct PokedexAppData *arg0);

void ov18_021F2270(PokedexAppData *arg0) {
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x35, 1, 1, 0xC597);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_MAIN_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x38, 0, 1, 1, 0xC559);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x36, 1, 0xC556);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x37, 1, 0xC556);
}

void ov18_021F2308(PokedexAppData *arg0);

void ov18_021F2308(PokedexAppData *arg0) {
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC597U);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC559U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC556U);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC556U);
}

void ov18_021F2348(struct PokedexAppData *arg0);

void ov18_021F2348(PokedexAppData *arg0) {
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x35, 1, 2, 0xC598);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x38, 0, 1, 2, 0xC55A);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x36, 1, 0xC557);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x37, 1, 0xC557);
}

void ov18_021F23E4(PokedexAppData *arg0);

void ov18_021F23E4(PokedexAppData *arg0) {
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC598U);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC55AU);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC557U);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC557U);
}

void ov18_021F2424(PokedexAppData *arg0, s32 arg1, ManagedSpriteTemplate *arg2);

void ov18_021F2424(PokedexAppData *arg0, s32 arg1, ManagedSpriteTemplate *unkTemplate) {
    if (unkTemplate->vramTransfer == 1) {
        arg0->unk_0670[arg1] = SpriteSystem_NewSprite(arg0->unk_0668, arg0->unk_066C, unkTemplate);
        return;
    }
    arg0->unk_0670[arg1] = SpriteSystem_NewSpriteWithYOffset(arg0->unk_0668, arg0->unk_066C, unkTemplate, 0x200000);
}
