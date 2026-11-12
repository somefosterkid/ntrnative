// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/un_select_graphic.h: the lines in use so
// far.
#ifndef BWPORT_APP_UN_SELECT_GRAPHIC_H
#define BWPORT_APP_UN_SELECT_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

typedef struct UnSelectGraphic UnSelectGraphic;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's UnSelectGraphic_Free
void func_ov218_021ba8f0(UnSelectGraphic *graphic);
// pokebw2's UnSelectGraphic_Update
// Called every frame
void func_ov218_021ba94c(UnSelectGraphic *graphic);
// pokebw2's UnSelectGraphic_Create
UnSelectGraphic *func_ov218_021ba848(u32 layout, HeapID heapId);
// pokebw2's UnSelectGraphic_GetClActUnit
ClActUnit *func_ov218_021ba960(UnSelectGraphic *graphic);

#endif // BWPORT_APP_UN_SELECT_GRAPHIC_H
