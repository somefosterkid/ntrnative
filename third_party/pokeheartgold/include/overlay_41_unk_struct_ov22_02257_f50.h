#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257_F50_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257_F50_H

#include "overlay_41_unk_struct_ov22_0225899C.h"
#include "overlay_41_unk_struct_ov22_02254de0.h"
#include "overlay_41_unk_struct_ov22_02259484.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "overlay_41_unk_struct_ov22_022597BC.h"

struct BgConfig;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;

typedef struct UnkStruct_ov22_02257F50_sub1_sub1 {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_ov22_02257F50_sub1_sub1;

typedef struct UnkStruct_ov22_02257F50_sub1 {
    int unk_00;
    UnkStruct_ov22_0225899C *unk_04;
    UnkStruct_ov22_02259484 *unk_08;
    UnkStruct_ov22_02257F50_sub1_sub1 unk_0C[4];
    int unk_3C;
    int unk_40;
} UnkStruct_ov22_02257F50_sub1;

typedef struct UnkStruct_ov22_02257F50_sub2 {
    struct SoftwareSpriteManager *unk_00;
    struct SoftwareSpriteChars **unk_04;
    struct SoftwareSpritePalette **unk_08;
    u8 *unk_0C;
    struct BgConfig *unk_10;
    UnkStruct_ov22_02254DE0 *unk_14;
    UnkStruct_ov22_022597BC unk_18;
    UnkStruct_ov22_02254DE0 *unk_44;
} UnkStruct_ov22_02257F50_sub2;

typedef struct UnkStruct_ov22_02257F50 {
    UnkStruct_ov22_02257F50_sub1 unk_00;
    UnkStruct_ov22_02257F50_sub2 unk_44;
} UnkStruct_ov22_02257F50;

// sizeof(UnkStruct_ov22_02257F50) == 0x8c on DS

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257F50_H
