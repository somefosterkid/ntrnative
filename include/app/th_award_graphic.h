// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/th_award_graphic.h: the lines in use so
// far.
#ifndef BWPORT_APP_TH_AWARD_GRAPHIC_H
#define BWPORT_APP_TH_AWARD_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

typedef struct ThAwardGraphic ThAwardGraphic;

// pokebw2's ThAwardGraphic_Create
ThAwardGraphic *func_ov220_021ba0d8(u32 layout, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ThAwardGraphic_Free
void func_ov220_021ba178(ThAwardGraphic *graphic);
// pokebw2's ThAwardGraphic_Update
void func_ov220_021ba1cc(ThAwardGraphic *graphic);
// pokebw2's ThAwardGraphic_GetClActUnit
ClActUnit *func_ov220_021ba1e0(ThAwardGraphic *graphic);

#endif // BWPORT_APP_TH_AWARD_GRAPHIC_H
