#ifndef POKEHEARTGOLD_OVERLAY_99_POKEATHLON_COURSE_RECORD_H
#define POKEHEARTGOLD_OVERLAY_99_POKEATHLON_COURSE_RECORD_H

// PokeathlonCourseRecord: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/overlay_99_021E5900.c) and 87 accesses; fields at every accessed offset, pads elsewhere.

#include "constants/heap.h"

struct BgConfig;
struct ManagedSprite;
struct NARC;
struct PokeathlonCourseRecordArgs;
struct PokeathlonSpriteContext;
struct Ov98TextSys;

typedef struct PokeathlonCourseRecord {
    struct PokeathlonCourseRecordArgs *args;
    struct BgConfig *bgConfig;
    struct NARC *narc;
    enum HeapID heapId;
    struct Ov98TextSys *textContext;
    struct PokeathlonSpriteContext *spriteContext;
    struct ManagedSprite *sprites[26];
    struct { s8 cursor; u8 unk_4_pad[0x3]; BOOL scrollForward; int page; BOOL scrolling; BOOL pageChanged; } selection;
} PokeathlonCourseRecord;

// sizeof(PokeathlonCourseRecord) == 0x94

#endif // POKEHEARTGOLD_OVERLAY_99_POKEATHLON_COURSE_RECORD_H
