#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225A0E4_HEAD_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225A0E4_HEAD_H

#include "g2d_renderer.h"

// UnkStruct_ov22_0225A0E4_Head: the sprite/BG environment at the start of both ov41 objects that call the shared
// ov41_022461xx..ov41_02246Bxx helpers: the 0x268-byte UnkStruct_ov22_0225B1BC (as its first member) and the 0x6f0-byte
// UnkStruct_ov22_0225A0E4 (which repeats these fields inline up to unk_5C). Field names keep the offsets of the m2c
// copies, so unk_5C (the NARC) really sits at 0x180.

struct BgConfig;
struct GF_2DGfxRawResMan;
struct NARC;
struct NNSG2dCharacterData;
struct PokepicManager;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResourceCollection;

typedef struct UnkStruct_ov22_0225A0E4_Head {
    struct SoftwareSpriteManager *unk_00;
    struct SoftwareSpriteChars **unk_04;
    int unk_08;
    int unk_0C;
    struct SoftwareSpritePalette **unk_10;
    int unk_14;
    int unk_18;
    BOOL unk_1C;
    struct PokepicManager *unk_20;
    u32 unk_24;
    u32 unk_28;
    BOOL unk_2C;
    u8 *unk_30;
    struct GF_2DGfxRawResMan *unk_34;
    struct NNSG2dCharacterData **unk_38;
    int unk_3C;
    struct BgConfig *unk_40;
    struct SpriteList *unk_44;
    struct SpriteResourceCollection *unk_48[4];
    G2dRenderer unk_58;
    struct NARC *unk_5C;
} UnkStruct_ov22_0225A0E4_Head;

// sizeof(UnkStruct_ov22_0225A0E4_Head) == 0x184 on the DS

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225A0E4_HEAD_H
