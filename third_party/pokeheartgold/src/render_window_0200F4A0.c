#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "field/field_sprite_manager.h"

#include "bg_window.h"
#include "gf_3d_loader.h"
#include "pokemon_types_def.h"
#include "seal_case.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"






typedef struct PokemonPreview {
    FieldSpriteManager spriteManager;
    ManagedSprite *managedSprite;
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 x;
    u8 y;
    u8 state;
} PokemonPreview;

#if defined(SDK_PORT) || defined(NONMATCHING)
PokemonPreview *sub_0200F5C4(BgConfig *bgConfig, s8 bgLayer, s8 x, s8 y, u32 heapID);
#else
PokemonPreview *sub_0200F5C4(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u32 heapID);
#endif

void sub_0200F600(PokemonPreview *preview, u32 heapID);

void sub_0200F62C(PokemonPreview *preview);

void sub_0200F684(PokemonPreview *preview, u8 x, u8 y);

void sub_0200F6D4(FieldSpriteManager *param0, u16 species, u8 gender);

void sub_0200F82C(PokemonPreview *preview, u8 palette, u16 tile);

u8 *DrawPokemonPicFromSpecies(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, u16 species, u8 gender, enum HeapID heapID);

void sub_0200F714(FieldSpriteManager *param0, Pokemon *mon);
u8 *DrawPokemonPicFromMon(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, Pokemon *mon, enum HeapID heapID);

u8 *DrawPokemonPicFromSpecies(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, u16 species, u8 gender, enum HeapID heapID) {
    PokemonPreview *preview = sub_0200F5C4(bgConfig, bgLayer, x, y, heapID);

    sub_0200F600(preview, heapID);
    sub_0200F62C(preview);
    sub_0200F684(preview, x, y);
    sub_0200F6D4(&preview->spriteManager, species, gender);
    sub_0200F82C(preview, palette, baseTile);
    BgCommitTilemapBufferToVram(bgConfig, bgLayer);

    return &preview->state;
}

u8 *DrawPokemonPicFromMon(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, Pokemon *mon, enum HeapID heapID) {
    PokemonPreview *preview = sub_0200F5C4(bgConfig, bgLayer, x, y, heapID);

    sub_0200F600(preview, heapID);
    sub_0200F62C(preview);
    sub_0200F684(preview, x, y);
    sub_0200F714(&preview->spriteManager, mon);
    sub_0200F82C(preview, palette, baseTile);
    BgCommitTilemapBufferToVram(bgConfig, bgLayer);

    return &preview->state;
}
