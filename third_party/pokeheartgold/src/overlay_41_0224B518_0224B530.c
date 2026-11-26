#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "overlay_41_unk_struct_ov22_0225af8c.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "bg_window.h"
#include "gf_3d_loader.h"
#include "overlay_41_unk_struct_ov22_02255cb8.h"
#include "filesystem.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_0225b4e4.h"
#include "overlay_41_unk_struct_ov22_0225b1bc.h"
#include "touchscreen.h"
#include "overlay_41_unk_struct_ov22_022597BC.h"

struct BgConfig;

struct DressUpPhoto;

struct PhotoAccessory;

struct PhotoPokemon;

struct Pokemon;

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteCharsTemplate;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

struct SoftwareSpritePaletteTemplate;

struct SpriteAnimFrame;


typedef struct PhotoAccessory PhotoAccessory;

typedef struct PhotoPokemon PhotoPokemon;

typedef struct DressUpPhoto DressUpPhoto;



typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

typedef struct SoftwareSpriteCharsTemplate SoftwareSpriteCharsTemplate;

typedef struct SoftwareSpritePaletteTemplate SoftwareSpritePaletteTemplate;



typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);







UnkStruct_ov22_0225B1BC *ov41_0224B630(const UnkStruct_ov22_0225B4E4 *param0);

void ov41_0224B8DC(UnkStruct_ov22_0225B4E4 *param0, const UnkStruct_ov22_0225AF8C *param1);

void ov41_0224B8F0(UnkStruct_ov22_0225B4E4 *param0, const DressUpPhoto *photo);

UnkStruct_ov22_0225B1BC *ov41_0224B530(const UnkStruct_ov22_0225AF8C *param0, const DressUpPhoto *photo);

UnkStruct_ov22_0225B1BC *ov41_0224B530(const UnkStruct_ov22_0225AF8C *param0, const DressUpPhoto *photo) {
    UnkStruct_ov22_0225B4E4 v0;

    ov41_0224B8DC(&v0, param0);
    ov41_0224B8F0(&v0, photo);

    return ov41_0224B630(&v0);
}
