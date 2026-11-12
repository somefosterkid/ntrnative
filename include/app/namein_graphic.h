// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/namein_graphic.h: the lines in use so
// far.
#ifndef BWPORT_APP_NAMEIN_GRAPHIC_H
#define BWPORT_APP_NAMEIN_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

typedef struct NameinGraphic NameinGraphic;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's NameinGraphic_Create
// Sets up the display layout, the BGs and the cell actor system, and adds a VBlank task
NameinGraphic *func_ov194_021f4ae4(u32 layout, HeapID heapId);
// pokebw2's NameinGraphic_Delete
void func_ov194_021f4b84(NameinGraphic *graphic);
// pokebw2's NameinGraphic_Main
// Called every frame
void func_ov194_021f4bd8(NameinGraphic *graphic);
// pokebw2's NameinGraphic_GetClActUnit
ClActUnit *func_ov194_021f4bec(NameinGraphic *graphic);

#endif // BWPORT_APP_NAMEIN_GRAPHIC_H
