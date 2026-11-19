#include "global.h"
struct PokedexAppData;

#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F2648(struct PokedexAppData *arg0);

void ov18_021F281C(PokedexAppData *arg0);

s32 ov18_021F1424(PokedexAppData *, s32);
s32 ov18_021F1A30(PokedexAppData *, s32);
s32 ov18_021F1D98(PokedexAppData *, s32);
s32 ov18_021F1FDC(PokedexAppData *, s32);
s32 ov18_021F2468(PokedexAppData *);
s32 ov18_021F2964(void *arg0);
s32 ov18_021F299C(PokedexAppData *);

void ov18_021F2648(PokedexAppData *arg0) {
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0xC, 1, 1, 0xC590);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_MAIN_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0xF, 0, 5, 1, 0xC556);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0xD, 1, 0xC552);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0xE, 1, 0xC552);
}

void ov18_021F26E4(PokedexAppData *arg0);

void ov18_021F26E4(PokedexAppData *arg0) {
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC590U);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC556U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC552U);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC552U);
}

void ov18_021F2724(struct PokedexAppData *arg0);

void ov18_021F2724(PokedexAppData *arg0) {
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x1A, 1, 2, 0xC591);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x20, 0, 1, 2, 0xC557);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x1B, 1, 0xC553);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x1C, 1, 0xC553);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x1D, 1, 2, 0xC592);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x1E, 1, 0xC554);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x1F, 1, 0xC554);
}

void ov18_021F281C(PokedexAppData *arg0) {
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC591U);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC557U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC553U);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC553U);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC592U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC554U);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC554U);
}

void ov18_021F2880(PokedexAppData *pokedexApp) {
    u16 species;
    u32 a2;

    ov18_021F2964(pokedexApp);
    ov18_021F1424(pokedexApp, 0x18);
    ov18_021F1620(pokedexApp, 0x18);
    ov18_021F299C(pokedexApp);
    if (pokedexApp->unk_1860 == 0) {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[0], 0);
    } else {
        ov18_021F2AC0(pokedexApp, 0);
    }
    ov18_021F2BB0(pokedexApp, 5);
    ov18_021F2C10(pokedexApp, 2, 1);
    ov18_021F2C5C(pokedexApp, 1, 1);
    ov18_021F2E80(pokedexApp, 1, 1);
    species = ov18_021F8838(pokedexApp);
    a2 = ov18_021F8824(pokedexApp);
    ov18_021F1A30(pokedexApp, 0xB);
    ov18_021F1CAC(pokedexApp, (s32)species, 0xB, 0xA);
    ov18_021F1FDC(pokedexApp, 0xE);
    ov18_021F209C(pokedexApp, (u32)species, (s32)a2, 0xE);
    ov18_021F1D98(pokedexApp, 0xD);
    ov18_021F1DE4(pokedexApp, (u32)species, (s32)a2, 0xD);
    ov18_021F2EC8(pokedexApp, (s32)a2, 9);
    ov18_021F2468(pokedexApp);
    ov18_021F2530(pokedexApp, (u32)species, 0x12);
    ov18_021F24E0(pokedexApp, (u32)species, 8);
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[8], 0);
}
