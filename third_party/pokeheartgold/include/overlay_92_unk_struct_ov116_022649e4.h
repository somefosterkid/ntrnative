#ifndef POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_022649E4_H
#define POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_022649E4_H

#include "nnsys/fnd/allocator.h"

struct BgConfig;
struct Camera;
struct GF3DVramMan;
struct NARC;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;

// Shared by the ov116 (overlay 92) tasks: the graphics context every one of them borrows. The unk_2C name predates
// NNSFndAllocator (0x10 bytes, not 8): unk_2C really sits at 0x34.

typedef struct UnkStruct_ov116_0226469C {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov116_0226469C; // 0x18

typedef struct UnkStruct_ov116_022649E4_sub1 {
    u8 unk_00;
    u8 padding_01[3];
} UnkStruct_ov116_022649E4_sub1;

typedef struct UnkStruct_ov116_022649E4 {
    struct NARC *unk_00;
    struct NARC *unk_04;
    struct SpriteSystem *unk_08;
    struct SpriteManager *unk_0C;
    struct BgConfig *unk_10;
    struct PaletteData *unk_14;
    struct GF3DVramMan *unk_18;
    struct Camera *camera;
    u8 padding_20[4];
    NNSFndAllocator unk_24;
    UnkStruct_ov116_022649E4_sub1 unk_2C;
} UnkStruct_ov116_022649E4; // 0x38

#endif // POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_022649E4_H
