// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/demo/the_end.h: the lines in use so far.
#ifndef BWPORT_DEMO_THE_END_H
#define BWPORT_DEMO_THE_END_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"

typedef struct TheEndGraphic TheEndGraphic;

// pokebw2's TheEndGraphic_Create
// the_end_graphic.c
TheEndGraphic *func_ov182_021b98e4(u32 layout, HeapID heapId);
// pokebw2's TheEndGraphic_Free
void func_ov182_021b998c(TheEndGraphic *graphic);

// Black 2's, not checked against Black
#define OVERLAY_THE_END OVERLAY_ID(267)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's TheEndGraphic_Update
void func_ov182_021b99e8(TheEndGraphic *graphic);

#endif // BWPORT_DEMO_THE_END_H
