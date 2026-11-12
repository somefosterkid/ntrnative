// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mystery/mystery_graphic.h: the lines in
// use so far.
#ifndef BWPORT_APP_MYSTERY_MYSTERY_GRAPHIC_H
#define BWPORT_APP_MYSTERY_MYSTERY_GRAPHIC_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's MysteryGraphic_Create
MysteryGraphic *func_ov121_021d9624(u32 mode, HeapID heapId);
// pokebw2's MysteryGraphic_Delete
void func_ov121_021d96c4(MysteryGraphic *graphic);
// pokebw2's MysteryGraphic_Start3D
// Switch the main screen to 3D for the effect of a received gift, and back
void func_ov121_021d9750(MysteryGraphic *graphic);
// pokebw2's MysteryGraphic_End3D
void func_ov121_021d9790(MysteryGraphic *graphic);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MysteryGraphic_Update
void func_ov121_021d9718(MysteryGraphic *graphic);
// pokebw2's MysteryGraphic_BeginFrame3D
void func_ov121_021d972c(MysteryGraphic *graphic);
// pokebw2's MysteryGraphic_EndFrame3D
void func_ov121_021d9738(MysteryGraphic *graphic);
// pokebw2's MysteryGraphic_GetClactUnit
ClActUnit *func_ov121_021d9744(MysteryGraphic *graphic);

#endif // BWPORT_APP_MYSTERY_MYSTERY_GRAPHIC_H
