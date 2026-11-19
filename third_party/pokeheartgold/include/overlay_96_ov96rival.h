#ifndef POKEHEARTGOLD_OVERLAY_96_OV96RIVAL_H
#define POKEHEARTGOLD_OVERLAY_96_OV96RIVAL_H

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

// One computer-controlled rival of a Pokeathlon course (0x24 bytes).
typedef struct Ov96Rival {
    PokeathlonCourseData *course; // 0x00
    u32 stat[4];                  // 0x04: bits 0-8 value, 18-25 key; [3] holds the stamina (starts at 100)
    u32 unk_14;                   // 0x14: bit field, bits 30-31 select stat[]
    u32 unk_18;                   // 0x18: bit field
    u32 unk_1C;                   // 0x1C: bit field
    u32 unk_20;                   // 0x20
} Ov96Rival;

// The rivals of one course (0x78 bytes, allocated by ov96_0220C93C).
typedef struct Ov96RivalSet {
    enum HeapID heapID;           // 0x00
    PokeathlonCourseData *course; // 0x04
    Ov96Rival rivals[3];          // 0x08
    u32 count;                    // 0x74
} Ov96RivalSet;

#endif // POKEHEARTGOLD_OVERLAY_96_OV96RIVAL_H
