#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite_system.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F1BC8(PokedexAppData *pokedexApp, u16 species, int a2, int a3);
#else
void ov18_021F1BC8(PokedexAppData *pokedexApp, int species, int a2, int a3);
#endif

enum NarcId ov18_021E5900(void);
s32 ov18_021E5908(void);
void ov18_021F1CB4(PokedexAppData *pokedexApp);
void ov18_021F1D58(PokedexAppData *pokedexApp);

void ov18_021F1CAC(PokedexAppData *pokedexApp, int species, int a2, int a3) {
    ov18_021F1BC8(pokedexApp, species, a2, a3);
}

void ov18_021F1CB4(PokedexAppData *pokedexApp) {
    enum NarcId narcId;

    narcId = ov18_021E5900();
    SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->unk_0668, pokedexApp->unk_066C, pokedexApp->gfxNarc, 0x4D, 1, 2, 0xC599);
    SpriteSystem_LoadPaletteBuffer(pokedexApp->paletteData, PLTTBUF_SUB_OBJ, pokedexApp->unk_0668, pokedexApp->unk_066C, narcId, ov18_021E5908(), 0, 1, 2, 0xC55B);
    SpriteSystem_LoadCellResObjFromOpenNarc(pokedexApp->unk_0668, pokedexApp->unk_066C, pokedexApp->gfxNarc, 0x4E, 1, 0xC558);
    SpriteSystem_LoadAnimResObjFromOpenNarc(pokedexApp->unk_0668, pokedexApp->unk_066C, pokedexApp->gfxNarc, 0x4F, 1, 0xC558);
}

void ov18_021F1D58(PokedexAppData *pokedexApp) {
    SpriteManager_UnloadCharObjById(pokedexApp->unk_066C, 0xC599U);
    SpriteManager_UnloadPlttObjById(pokedexApp->unk_066C, 0xC55BU);
    SpriteManager_UnloadCellObjById(pokedexApp->unk_066C, 0xC558U);
    SpriteManager_UnloadAnimObjById(pokedexApp->unk_066C, 0xC558U);
}
