#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "bg_window.h"
#include "filesystem.h"
#include "overlay_74_main_menu_util.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "pokemon_types_def.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "pokepic.h"
#include "overlay_41_unk_struct_ov22_0225b1bc.h"
#include "filesystem_files_def.h"

struct Pokepic;
struct PokepicDrawParam;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

void ov80_0222C03C(UnkStruct_ov22_0225B1BC *param0, BOOL param1);

void ov80_0222C03C(UnkStruct_ov22_0225B1BC *param0, BOOL param1) {
    param0->unk_00.unk_1C = param1;
}
