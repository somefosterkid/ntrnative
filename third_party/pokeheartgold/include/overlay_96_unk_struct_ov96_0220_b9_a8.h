#ifndef POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_0220_B9_A8_H
#define POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_0220_B9_A8_H

// UnkStruct_ov96_0220B9A8: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_96_0220B7F4_0220B9A8_0220B9A8.c) and 24 accesses; fields at every accessed offset, pads elsewhere.

struct ManagedSprite;
struct PokeathlonCourseData;

typedef struct UnkStruct_ov96_0220B9A8 {
    u8 pad0[12];
    struct PokeathlonCourseData *course;
    struct ManagedSprite *spr10;
    struct ManagedSprite *spr14;
    struct ManagedSprite *spr18;
    struct ManagedSprite *spr1C;
    struct ManagedSprite *spr20;
    struct ManagedSprite *spr24;
    struct ManagedSprite *spr28;
    s32 w2C;
    s32 w30;
    u32 unk_34;
    u8 pad_38[0x8];
    u32 sel : 2;
    u32 pad40 : 30;
    u32 pad44a : 10;
    u32 state : 6;
    u32 pad44b : 8;
    u32 cnt : 8;
} UnkStruct_ov96_0220B9A8;

// sizeof(UnkStruct_ov96_0220B9A8) == 0x48

#endif // POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_0220_B9_A8_H
