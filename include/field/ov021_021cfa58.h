#ifndef FIELD_OV021_021CFA58_H
#define FIELD_OV021_021CFA58_H

#include "types.h"
#include "struct_decls.h"

// A place of ov021_021cfa58.c's table: a grid position and a u16 that func_ov021_021cfac0 compares
typedef struct Ov021DataRec {
    s16 unk00; // x
    s16 unk02; // y
    s16 unk04; // z
    u16 unk06;
} Ov021DataRec;

// The zone of each place, and the places; ov018's intrude_field.c reads both
extern const u16 data_ov021_021db810[18];
extern const Ov021DataRec data_ov021_021db834[18];

// The index of the save's entry at the place (zone, grid x, y, z; y within one), or 0xff
u32 func_ov021_021cfac0(HighLinkSave *save, u16 zone, s16 x, s16 y, s16 z);
// The place of the save's next entry from *index on (advancing *index past it), or NULL
const Ov021DataRec *func_ov021_021cfb68(HighLinkSave *save, s32 *index);
GameEvent *func_ov021_021cfc14(GameSystem *gsys, u32 heapId, s32 index);

#endif // FIELD_OV021_021CFA58_H
