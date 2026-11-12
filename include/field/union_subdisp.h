// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/union_subdisp.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_UNION_SUBDISP_H
#define BWPORT_FIELD_UNION_SUBDISP_H

#include "types.h"
#include "struct_decls.h"

typedef struct UnionSubdisp UnionSubdisp;

// pokebw2's UnionSubdisp_Free
void func_ov031_021effe8(UnionSubdisp *work);
// pokebw2's UnionSubdisp_Update
// Both take whether the field lets the screen act, which is not while the Union Room system is busy
void func_ov031_021f0054(UnionSubdisp *work, BOOL active);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's UnionSubdisp_Create
UnionSubdisp *func_ov031_021efec0(GameSystem *gsys);
// pokebw2's UnionSubdisp_UpdateAltFrame
void func_ov031_021f00d4(UnionSubdisp *work, BOOL active);

#endif // BWPORT_FIELD_UNION_SUBDISP_H
