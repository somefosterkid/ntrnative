#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225AB54_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225AB54_H

// UnkStruct_ov22_0225AB54: merged by tools/hg/promote.py from 20 m2c local copies (1 variants, base
// src/overlay_41_02246DE0_02246F08.c) and 6 accesses; fields at every accessed offset, pads elsewhere.

struct Sprite;
struct SpriteResource;
struct SysTask;
struct Window;

typedef struct UnkStruct_ov22_0225AB54 {
    struct SpriteResource *unk_00[4];
    struct Sprite *unk_10[2];
    struct Window *unk_18;
    int unk_1C;
    int unk_20;
    struct SysTask *unk_24;
    struct SysTask *unk_28;
    void *unk_2C;
    struct { BOOL unk_00; struct Sprite *unk_04[2]; struct { fx32 x; fx32 y; fx32 z; } unk_0C[2]; struct { fx32 unk_00; fx32 unk_04; fx32 unk_08; int unk_0C; int unk_10; } unk_24; struct { fx32 unk_00; fx32 unk_04; fx32 unk_08; int unk_0C; int unk_10; } unk_38; struct { fx32 unk_00; fx32 unk_04; fx32 unk_08; int unk_0C; int unk_10; } unk_4C; } unk_30;
    BOOL unk_90;
} UnkStruct_ov22_0225AB54;

// sizeof(UnkStruct_ov22_0225AB54) == 0x94

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225AB54_H
