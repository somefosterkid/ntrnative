#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225_E1_A4_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225_E1_A4_H

// UnkStruct_ov114_0225E1A4: merged by tools/hg/promote.py from 10 m2c local copies (1 variants, base
// src/overlay_90_0225892C.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct String;

typedef struct UnkStruct_ov114_0225E1A4 {
    struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } unk_00;
    u32 unk_10;
    struct String *unk_14;
    struct { fx32 unk_00; fx32 unk_04; fx32 unk_08; fx32 unk_0C; int unk_10; } unk_18;
    s16 unk_2C;
    s16 unk_2E;
} UnkStruct_ov114_0225E1A4;

// sizeof(UnkStruct_ov114_0225E1A4) == 0x30

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225_E1_A4_H
