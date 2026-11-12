// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/chihou_zukan_award_graphic.h: the lines
// in use so far.
#ifndef BWPORT_APP_CHIHOU_ZUKAN_AWARD_GRAPHIC_H
#define BWPORT_APP_CHIHOU_ZUKAN_AWARD_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

typedef struct ChihouZukanAwardGraphic ChihouZukanAwardGraphic;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ChihouZukanAwardGraphic_Create
ChihouZukanAwardGraphic *func_ov221_021b9e78(u32 layout, HeapID heapId);
// pokebw2's ChihouZukanAwardGraphic_Free
void func_ov221_021b9f20(ChihouZukanAwardGraphic *graphic);
// pokebw2's ChihouZukanAwardGraphic_Update
void func_ov221_021b9f7c(ChihouZukanAwardGraphic *graphic);
// pokebw2's ChihouZukanAwardGraphic_GetClActUnit
ClActUnit *func_ov221_021b9f90(ChihouZukanAwardGraphic *graphic);

#endif // BWPORT_APP_CHIHOU_ZUKAN_AWARD_GRAPHIC_H
