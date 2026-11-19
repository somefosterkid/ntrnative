#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225A154_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225A154_H

// UnkStruct_ov22_0225A154: merged by tools/hg/promote.py from 13 m2c local copies (1 variants, base
// src/overlay_41_02246DE0_02246F08.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct FontOAM;
struct Sprite;

typedef struct UnkStruct_ov22_0225A154 {
    struct { struct Sprite *unk_00; int unk_04; void *unk_08; void *unk_0C; } unk_00;
    struct FontOAM *unk_10;
    struct { u32 size; u32 offset; u16 vramType; u16 atEnd; } unk_14;
} UnkStruct_ov22_0225A154;

// sizeof(UnkStruct_ov22_0225A154) == 0x20

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225A154_H
