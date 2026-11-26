#ifndef POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_022312B0_H
#define POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_022312B0_H

// UnkStruct_ov62_022312B0: merged by tools/hg/promote.py from 143 m2c local copies (1 variants, base
// src/overlay_40_0222C4F8_0222D55C_0222D5AC.c) and 4 accesses; fields at every accessed offset, pads elsewhere.

#include "obj_char_transfer_types.h"

struct FontOAM;
struct ManagedSprite;
struct UnkStruct_02012B20_t;

typedef struct UnkStruct_ov62_022312B0 {
    u8 padding_00[4];
    struct ManagedSprite *unk_08;
    struct FontOAM *unk_0C;
    struct UnkStruct_02012B20_t *unk_10;
    UnkStruct_02021AC8 unk_14;
} UnkStruct_ov62_022312B0;

// sizeof(UnkStruct_ov62_022312B0) == 0x1c

#endif // POKEHEARTGOLD_OVERLAY_40_UNK_STRUCT_OV62_022312B0_H
