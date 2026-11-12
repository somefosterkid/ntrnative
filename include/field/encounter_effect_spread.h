// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/encounter_effect_spread.h, the lines
// in use so far.
#ifndef BWPORT_FIELD_ENCOUNTER_EFFECT_SPREAD_H
#define BWPORT_FIELD_ENCOUNTER_EFFECT_SPREAD_H

// Overlay 81: the encounter effect whose cells fly off in waves that spread out from three cells. The file's name is
// a guess; the ROM has no string for it

#include "types.h"
#include "field/encounter_effect.h"
#include "struct_decls.h"

// The grid's work from 0x300c on. It runs 0x114 bytes past the end of the grid that overlay 77 allocates, over its
// update function and mode if the wave got long enough, and over whatever follows it on the heap
typedef struct EncEffSpread {
    // The cells shown in the last wave
    u16 wave[96];
    // The cells to show in the next wave
    u16 next[96];
    u16 waveCount;
    u16 nextCount;
} EncEffSpread;

GameEvent *func_ov081_021f5a80(GameSystem *gsys, Field *field);

#endif // BWPORT_FIELD_ENCOUNTER_EFFECT_SPREAD_H
