#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "field/field_sprite_manager.h"

#include "bg_window.h"
#include "sprite.h"
#include "unk_02009D48.h"

typedef struct PokemonPreview {
    FieldSpriteManager spriteManager;
    ManagedSprite *managedSprite;
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 x;
    u8 y;
    u8 state;
} PokemonPreview;

void sub_0200F82C(PokemonPreview *preview, u8 palette, u16 tile);

void sub_0200F82C(PokemonPreview *preview, u8 palette, u16 tile) {
    FillBgTilemapRect(preview->bgConfig,
        preview->bgLayer,
        tile,
        preview->x - 1,
        preview->y - 1,
        1,
        1,
        palette);
    FillBgTilemapRect(preview->bgConfig,
        preview->bgLayer,
        tile + 1,
        preview->x,
        preview->y - 1,
        10,
        1,
        palette);
    FillBgTilemapRect(preview->bgConfig,
        preview->bgLayer,
        tile + 2,
        preview->x + 10,
        preview->y - 1,
        1,
        1,
        palette);
    FillBgTilemapRect(preview->bgConfig,
        preview->bgLayer,
        tile + 4,
        preview->x,
        preview->y,
        10,
        10,
        palette);
    FillBgTilemapRect(preview->bgConfig,
        preview->bgLayer,
        tile + 3,
        preview->x - 1,
        preview->y,
        1,
        10,
        palette);
    FillBgTilemapRect(preview->bgConfig,
        preview->bgLayer,
        tile + 5,
        preview->x + 10,
        preview->y,
        1,
        10,
        palette);
    FillBgTilemapRect(preview->bgConfig,
        preview->bgLayer,
        tile + 6,
        preview->x - 1,
        preview->y + 10,
        1,
        1,
        palette);
    FillBgTilemapRect(preview->bgConfig,
        preview->bgLayer,
        tile + 7,
        preview->x,
        preview->y + 10,
        10,
        1,
        palette);
    FillBgTilemapRect(preview->bgConfig,
        preview->bgLayer,
        tile + 8,
        preview->x + 10,
        preview->y + 10,
        1,
        1,
        palette);

    ScheduleBgTilemapBufferTransfer(preview->bgConfig, preview->bgLayer);
}

void sub_0200F9DC(PokemonPreview *preview);

void sub_0200F9DC(PokemonPreview *preview) {
    FillBgTilemapRect(preview->bgConfig,
        preview->bgLayer,
        0,
        preview->x - 1,
        preview->y - 1,
        10 + 2,
        10 + 2,
        0);
    ScheduleBgTilemapBufferTransfer(preview->bgConfig, preview->bgLayer);
}
