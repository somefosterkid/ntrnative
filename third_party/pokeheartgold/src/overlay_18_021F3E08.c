#include "global.h"
struct PokedexAppData;

#include "application/pokedex/pokedex_internal.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"

s32 ov18_021F10E8(PokedexAppData *, u32);

s32 ov18_021F3FDC(PokedexAppData *);

void ov18_021F3E24(struct PokedexAppData *arg0);

void ov18_021F3E08(PokedexAppData *pokedexApp) {
    u32 v0;

    v0 = 1;
    do {
        ov18_021F10E8(pokedexApp, v0);
        v0 += 1;
    } while (v0 < 0x3BU);
    ov18_021F3FDC(pokedexApp);
}

void ov18_021F3E24(PokedexAppData *arg0) {
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x4C, 1, 2, 0xC550);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->unk_0858, (s32)sub_02074490(), 0, 3, 2, 0xC551);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->unk_0858, (s32)sub_0207449C(), 0, 0xC550);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->unk_0858, (s32)sub_020744A8(), 0, 0xC550);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x73, 1, 2, 0xC59C);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x76, 0, 1, 2, 0xC55E);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x74, 1, 0xC55A);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x75, 1, 0xC55A);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x77, 1, 2, 0xC59A);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x7A, 0, 2, 2, 0xC55C);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x78, 1, 0xC559);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x79, 1, 0xC559);
}
