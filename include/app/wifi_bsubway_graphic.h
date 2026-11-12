// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifi_bsubway_graphic.h: the lines in use
// so far.
#ifndef BWPORT_APP_WIFI_BSUBWAY_GRAPHIC_H
#define BWPORT_APP_WIFI_BSUBWAY_GRAPHIC_H

#include "types.h"
#include "gfl/heap.h"

typedef struct WifiBSubwayGraphic WifiBSubwayGraphic;

// pokebw2's WifiBSubwayGraphic_Create
WifiBSubwayGraphic *func_ov116_021d821c(u32 layout, HeapID heapId);
// pokebw2's WifiBSubwayGraphic_Free
void func_ov116_021d82ac(WifiBSubwayGraphic *graphic);

#endif // BWPORT_APP_WIFI_BSUBWAY_GRAPHIC_H
