#ifndef POKEHEARTGOLD_OVERLAY_99_POKEATHLON_EVENT_RECORD_H
#define POKEHEARTGOLD_OVERLAY_99_POKEATHLON_EVENT_RECORD_H

// PokeathlonEventRecord: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_99_021E7150_021E83F4_021E83F4.c) and 145 accesses; fields at every accessed offset, pads elsewhere.

#include "constants/heap.h"

struct BgConfig;
struct ManagedSprite;
struct NARC;
struct PokeathlonEventRecordData;
struct PokeathlonSpriteContext;
struct Ov98TextSys;

typedef struct PokeathlonEventRecord {
    struct PokeathlonEventRecordData *data;
    struct BgConfig *bgConfig;
    struct NARC *narc;
    enum HeapID heapId;
    struct Ov98TextSys *textContext;
    struct PokeathlonSpriteContext *spriteContext;
    struct ManagedSprite *sprites[37];
    s8 cursor;
    s8 lastColumn;
    u8 pad_AE[0x2];
    BOOL linkMode;
    BOOL modeSwitchRequested;
    BOOL eventChanged;
    int noRecord[10];
} PokeathlonEventRecord;

// sizeof(PokeathlonEventRecord) == 0xe4

#endif // POKEHEARTGOLD_OVERLAY_99_POKEATHLON_EVENT_RECORD_H
