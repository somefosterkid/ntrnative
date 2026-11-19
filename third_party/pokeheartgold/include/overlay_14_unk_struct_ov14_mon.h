#ifndef POKEHEARTGOLD_OVERLAY_14_UNK_STRUCT_OV14_MON_H
#define POKEHEARTGOLD_OVERLAY_14_UNK_STRUCT_OV14_MON_H

// UnkStruct_ov14_mon: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/overlay_14_021F50A0_021F5114.c) and 26 accesses; fields at every accessed offset, pads elsewhere.

#include "save_layout.h"

struct BoxPokemon;

typedef struct UnkStruct_ov14_mon {
    struct BoxPokemon *boxmon;
    u16 species;
    u16 item;
    u32 personality;
    u8 type1;
    u8 type2;
    u8 ability;
    u8 nature;
    u16 unk_10;
    union {
        u8 levelEgg;
        struct {
            u8 level : 7;
            u8 isEgg : 1;
        };
    };
    u8 gender : 7;
    u8 hasGender : 1;
    u16 msgIds[4];
} UnkStruct_ov14_mon;

// sizeof(UnkStruct_ov14_mon) == 0x1c on the DS (boxmon is a pointer: a host with 8-byte pointers has a bigger one, the allocation uses sizeof)
#ifdef __MWERKS__
LAYOUT_CHECK(UnkStruct_ov14_mon_size, sizeof(UnkStruct_ov14_mon) == 0x1C);
#endif

#endif // POKEHEARTGOLD_OVERLAY_14_UNK_STRUCT_OV14_MON_H
