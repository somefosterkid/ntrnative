#ifndef POKEHEARTGOLD_OVERLAY_96_OV96HURDLE_WORK_H
#define POKEHEARTGOLD_OVERLAY_96_OV96HURDLE_WORK_H

#include "constants/heap.h"

#include "bg_window.h"

struct NNSG2dCharacterData;
struct Ov96HurdleCpu;
struct Ov96HurdleHud;

// Work area of the Pokeathlon hurdle event (ov96_022047EC allocates it, 0x6c0 bytes).

typedef struct Ov96HurdleRunner {
    void *unk_00[18];
    void *unk_48;
    void *unk_4C[2];
    void *unk_54;
    VecFx32 pos; // 0x58
    VecFx32 vel; // 0x64
    s16 unk_70;
    u8 unk_72[2];
    u32 dir; // 0x74
    u8 unk_78[0x20];
    u32 unk_98;
    u8 unk_9C;
    u8 unk_9D;
    u8 unk_9E;
    u8 unk_9F;
    u8 unk_A0[2];
    u8 unk_A2;
    u8 unk_A3;
    u8 active; // 0xA4
    u8 unk_A5;
    u8 unk_A6;
    u8 unk_A7;
    u8 unk_A8;
    u8 unk_A9;
    u8 unk_AA;
    u8 unk_AB;
    u16 unk_AC;
    u8 unk_AE[2];
    u8 unk_B0;
    u8 unk_B1;
    u8 unk_B2[6];
} Ov96HurdleRunner; // 0xB8

typedef struct Ov96HurdleCell {
    s32 active;
    void *sprite;
    u16 x;
    u16 y;
} Ov96HurdleCell; // 0xC

typedef struct Ov96HurdleCellPair {
    s32 active;
    void *sprites[2];
    u16 x;
    u16 y;
} Ov96HurdleCellPair; // 0x10

typedef struct Ov96HurdleSlot {
    u32 unk_0;
    void *unk_4;
    void *unk_8;
    s32 value;
    u32 unk_10;
} Ov96HurdleSlot; // 0x14

typedef struct Ov96HurdlePoint {
    u16 x;
    u16 y;
} Ov96HurdlePoint; // 0x4

typedef struct Ov96HurdleWork {
    BgConfig *bgConfig; // 0x00
    Window window;      // 0x04
    enum HeapID heapId; // 0x14
    void *unk_18;
    struct NNSG2dCharacterData *unk_1C;
    void *unk_20;
    Ov96HurdleRunner runners[4]; // 0x24
    u8 unk_304[0x40];
    void *unk_344;
    void *unk_348;
    void *unk_34C[4];
    void *unk_35C[4];
    struct Ov96HurdleCpu *cpu; // 0x36C ov96_02208AA8
    struct Ov96HurdleHud *hud; // 0x370 ov96_02207CCC
    u32 unk_374;
    u32 unk_378;
    Ov96HurdleCell cells[20];      // 0x37C
    Ov96HurdleCellPair pairs[10];  // 0x46C
    u16 unk_50C;
    u8 unk_50E;
    u8 unk_50F;
    u8 unk_510;
    u8 unk_511;
    u8 unk_512;
    u8 unk_513[5];
    u8 unk_518;
    u8 unk_519;
    u8 unk_51A;
    u8 unk_51B;
    u16 unk_51C;
    u16 unk_51E;
    u16 unk_520;
    u16 unk_522;
    u8 unk_524[0x40];
    Ov96HurdleSlot slots[5];        // 0x564
    Ov96HurdlePoint points_5C8[20]; // 0x5C8
    u16 unk_618;
    u16 unk_61A;
    Ov96HurdlePoint points_61C[20]; // 0x61C
    Ov96HurdlePoint points_66C[10]; // 0x66C
    u8 unk_694[0x28];
    u8 unk_6BC[4];
} Ov96HurdleWork; // 0x6C0

#endif // POKEHEARTGOLD_OVERLAY_96_OV96HURDLE_WORK_H
