#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_RD_OBJ_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_RD_OBJ_H

// Ov96RdObj: merged by tools/hg/promote.py from 3 m2c local copies (3 variants, base
// src/overlay_96_02217E08_02217E7C.c) and 127 accesses; fields at every accessed offset, pads elsewhere.

struct PokeathlonCourseData;

typedef struct Ov96RdPos16 {
    s16 x;
    s16 y;
} Ov96RdPos16;

// one runner's per-event stats (0x1c): Ov96RdWork holds [4][3] of them at 0x30 (ov96_0221918C fills them,
// ov96_0221935C returns one); s2 is the max of Ov96RdEntry.value
typedef struct Ov96RdStats {
    s32 s0;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 s5;
    u8 k2;
} Ov96RdStats;

typedef struct Ov96RdEntry {
    void *sprite;
    Ov96RdStats *stats;
    s32 value;
    u32 idx : 4;
    u32 rest : 28;
} Ov96RdEntry;

typedef struct Ov96RdPlayer Ov96RdPlayer;

typedef struct Ov96RdObj {
    struct PokeathlonCourseData *data;
    Ov96RdEntry *info;
    Ov96RdPlayer *owner;
    void *unkC; // = Ov96RdWork.unk_180
    struct Ov96RdObj *link; // the Ov96RdObj that hit this one (ov96_022178A0)
    s32 state;
    s32 lastState; // state of the last Ov96RdObj that hit this one
    Ov96RdPos16 target;
    VecFx32 pos20;
    VecFx32 pos2C;
    VecFx32 dir;
    VecFx32 vel;
    s32 unk_50;
    s32 w54;
    s8 timer58;
    s8 timer59;
    s8 timer5A;
    s8 timer5B;
    s8 timer5C;
    s8 b5D;
    s8 timer5E;
    u8 b5F;
    u32 cnt : 12;
    u32 a : 2;
    u32 mode : 2;
    u32 f16 : 4;
    u32 d : 2;
    u32 e : 2;
    u32 f24 : 2;
    u32 f26 : 1;
    u32 f27 : 1;
    u32 f28 : 1;
    u32 f29 : 1;
    u32 f30 : 1;
    u32 f31 : 1;
    s32 w64;
    Ov96RdEntry entries[3];
} Ov96RdObj;

// sizeof(Ov96RdObj) == 0x98

typedef struct Ov96RdFlags {
    u32 f0 : 14;
    u32 b14 : 1;
    u32 b15 : 1;
    u32 cnt16 : 13;
    u32 b29 : 1;
    u32 b30 : 1;
    u32 b31 : 1;
} Ov96RdFlags;

// one runner (0xa8); Ov96RdWork holds 4 of them at 0x198
struct Ov96RdPlayer {
    Ov96RdPos16 pos;
    s16 tx;
    s16 ty;
    Ov96RdFlags flags;
    Ov96RdObj obj;
    u32 index;
};

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_RD_OBJ_H
