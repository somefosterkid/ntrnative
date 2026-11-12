// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/demo3d/demo3d_graphic.h: the lines in
// use so far.
#ifndef BWPORT_APP_DEMO3D_DEMO3D_GRAPHIC_H
#define BWPORT_APP_DEMO3D_DEMO3D_GRAPHIC_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/clact.h"

typedef struct Demo3DGraphic Demo3DGraphic;

// pokebw2's Demo3DGraphic_Create
// A demo that draws 3D on both screens has no BGs and no cell actors
Demo3DGraphic *func_ov203_021bddd8(u32 layout, u32 demoId, HeapID heapId);
// pokebw2's Demo3DGraphic_Free
void func_ov203_021bde98(Demo3DGraphic *graphic);
// pokebw2's Demo3DGraphic_Update
void func_ov203_021bdf00(Demo3DGraphic *graphic);

// pokebw2's Demo3DGraphic_SetLightColor
void func_ov203_021bdf74(Demo3DGraphic *graphic, u8 light, GXRgb color);
// pokebw2's Demo3DGraphic_SetLightVector
// Sets a light's direction, which it normalizes
void func_ov203_021bdf88(Demo3DGraphic *graphic, u8 light, const VecFx16 *direction);

// pokebw2's Demo3DGraphic_SetLightColors
// The four lights' colors, as FieldLightStatus_Get gives them
void func_ov203_021bdf3c(Demo3DGraphic *graphic, const GXRgb *colors, u32 unused);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Demo3DGraphic_GetClActUnit
ClActUnit *func_ov203_021bdf30(Demo3DGraphic *graphic);
// pokebw2's Demo3DGraphic_Begin3D
// Around the frame's 3D drawing
void func_ov203_021bdf18(Demo3DGraphic *graphic);
// pokebw2's Demo3DGraphic_End3D
void func_ov203_021bdf24(Demo3DGraphic *graphic);

#endif // BWPORT_APP_DEMO3D_DEMO3D_GRAPHIC_H
