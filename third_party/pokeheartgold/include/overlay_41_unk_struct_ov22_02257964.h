#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257964_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257964_H

#include "touchscreen.h"
#include "overlay_41_unk_struct_ov22_02254de0.h"
#include "overlay_41_unk_struct_ov22_02259484.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "overlay_41_unk_struct_ov22_022597BC.h"

struct BgConfig;
struct Pokemon;
struct Pokepic;
struct PokepicManager;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;

typedef struct UnkStruct_ov22_02257964_sub1 {
    UnkStruct_ov22_02259484 *unk_00;
    UnkStruct_ov22_02259560 unk_04;
    UnkStruct_ov22_02259560 unk_14;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_02257964_sub1;

typedef struct UnkStruct_ov22_02257964_sub2_sub {
    struct Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    struct { u8 unk_00; u8 unk_01; u8 unk_02; u8 unk_03; } unk_08;
    struct Pokemon *unk_0C;
} UnkStruct_ov22_02257964_sub2_sub;

typedef struct UnkStruct_ov22_02257964_sub2 {
    struct SoftwareSpriteManager *unk_00;
    struct SoftwareSpriteChars **unk_04;
    struct SoftwareSpritePalette **unk_08;
    u8 *unk_0C;
    struct PokepicManager *unk_10;
    struct BgConfig *unk_14;
    UnkStruct_ov22_02254DE0 *unk_18;
    UnkStruct_ov22_022597BC unk_1C;
    int unk_48;
    UnkStruct_ov22_02257964_sub2_sub unk_4C;
} UnkStruct_ov22_02257964_sub2;

typedef struct UnkStruct_ov22_02257964 {
    UnkStruct_ov22_02257964_sub1 unk_00;
    UnkStruct_ov22_02257964_sub2 unk_2C;
} UnkStruct_ov22_02257964;

// sizeof(UnkStruct_ov22_02257964) == 0x88 on DS

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257964_H
