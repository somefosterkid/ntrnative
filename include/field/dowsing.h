// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/dowsing.h: the lines in use so far.
#ifndef BWPORT_FIELD_DOWSING_H
#define BWPORT_FIELD_DOWSING_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef struct DowsingWork DowsingWork;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Dowsing_Create
DowsingWork *func_ov033_021f000c(HeapID heapId, HeapID subscreenHeapId, FieldSubscreen *subscreen, Field *field);
// pokebw2's Dowsing_Free
void func_ov033_021f0124(DowsingWork *work);
// pokebw2's Dowsing_Update
// Both take whether the field lets the screen act, which is not while an event runs
void func_ov033_021f0174(DowsingWork *work, BOOL active);
// pokebw2's Dowsing_UpdateAltFrame
void func_ov033_021f05a4(DowsingWork *work, BOOL active);

#endif // BWPORT_FIELD_DOWSING_H
