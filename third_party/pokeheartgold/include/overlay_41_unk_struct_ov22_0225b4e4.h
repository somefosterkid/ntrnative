#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225B4E4_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225B4E4_H

// UnkStruct_ov22_0225B4E4: merged by tools/hg/promote.py from 5 m2c local copies (1 variants, base
// src/overlay_41_0224B518_0224B530.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

#include "constants/heap.h"

struct BgConfig;
struct PhotoAccessory;
struct PhotoPokemon;

typedef struct UnkStruct_ov22_0225B4E4 {
    struct PhotoPokemon *photoMon;
    struct PhotoAccessory *unk_04[20];
    int unk_54;
    struct BgConfig *unk_58;
    int unk_5C;
    int unk_60;
    int unk_64;
    enum HeapID heapID;
} UnkStruct_ov22_0225B4E4;

// sizeof(UnkStruct_ov22_0225B4E4) == 0x6c

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225B4E4_H
