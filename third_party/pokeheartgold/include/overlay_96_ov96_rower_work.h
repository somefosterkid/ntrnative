#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_ROWER_WORK_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_ROWER_WORK_H

#include "bg_window.h"
#include "sprite.h"
#include "overlay_96_ov96_mon_icon_work.h"

// Pokeathlon course work area (PokeathlonCourse_AllocPtr4FromHeap(course, 0x648)), four rowers.

typedef struct Ov96RowerSub {
    f32 unk00;    // 0x00
    f32 unk04;    // 0x04
    u8 unk08[4];  // 0x08
    f32 unk0C;    // 0x0C
    u8 unk10[4];  // 0x10
    u8 unk14;     // 0x14
    u8 unk15;     // 0x15
    u8 unk16;     // 0x16
    u8 unk17;     // 0x17
    u8 unk18;     // 0x18
    u8 unk19[3];  // 0x19
} Ov96RowerSub; // size = 0x1C

typedef struct Ov96Rower {
    struct Ov96SpriteSetElem *unk00[3]; // 0x00
    s32 unk0C[3];           // 0x0C
    Ov96RowerSub subs[3];   // 0x18
    void *unk6C;            // 0x6C
    void *unk70;            // 0x70
    void *unk74;            // 0x74
    void *unk78;            // 0x78
    s32 unk7C;              // 0x7C
    s32 unk80;              // 0x80
    u8 unk84[7];            // 0x84
    u8 subIdx;              // 0x8B
    VecFx32 unk8C;          // 0x8C
    u8 unk98[4];            // 0x98
    u8 unk9C;               // 0x9C
    u8 unk9D;               // 0x9D
    u8 unk9E;               // 0x9E
    u8 unk9F;               // 0x9F
    u8 unkA0;               // 0xA0
    u8 unkA1;               // 0xA1
    u8 unkA2;               // 0xA2
    u8 unkA3;               // 0xA3
    u8 unkA4;               // 0xA4
    u8 unkA5;               // 0xA5
    u8 unkA6[3];            // 0xA6
    u8 unkA9;               // 0xA9
    u8 unkAA;               // 0xAA
    u8 unkAB;               // 0xAB
    s32 unkAC;              // 0xAC
    VecFx32 unkB0;          // 0xB0
    VecFx32 unkBC;          // 0xBC
    s32 unkC8;              // 0xC8
    u8 unkCC[4];            // 0xCC
    u8 unkD0;               // 0xD0
    u8 unkD1;               // 0xD1
    u8 unkD2[2];            // 0xD2
} Ov96Rower; // size = 0xD4

typedef struct Ov96RowerScore {
    Sprite *digit0;  // 0x00
    Sprite *digit1;  // 0x04
    void *unk08;     // 0x08
    u16 unk0C;       // 0x0C
    u16 unk0E;       // 0x0E
} Ov96RowerScore; // size = 0x10

typedef struct Ov96RowerCount {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Ov96RowerCount; // size = 0xC

typedef struct Ov96Obstacle {
    s32 unk0;     // 0x0
    void *unk4;   // 0x4
    u16 unk8;     // 0x8
    u8 unkA;      // 0xA
    u8 unkB;      // 0xB
} Ov96Obstacle; // size = 0xC

typedef struct Ov96RowerWork {
    BgConfig *bgConfig;                       // 0x000
    Window window;                            // 0x004
    enum HeapID heapID;                       // 0x014
    void *unk18;                              // 0x018
    void *unk1C;                              // 0x01C
    Ov96RowerScore score;                     // 0x020
    Ov96Rower rowers[4];                      // 0x030
    Ov96RowerCount counts[4];                 // 0x380
    u32 unk3B0;                               // 0x3B0
    u8 unk3B4[0xC];                           // 0x3B4
    u8 unk3C0;                                // 0x3C0
    u8 unk3C1;                                // 0x3C1
    u8 unk3C2;                                // 0x3C2
    u8 unk3C3;                                // 0x3C3
    struct UnkStruct_ov96_021E9A78 *unk3C4; // 0x3C4
    struct Ov96SpriteSet *unk3C8;           // 0x3C8
    void *unk3CC[4];                          // 0x3CC
    void *unk3DC;                             // 0x3DC
    Ov96MonIconWork *unk3E0;                  // 0x3E0
    u32 unk3E4[30];                           // 0x3E4
    u32 unk45C[30];                           // 0x45C
    Ov96Obstacle obstacles[30];               // 0x4D4
    u8 unk63C;                                // 0x63C
    u8 unk63D;                                // 0x63D
    u16 unk63E;                               // 0x63E
    u16 unk640;                               // 0x640
    u16 unk642;                               // 0x642
    u8 unk644[4];                             // 0x644
} Ov96RowerWork; // size = 0x648

// The DS caller passes the course position as u8 (x / 4096, cut to a byte).
void ov96_021FFB7C(Ov96Rower *rower, u8 pos, s32 flag);

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_ROWER_WORK_H
