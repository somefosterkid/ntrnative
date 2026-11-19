#ifndef GUARD_POKEHEARTGOLD_UNK_0201956C_TYPES_H
#define GUARD_POKEHEARTGOLD_UNK_0201956C_TYPES_H

#include "bg_window.h"

// One BG tilemap frame (a window frame image copied to a BG screen).
typedef struct BGWinFrameEntry {
    u16 *screen;
    u16 width : 6;
    u16 height : 6;
    u16 hidden : 1;
    u16 moving : 3;
    s8 x;
    s8 y;
    s8 moveX;
    s8 moveY;
    u8 bg;
    u8 moveCount;
    s8 left;
    s8 right;
    s8 top;
    s8 bottom;
} BGWinFrameEntry; // size 0x10 on the DS (pointer + 12 bytes)

typedef struct UnkStruct_0201956C {
    BgConfig *bgConfig;
    BGWinFrameEntry *entries;
    u16 count;
    u16 transferMode : 15;
    u16 moving : 1;
    enum HeapID heapId;
} UnkStruct_0201956C; // size 0x10 on the DS

#endif // GUARD_POKEHEARTGOLD_UNK_0201956C_TYPES_H
