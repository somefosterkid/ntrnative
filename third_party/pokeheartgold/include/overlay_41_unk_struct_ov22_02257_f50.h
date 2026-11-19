#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257_F50_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257_F50_H

// UnkStruct_ov22_02257F50: merged by tools/hg/promote.py from 27 m2c local copies (1 variants, base
// src/overlay_41_02246DE0_02246F08.c) and 10 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct SoftwareSpriteManager;
struct UnkStruct_ov22_02259560_t;

typedef struct UnkStruct_ov22_02257F50 {
    struct { int unk_00; void *unk_04; void *unk_08; struct { struct UnkStruct_ov22_02259560_t *unk_00; int unk_04; int unk_08; } unk_0C[4]; int unk_3C; int unk_40; } unk_00;
    struct { struct SoftwareSpriteManager *unk_00; void **unk_04; void **unk_08; u8 *unk_0C; struct BgConfig *unk_10; void *unk_14; struct { struct BgConfig *unk_00; u32 narcID; int unk_08; int unk_0C; int unk_10; int unk_14; int unk_18; int unk_1C; int unk_20; int unk_24; int unk_28; } unk_18; void *unk_44; } unk_44;
} UnkStruct_ov22_02257F50;

// sizeof(UnkStruct_ov22_02257F50) == 0x8c

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02257_F50_H
