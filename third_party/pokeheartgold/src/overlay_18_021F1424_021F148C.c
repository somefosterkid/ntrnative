#include "global.h"
struct PokedexAppData;

#include "gf_gfx_loader.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

void *ov18_021F148C(struct PokedexAppData *arg0, u32 arg1, u32 arg2, NNSG2dCharacterData **arg3);

void *ov18_021F148C(PokedexAppData *arg0, u32 species, u32 form, NNSG2dCharacterData **ppCharData) {
    return GfGfxLoader_GetCharDataFromOpenNarc(arg0->unk_0858, (s32)GetBattleMonIconNaixEx(species, 0, form), 0, ppCharData, HEAP_ID_POKEDEX_APP);
}

s32 ov18_021F111C(PokedexAppData *, s32, void *, s32, s32);

void ov18_021F14B4(PokedexAppData *arg0, void *arg1, s32 arg2, s32 arg3);

void ov18_021F14B4(PokedexAppData *arg0, void *arg1, s32 arg2, s32 arg3) {
    void *v0;
    s32 v1;

    v0 = arg1;
    v1 = SpriteManager_FindPlttResourceOffset(arg0->unk_066C, 0xC550, 1);
    ov18_021F111C(arg0, arg2, v0, 0x200, 1);
    ManagedSprite_SetPaletteOverride(arg0->unk_0670[arg2], v1 + arg3);
}
