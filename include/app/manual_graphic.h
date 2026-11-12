// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/manual_graphic.h: the lines in use so
// far.
#ifndef BWPORT_APP_MANUAL_GRAPHIC_H
#define BWPORT_APP_MANUAL_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

typedef struct ManualGraphic ManualGraphic;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
ManualGraphic *func_ov225_021b9900(u32 layout, HeapID heapId);
void func_ov225_021b99a0(ManualGraphic *graphic);
void func_ov225_021b99f4(ManualGraphic *graphic);

// pokebw2's ManualGraphic_GetClActUnit
ClActUnit *func_ov225_021b9a08(ManualGraphic *graphic);

#endif // BWPORT_APP_MANUAL_GRAPHIC_H
