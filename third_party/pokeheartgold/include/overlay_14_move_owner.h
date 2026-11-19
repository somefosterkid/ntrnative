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
} MoveSet;

#endif // POKEHEARTGOLD_OVERLAY_14_MOVE_OWNER_H
