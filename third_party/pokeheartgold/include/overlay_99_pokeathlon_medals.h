#ifndef POKEHEARTGOLD_OVERLAY_99_POKEATHLON_MEDALS_H
#define POKEHEARTGOLD_OVERLAY_99_POKEATHLON_MEDALS_H

// PokeathlonMedals: the Pokeathlon medals app's data (OverlayManager_CreateAndGetData(man, 0x4D0, HEAP_ID_132)
// in PokeathlonMedals_Init), moved here from src/overlay_99_021E7150_021E794C_021E794C.c so that every function of
// the app (ov99_021E7158 .. ov99_021E8224) can name it. The m2c-era functions reach 0x3F4 / 0x3F8 as whole words:
// the bit-field records are one leaf each (tools/hg/lift_typed.py: a record of bit-fields only is a leaf).

#include "constants/heap.h"

struct BgConfig;
struct GridInputHandler;
struct ManagedSprite;
struct NARC;
struct PokeathlonMedalsArgs;
struct PokeathlonSpriteContext;
struct Ov98TextSys;

typedef struct PokeathlonMedalsFlags {
    u32 maxPage : 5;
    u32 speciesCount : 9;
    u32 page : 5;
    u32 cursor : 8;
    u32 scrollBackward : 1;
    u32 scrolling : 1;
    u32 refreshSpecies : 1;
    u32 unused : 2;
} PokeathlonMedalsFlags;

typedef struct PokeathlonMedalsScroll {
    u32 frame : 8;
    u32 unused : 24;
} PokeathlonMedalsScroll;

typedef struct PokeathlonMedals {
    struct BgConfig *bgConfig;                     // 0x000
    struct NARC *narc;                             // 0x004
    struct NARC *zukanNarc;                        // 0x008
    enum HeapID heapId;                            // 0x00C
    struct PokeathlonMedalsArgs *args;             // 0x010
    u16 species[493 + 1];                          // 0x014
    u16 *speciesOrder;                             // 0x3F0
    PokeathlonMedalsFlags flags;                   // 0x3F4
    PokeathlonMedalsScroll scroll;                 // 0x3F8
    struct GridInputHandler *grid;                 // 0x3FC
    struct Ov98TextSys *textContext;     // 0x400
    struct PokeathlonSpriteContext *spriteContext; // 0x404
    struct ManagedSprite *sprites[50];             // 0x408
} PokeathlonMedals; // size 0x4D0

#endif // POKEHEARTGOLD_OVERLAY_99_POKEATHLON_MEDALS_H
