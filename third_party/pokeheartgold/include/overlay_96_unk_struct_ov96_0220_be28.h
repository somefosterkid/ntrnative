#ifndef POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_0220_BE28_H
#define POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_0220_BE28_H

// UnkStruct_ov96_0220BE28: merged by tools/hg/promote.py from 2 m2c local copies (2 variants, base
// src/overlay_96_0220B7F4_0220B9A8_0220BE28_0220BE28.c) and 40 accesses; fields at every accessed offset, pads elsewhere.

struct ManagedSprite;
struct PokeathlonCourseData;

typedef struct UnkStruct_ov96_0220BE28 {
    struct PokeathlonCourseData *course;
    s32 w4[3];
    u32 w10[3];
    struct ManagedSprite *spr1C;
    struct ManagedSprite *spr20;
    struct ManagedSprite *spr24;
    struct ManagedSprite *spr28;
    struct ManagedSprite *spr2C;
    struct ManagedSprite *spr30;
    u32 w34;
    u32 pad38 : 25;
    u32 a : 2;
    u32 b : 2;
    u32 pad38b : 3;
} UnkStruct_ov96_0220BE28;

// sizeof(UnkStruct_ov96_0220BE28) == 0x3c

#endif // POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_0220_BE28_H
