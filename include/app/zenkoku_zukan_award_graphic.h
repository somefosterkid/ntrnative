// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zenkoku_zukan_award_graphic.h: the lines
// in use so far.
#ifndef BWPORT_APP_ZENKOKU_ZUKAN_AWARD_GRAPHIC_H
#define BWPORT_APP_ZENKOKU_ZUKAN_AWARD_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"

typedef struct ZenkokuZukanAwardGraphic ZenkokuZukanAwardGraphic;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZenkokuZukanAwardGraphic_Free
void func_ov222_021b9fb8(ZenkokuZukanAwardGraphic *graphic);
// pokebw2's ZenkokuZukanAwardGraphic_Update
void func_ov222_021ba014(ZenkokuZukanAwardGraphic *graphic);
// pokebw2's ZenkokuZukanAwardGraphic_GetClActUnit
ClActUnit *func_ov222_021ba028(ZenkokuZukanAwardGraphic *graphic);
// pokebw2's ZenkokuZukanAwardGraphic_Create
ZenkokuZukanAwardGraphic *func_ov222_021b9f10(u32 layout, HeapID heapId);

#endif // BWPORT_APP_ZENKOKU_ZUKAN_AWARD_GRAPHIC_H
