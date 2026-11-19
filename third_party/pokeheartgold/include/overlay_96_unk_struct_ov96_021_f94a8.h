#ifndef POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F94A8_H
#define POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F94A8_H

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_96_unk_struct_ov96_021_fab24.h"
#include "sprite.h"

// The 0x3C8-byte work object of the ov96 app run by ov96_021F94A8 (PokeathlonCourse_AllocPtr4FromHeap).
// hk191: split from UnkStruct_ov96_021FAB24 (now only the 0x6C lane row) and Ov96Work_021FB994 (removed).

// One CPU participant's lane (ov96_021FBA3C), 9 at +0x238.
typedef struct UnkStruct_ov96_021FBA3C {
    u8 state;
    u8 timer;
    u8 level;
    u8 chanceA;
    u8 chanceB;
    u8 unk05[3];
    f32 speed;
    f32 frac;
    f32 maxSpeed;
    f32 accel;
    s32 finished;
    s32 unk1C;
    s32 pos;
    s32 lastPos;
} UnkStruct_ov96_021FBA3C;

// sizeof(UnkStruct_ov96_021FBA3C) == 0x28

typedef struct UnkStruct_ov96_021F94A8 {
    enum HeapID heapID;              // 0x000
    BgConfig *bgConfig;              // 0x004
    Window window;                   // 0x008
    u8 unk_18[0xC0];                 // 0x018 not reached by the C code yet
    void *unk_D8;                    // 0x0D8 ov96_021FC028
    void *unk_DC;                    // 0x0DC ov96_021FC618
    UnkStruct_ov96_021FAB24 rows[3]; // 0x0E0
    void *unk_224;                   // 0x224 ov96_021EB180
    struct Ov96IconWork *unk_228;    // 0x228 ov96_021FC188
    s32 raceTime;                    // 0x22C frames while a lane still runs
    s32 frameCount;                  // 0x230
    u32 unk_234;                     // 0x234
    UnkStruct_ov96_021FBA3C cpu[9];  // 0x238
    void *unk_3A0;                   // 0x3A0 ov96_021E9A78
    void *unk_3A4;                   // 0x3A4 ov96_021EA854
    void *unk_3A8;                   // 0x3A8 ov96_021EB3E4
    Sprite *sprites[6];              // 0x3AC time digits
    u16 unk_3C4;
    u8 unk_3C6;
    u8 unk_3C7;
} UnkStruct_ov96_021F94A8;

// sizeof(UnkStruct_ov96_021F94A8) == 0x3c8

#endif // POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F94A8_H
