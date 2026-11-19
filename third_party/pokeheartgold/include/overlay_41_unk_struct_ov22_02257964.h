#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257964_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257964_H

// UnkStruct_ov22_02257964: merged by tools/hg/promote.py from 30 m2c local copies (1 variants, base
// src/overlay_41_02246DE0_02246F08.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct Pokemon;
struct PokemonSprite;
struct PokemonSpriteManager;
struct SoftwareSpriteManager;
struct UnkStruct_ov22_02259560_t;

typedef struct UnkStruct_ov22_02257964 {
    struct { void *unk_00; struct { void *unk_00; int unk_04; struct UnkStruct_ov22_02259560_t *unk_08; struct UnkStruct_ov22_02259560_t *unk_0C; } unk_04; struct { void *unk_00; int unk_04; struct UnkStruct_ov22_02259560_t *unk_08; struct UnkStruct_ov22_02259560_t *unk_0C; } unk_14; int unk_24; int unk_28; } unk_00;
    struct { struct SoftwareSpriteManager *unk_00; void **unk_04; void **unk_08; u8 *unk_0C; struct PokemonSpriteManager *unk_10; struct BgConfig *unk_14; void *unk_18; struct { struct BgConfig *unk_00; u32 narcID; int unk_08; int unk_0C; int unk_10; int unk_14; int unk_18; int unk_1C; int unk_20; int unk_24; int unk_28; } unk_1C; int unk_48; struct { struct PokemonSprite *unk_00; u8 unk_04[0x4]; struct { u8 unk_00; u8 unk_01; u8 unk_02; u8 unk_03; } unk_08; struct Pokemon *unk_0C; } unk_4C; } unk_2C;
} UnkStruct_ov22_02257964;

// sizeof(UnkStruct_ov22_02257964) == 0x88

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257964_H
