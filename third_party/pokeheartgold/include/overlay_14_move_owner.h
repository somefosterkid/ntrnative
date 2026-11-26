#ifndef POKEHEARTGOLD_OVERLAY_14_MOVE_OWNER_H
#define POKEHEARTGOLD_OVERLAY_14_MOVE_OWNER_H

#include "pokemon_types_def.h"

struct UnkStruct_ov14_boxsys;

// MoveSet: what UnkStruct_ov14_boxsys.unk_C points to while the box app moves a Pokemon/item between slots
// (6 party + 1 held entries, 0x20 each; unk_E4/unk_E8 = the source and target slot, 0xFF = none).
typedef struct MoveEntry {
    Pokemon *mon;
    s32 idx;
    s32 arg;
    s32 active;
    u32 mulX;
    u32 mulY;
    s16 dx;
    s16 dy;
    s16 x0;
    s16 y0;
} MoveEntry;

typedef struct MoveSet {
    MoveEntry e[7];
    s32 frame;
    u32 from;
    u32 to;
    s32 unk_EC; // never written by ov14_021F08BC (0xF0 alloc); ov14_021ECDA8 compares it with the cursor slot
} MoveSet;
#ifdef __MWERKS__
LAYOUT_CHECK(MoveSet_size, sizeof(MoveSet) == 0xF0);
#endif

#endif // POKEHEARTGOLD_OVERLAY_14_MOVE_OWNER_H
