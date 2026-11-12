// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_player_nogrid.h: the lines in
// use so far.
#ifndef BWPORT_FIELD_FIELD_PLAYER_NOGRID_H
#define BWPORT_FIELD_FIELD_PLAYER_NOGRID_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"

// struct FieldPlayerNoGrid is field_player_nogrid.c's own (as in pokebw2)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov021_021a5920(FieldPlayerNoGrid *nogrid);
void func_ov021_021a5a34(FieldPlayerNoGrid *nogrid);
BOOL func_ov021_021a59dc(FieldPlayerNoGrid *nogrid);
BOOL func_ov021_021a59ec(FieldPlayerNoGrid *nogrid);
FieldPlayerNoGrid *func_ov021_021a5880(FieldPlayerCore *core, HeapID heapId);
void func_ov021_021a5a0c(FieldPlayerNoGrid *nogrid, RailPosition *pos);
void func_ov021_021a5928(FieldPlayerNoGrid *nogrid, RailPosition *pos);
void func_ov021_021a5a18(FieldPlayerNoGrid *nogrid, u32 dir, RailPosition *pos);
void func_ov021_021a5a24(FieldPlayerNoGrid *nogrid, VecFx32 *pos);
void func_ov021_021a596c(FieldPlayerNoGrid *nogrid, u32 pressedKeys, u32 heldKeys);
void func_ov021_021a5964(FieldPlayerNoGrid *nogrid);
void func_ov021_021a5a00(FieldPlayerNoGrid *nogrid, const RailPosition *pos);
RailUnit *func_ov021_021a5a30(FieldPlayerNoGrid *nogrid);

#endif // BWPORT_FIELD_FIELD_PLAYER_NOGRID_H
