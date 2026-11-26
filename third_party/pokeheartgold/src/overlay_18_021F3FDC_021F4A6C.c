#include "global.h"
struct PokedexAppData;

#include "player_data.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1324(void *, s32);

void ov18_021F4A6C(void *arg0);

void ov18_021F4A6C(void *arg0) {
    ov18_021F1324(arg0, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x4C, 1, 2, 0xC551);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(((PokedexAppData *)arg0)->paletteData, PLTTBUF_SUB_OBJ, ((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->unk_0858, (s32)sub_02074490(), 0, 3, 2, 0xC551);
    SpriteSystem_LoadCellResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x6A, 1, 0xC55A);
    SpriteSystem_LoadAnimResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x6B, 1, 0xC55A);
    SpriteSystem_LoadCellResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x70, 1, 0xC55B);
    SpriteSystem_LoadAnimResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x71, 1, 0xC55B);
    if (PlayerProfile_GetTrainerGender(((PokedexAppData *)arg0)->args->playerProfile) == 0) {
        SpriteSystem_LoadCharResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x69, 1, 1, 0xC59B);
        SpriteSystem_LoadCharResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x69, 1, 2, 0xC59C);
        SpriteSystem_LoadCharResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x6F, 1, 2, 0xC59D);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(((PokedexAppData *)arg0)->paletteData, PLTTBUF_MAIN_OBJ, ((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x6C, 0, 1, 1, 0xC55D);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(((PokedexAppData *)arg0)->paletteData, PLTTBUF_SUB_OBJ, ((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x6C, 0, 1, 2, 0xC55E);
        return;
    }
    SpriteSystem_LoadCharResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x6D, 1, 1, 0xC59B);
    SpriteSystem_LoadCharResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x6D, 1, 2, 0xC59C);
    SpriteSystem_LoadCharResObjFromOpenNarc(((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x72, 1, 2, 0xC59D);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(((PokedexAppData *)arg0)->paletteData, PLTTBUF_MAIN_OBJ, ((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x6E, 0, 1, 1, 0xC55D);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(((PokedexAppData *)arg0)->paletteData, PLTTBUF_SUB_OBJ, ((PokedexAppData *)arg0)->unk_0668, ((PokedexAppData *)arg0)->unk_066C, ((PokedexAppData *)arg0)->gfxNarc, 0x6E, 0, 1, 2, 0xC55E);
}

s32 ov18_021F13DC(void *, s32);

void ov18_021F4CC4(PokedexAppData *arg0);

void ov18_021F4CC4(PokedexAppData *arg0) {
    ov18_021F13DC(arg0, 1);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC551U);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC551U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC55AU);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC55AU);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC55BU);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC55BU);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC59BU);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC59CU);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC59DU);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC55DU);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC55EU);
}

s32 ov18_021F14FC(void *, u32, u32, s32);

s32 ov18_021F1534(void *, u32, u32, s32);

void ov18_021F4D64(struct PokedexAppData *arg0);

void ov18_021F4D64(PokedexAppData *arg0) {
    u8 v0;
    u8 v1;

    v0 = *((u8 *)&arg0->seenForms[0]);
    if (v0 & 0x80) {
        v1 = 0x80 ^ v0;
        if (arg0->curSpecies == 0xAC) {
            if (v1 == 2) {
                v1 = 1;
            } else {
                v1 = 0;
            }
        }
    } else {
        v1 = 0;
    }
    ov18_021F14FC(arg0, arg0->curSpecies, v1, 2);
    ov18_021F1534(arg0, arg0->curSpecies, v1, 3);
    ManagedSprite_SetAffineOverwriteMode(arg0->unk_0670[7], 2U);
    ManagedSprite_SetAffineZRotation(arg0->unk_0670[7], 0U);
    ManagedSprite_SetAffineTranslation(arg0->unk_0670[7], 0, -4);
}
