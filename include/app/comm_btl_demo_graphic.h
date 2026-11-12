// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_btl_demo_graphic.h: the lines in
// use so far.
#ifndef BWPORT_APP_COMM_BTL_DEMO_GRAPHIC_H
#define BWPORT_APP_COMM_BTL_DEMO_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

typedef struct CommBtlDemoGraphic CommBtlDemoGraphic;

// pokebw2's CommBtlDemoGraphic_Create
CommBtlDemoGraphic *func_ov213_021bab64(u32 layout, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CommBtlDemoGraphic_Free
void func_ov213_021bac0c(CommBtlDemoGraphic *graphic);
// pokebw2's CommBtlDemoGraphic_Update
void func_ov213_021bac68(CommBtlDemoGraphic *graphic);
// pokebw2's CommBtlDemoGraphic_Begin3D
// Around the frame's 3D drawing
void func_ov213_021bac7c(CommBtlDemoGraphic *graphic);
// pokebw2's CommBtlDemoGraphic_End3D
void func_ov213_021bac88(CommBtlDemoGraphic *graphic);
// pokebw2's CommBtlDemoGraphic_GetClActUnit
ClActUnit *func_ov213_021bac94(CommBtlDemoGraphic *graphic);

#endif // BWPORT_APP_COMM_BTL_DEMO_GRAPHIC_H
