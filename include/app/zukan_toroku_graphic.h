// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukan_toroku_graphic.h: the lines in use
// so far.
#ifndef BWPORT_APP_ZUKAN_TOROKU_GRAPHIC_H
#define BWPORT_APP_ZUKAN_TOROKU_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

typedef struct ZukanTorokuGraphic ZukanTorokuGraphic;

// pokebw2's ZukanTorokuGraphic_Create
ZukanTorokuGraphic *func_ov207_0220b318(u32 layout, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZukanTorokuGraphic_GetClActUnit
ClActUnit *func_ov207_0220b428(ZukanTorokuGraphic *graphic);
// pokebw2's ZukanTorokuGraphic_Free
void func_ov207_0220b3b8(ZukanTorokuGraphic *graphic);
// pokebw2's ZukanTorokuGraphic_Update
void func_ov207_0220b40c(ZukanTorokuGraphic *graphic);
// pokebw2's ZukanTorokuGraphic_Begin3D
void func_ov207_0220b420(ZukanTorokuGraphic *graphic);
// pokebw2's ZukanTorokuGraphic_End3D
void func_ov207_0220b424(ZukanTorokuGraphic *graphic);

#endif // BWPORT_APP_ZUKAN_TOROKU_GRAPHIC_H
