#ifndef POKEHEARTGOLD_OVERLAY_96_HURDLE_CPU_H
#define POKEHEARTGOLD_OVERLAY_96_HURDLE_CPU_H

// Ov96HurdleCpu: the 0x55c CPU brain of the Pokeathlon hurdle event (Ov96HurdleWork.cpu, ov96_02208AA8).
// Every few ticks (ov96_02208B34) one CPU runner picks a new target: ov96_02208BB8 scores 48 cells (8 x 6,
// 32 pixels apart) around it against the hurdles, the other runners and the slots, sorts them
// (ov96_02208FF0) and steers the runner to a random best one (ov96_02208FB8). Offsets are the DS ones.

#include "global.h"
#include "constants/heap.h"

struct Ov96HurdleRunner;
struct Ov96HurdleSlot;

// one runner (8 bytes): Ov96HurdleCpu.runners[4], ov96_02208AE8; x / y are only filled in ov96_02208BB8's stack copy
typedef struct Ov96HurdleCpuRunner {
    struct Ov96HurdleRunner *runner; // 0x0
    s16 x;                           // 0x4
    s16 y;                           // 0x6
} Ov96HurdleCpuRunner;

// one hurdle (0xC): ov96_02208AF8 fills it from a (x / 8, y / 8) byte pair, (0, 0) = none
typedef struct Ov96HurdleCpuHurdle {
    BOOL active; // 0x0
    s32 x;       // 0x4 pixels
    s32 y;       // 0x8
} Ov96HurdleCpuHurdle;

// one candidate cell (0x14): Ov96HurdleCpu.cells[48]
typedef struct Ov96HurdleCpuCell {
    s32 x;     // 0x00 cell origin
    s32 y;     // 0x04
    s32 px;    // 0x08 random pixel inside the cell
    s32 py;    // 0x0C
    s16 score; // 0x10 the sort key of ov96_02208FF0
    s16 idx;   // 0x12 own index in cells[]
} Ov96HurdleCpuCell;

typedef struct Ov96HurdleCpu {
    Ov96HurdleCpuRunner runners[4];    // 0x000
    Ov96HurdleCpuHurdle hurdles[20];   // 0x020
    Ov96HurdleCpuHurdle hurdles2[10];  // 0x110 read by ov96_02208BB8, never filled
    struct Ov96HurdleSlot *slots;      // 0x188 Ov96HurdleWork.slots (5)
    u8 unk_18C[4];
    Ov96HurdleCpuCell cells[48];       // 0x190
    BOOL active;                       // 0x550 a round of target picks is running
    u8 count;                          // 0x554 CPU runners
    u8 unk_555;                        // 0x555 CPU runner of the round
    u8 timer;                          // 0x556
    u8 interval;                       // 0x557 ticks between rounds
    u8 unk_558;                        // 0x558 first CPU runner (= human runners)
    u8 unk_559;                        // 0x559 human runners
    u8 unk_55A[2];
} Ov96HurdleCpu; // size = 0x55C

Ov96HurdleCpu *ov96_02208AA8(enum HeapID heapId, s8 count);
void ov96_02208AE8(Ov96HurdleCpu *cpu, s32 idx, struct Ov96HurdleRunner *runner);
void ov96_02208AF0(Ov96HurdleCpu *cpu, struct Ov96HurdleSlot *slots);
void ov96_02208AF8(Ov96HurdleCpu *cpu, const void *data);
void ov96_02208B2C(Ov96HurdleCpu *cpu);
u8 ov96_02208B34(Ov96HurdleCpu *cpu);
void ov96_02208BB8(Ov96HurdleCpu *cpu, u8 idx);
Ov96HurdleCpuCell *ov96_02208FB8(Ov96HurdleCpu *cpu);
u8 ov96_02208FF0(Ov96HurdleCpuCell **src, Ov96HurdleCpuCell **dst);
BOOL ov96_022090A8(Ov96HurdleCpuCell *cell, u16 x, u16 y);
BOOL ov96_022090D8(Ov96HurdleCpuCell *cell);

#endif // POKEHEARTGOLD_OVERLAY_96_HURDLE_CPU_H
