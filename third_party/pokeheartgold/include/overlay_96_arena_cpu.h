#ifndef POKEHEARTGOLD_OVERLAY_96_ARENA_CPU_H
#define POKEHEARTGOLD_OVERLAY_96_ARENA_CPU_H

// Ov96ArenaCpu: the 0x4ec CPU brain of the one-screen arena course (Ov96ArenaWork.unk_770, ov96_021F3390).
// The arena is a 7x5 grid of 32-pixel cells (cell = x + 7 * y, ov96_021F38FC). Every tick (ov96_021F342C) one
// CPU-controlled Pokemon picks a new target: ov96_021F350C scores the 5x5 cells around it (ov96_021F3888)
// and writes the chosen pixel into its target vector. Offsets are the DS ones.

#include "global.h"
#include "constants/heap.h"

struct Ov96SpriteSetElem;

// one Pokemon (0x20): Ov96ArenaCpu.mons[player * 3 + slot], filled by ov96_021F33E0 from an Ov96ArenaMon
typedef struct Ov96ArenaCpuMon {
    s32 cell;                      // 0x00 grid cell under the mon, -1 = off the grid
    s32 unk_04;                    // 0x04 Ov96ArenaMon.unk_8A
    u16 *unk_08;                   // 0x08 &Ov96ArenaMon.unk_8E
    VecFx32 *pos;                  // 0x0C &Ov96ArenaMon.pos
    s32 *state;                    // 0x10 &Ov96ArenaMon.state
    s32 idx;                       // 0x14 own index in mons[]
    VecFx32 *target;               // 0x18 &Ov96ArenaMon.unk_1C, NULL for human players
    struct Ov96SpriteSetElem *spr; // 0x1C Ov96ArenaMon.spr
} Ov96ArenaCpuMon;

// one candidate cell (0x20): Ov96ArenaCpu.cells[25], the 5x5 window around the mon being steered
typedef struct Ov96ArenaCpuCell {
    s32 x;     // 0x00 grid column
    s32 y;     // 0x04 grid row
    s32 px;    // 0x08 pixel x (ov96_021F380C)
    s32 py;    // 0x0C pixel y
    BOOL valid; // 0x10 inside the 7x5 grid
    s32 cell;  // 0x14 x + 7 * y, -1 when invalid
    s32 score; // 0x18 ov96_021F3930, the sort key of ov96_021F3B38
    s32 idx;   // 0x1C own index in cells[]
} Ov96ArenaCpuCell;

// one item on the field (8 bytes), the stack array ov96_021F333C fills: radius 0 = no item
typedef struct Ov96ArenaCpuItem {
    s32 radius; // 0x0
    u16 x;      // 0x4 pixels
    u16 y;      // 0x6
} Ov96ArenaCpuItem;

typedef struct Ov96ArenaCpu {
    Ov96ArenaCpuMon mons[12];   // 0x000
    Ov96ArenaCpuCell cells[25]; // 0x180
    s32 itemCells[4];           // 0x4A0 grid cell of each item, -1 = none
    BOOL active;                // 0x4B0 a round of target picks is running
    s32 unk_4B4[12];            // 0x4B4 per-player counters (ov96_021F350C indexes them by unk_4E8)
    u8 count;                   // 0x4E4 CPU players
    u8 unk_4E5;                 // 0x4E5 CPU player of the round
    u8 timer;                   // 0x4E6
    u8 interval;                // 0x4E7 ticks between rounds
    u8 unk_4E8;                 // 0x4E8 slot (0..2) of the round
    u8 unk_4E9;                 // 0x4E9 human players: the CPU players follow them
    u8 unk_4EA;                 // 0x4EA
    u8 unk_4EB;
} Ov96ArenaCpu; // size = 0x4EC

Ov96ArenaCpu *ov96_021F3390(enum HeapID heapId, u8 count, BOOL flag);
void ov96_021F33E0(Ov96ArenaCpu *cpu, s32 idx, s32 arg2, u16 *arg3, VecFx32 *pos, s32 *state, VecFx32 *target, struct Ov96SpriteSetElem *spr);
void ov96_021F3424(Ov96ArenaCpu *cpu);
u8 ov96_021F342C(Ov96ArenaCpu *cpu, s32 arg1);
void ov96_021F350C(Ov96ArenaCpu *cpu, u8 idx, s32 arg2);
s32 ov96_021F333C(s32 table, Ov96ArenaCpuItem *items);
void ov96_021F380C(u8 n, Ov96ArenaCpuCell **cells);
void ov96_021F3888(Ov96ArenaCpu *cpu, u8 n, Ov96ArenaCpuItem *items, Ov96ArenaCpuMon *mon, Ov96ArenaCpuCell **cells);
s32 ov96_021F38FC(s32 x, s32 y);
s32 ov96_021F3930(Ov96ArenaCpu *cpu, Ov96ArenaCpuItem *items, s32 cell, Ov96ArenaCpuMon *mon, s32 px, s32 py, BOOL *onItem);
s32 ov96_021F39F0(Ov96ArenaCpu *cpu, Ov96ArenaCpuMon *mon, s32 cell);
s32 ov96_021F3A5C(Ov96ArenaCpuItem *items, s32 x, s32 y);
BOOL ov96_021F3AB0(Ov96ArenaCpu *cpu, s32 cell);
void ov96_021F3AD8(Ov96ArenaCpu *cpu, Ov96ArenaCpuItem *items, s32 cell, s32 *px, s32 *py);
u8 ov96_021F3B38(u32 n, Ov96ArenaCpuCell **src, Ov96ArenaCpuCell **out);

#endif // POKEHEARTGOLD_OVERLAY_96_ARENA_CPU_H
