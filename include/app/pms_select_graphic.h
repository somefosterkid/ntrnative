// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pms_select_graphic.h: the lines in use
// so far.
#ifndef BWPORT_APP_PMS_SELECT_GRAPHIC_H
#define BWPORT_APP_PMS_SELECT_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

typedef struct PMSSelectGraphic PMSSelectGraphic;

// pokebw2's PMSSelectGraphic_Create
PMSSelectGraphic *func_ov110_021c1610(u32 layout, HeapID heapId);
// pokebw2's PMSSelectGraphic_Delete
void func_ov110_021c16b0(PMSSelectGraphic *graphic);
// pokebw2's PMSSelectGraphic_Main
// Called every frame
void func_ov110_021c1704(PMSSelectGraphic *graphic);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSSelectGraphic_GetClActUnit
ClActUnit *func_ov110_021c1718(PMSSelectGraphic *graphic);

#endif // BWPORT_APP_PMS_SELECT_GRAPHIC_H
