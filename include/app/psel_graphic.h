// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/psel_graphic.h: the lines in use so far.
#ifndef BWPORT_APP_PSEL_GRAPHIC_H
#define BWPORT_APP_PSEL_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"

typedef struct PselGraphic PselGraphic;


// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PselGraphic_Create
PselGraphic *func_ov223_021bc3f0(u32 layout, HeapID heapId);
// pokebw2's PselGraphic_Free
void func_ov223_021bc498(PselGraphic *graphic);
// pokebw2's PselGraphic_Update
void func_ov223_021bc4f4(PselGraphic *graphic);
// pokebw2's PselGraphic_Begin3D
// Around the frame's 3D drawing
void func_ov223_021bc508(PselGraphic *graphic);
// pokebw2's PselGraphic_End3D
void func_ov223_021bc514(PselGraphic *graphic);
// pokebw2's PselGraphic_GetClActUnit
ClActUnit *func_ov223_021bc520(PselGraphic *graphic);
// pokebw2's PselGraphic_GetCamera
G3DCamera *func_ov223_021bc52c(PselGraphic *graphic);

#endif // BWPORT_APP_PSEL_GRAPHIC_H
