#include "global.h"
struct PokedexAppData;

#include "filesystem.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F1314(struct PokedexAppData *arg0);

void ov18_021F1324(struct PokedexAppData *arg0, u32 arg1);
void ov18_021F13DC(PokedexAppData *arg0, u32 arg1);

void ov18_021F1314(PokedexAppData *arg0) {
    NARC_Delete(arg0->unk_0858);
}

void ov18_021F1324(PokedexAppData *arg0, u32 arg1) {
    u32 v0;

    v0 = 0;
    if (arg1 != 0) {
        do {
            SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->gfxNarc, 0x4C, 1, 1, v0 + 0xC550);
            v0 += 1;
        } while (v0 < arg1);
    }
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->paletteData, PLTTBUF_MAIN_OBJ, arg0->unk_0668, arg0->unk_066C, arg0->unk_0858, (s32)sub_02074490(), 0, 3, 1, 0xC550);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->unk_0858, (s32)sub_0207449C(), 0, 0xC550);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_0668, arg0->unk_066C, arg0->unk_0858, (s32)sub_020744A8(), 0, 0xC550);
}

void ov18_021F13DC(PokedexAppData *arg0, u32 arg1) {
    u32 v0;

    v0 = 0;
    if (arg1 != 0) {
        do {
            SpriteManager_UnloadCharObjById(arg0->unk_066C, v0 + 0xC550);
            v0 += 1;
        } while (v0 < arg1);
    }
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC550U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC550U);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC550U);
}
