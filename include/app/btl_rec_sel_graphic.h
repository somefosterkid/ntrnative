// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/btl_rec_sel_graphic.h: the lines in use
// so far.
#ifndef BWPORT_APP_BTL_REC_SEL_GRAPHIC_H
#define BWPORT_APP_BTL_REC_SEL_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

typedef struct BtlRecSelGraphic BtlRecSelGraphic;

// pokebw2's BtlRecSelGraphic_Create
BtlRecSelGraphic *func_ov214_021bb178(u32 layout, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlRecSelGraphic_Free
void func_ov214_021bb220(BtlRecSelGraphic *graphic);
// pokebw2's BtlRecSelGraphic_Update
void func_ov214_021bb27c(BtlRecSelGraphic *graphic);
// pokebw2's BtlRecSelGraphic_Begin3D
// Around the frame's 3D drawing
void func_ov214_021bb290(BtlRecSelGraphic *graphic);
// pokebw2's BtlRecSelGraphic_End3D
void func_ov214_021bb29c(BtlRecSelGraphic *graphic);
// pokebw2's BtlRecSelGraphic_GetClActUnit
ClActUnit *func_ov214_021bb2a8(BtlRecSelGraphic *graphic);

#endif // BWPORT_APP_BTL_REC_SEL_GRAPHIC_H
