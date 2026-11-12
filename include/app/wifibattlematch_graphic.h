// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifibattlematch_graphic.h: the lines in
// use so far.
#ifndef BWPORT_APP_WIFIBATTLEMATCH_GRAPHIC_H
#define BWPORT_APP_WIFIBATTLEMATCH_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

typedef struct WifiBattleMatchGraphic WifiBattleMatchGraphic;

// pokebw2's WifiBattleMatchGraphic_GetClActUnit
ClActUnit *func_ov177_021e5548(WifiBattleMatchGraphic *graphic);

// pokebw2's WifiBattleMatchGraphic_Free
void func_ov177_021e54e0(WifiBattleMatchGraphic *graphic);

// pokebw2's WifiBattleMatchGraphic_Update
void func_ov177_021e5534(WifiBattleMatchGraphic *graphic);

// pokebw2's WifiBattleMatchGraphic_Create
WifiBattleMatchGraphic *func_ov177_021e5440(u32 layout, HeapID heapId);

#endif // BWPORT_APP_WIFIBATTLEMATCH_GRAPHIC_H
