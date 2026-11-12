// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/arrow.h: the lines in use
// so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_ARROW_H
#define BWPORT_APP_RESEARCH_RADAR_ARROW_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The arrow's resources and the animation of each kind of piece
struct ArrowResources {
    u32 chars;
    u32 palette;
    u32 cellAnims;
    u16 surface;
    // A piece of the row, of the column, the corner and the head
    u16 sequences[4];
};

// pokebw2's Arrow_Create
Arrow *func_ov217_021c2070(HeapID heapId, const ArrowResources *resources);
// pokebw2's Arrow_Hide
void func_ov217_021c20d8(Arrow *arrow);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Arrow_Delete
void func_ov217_021c209c(Arrow *arrow);
// pokebw2's Arrow_SetPath
// Lays the arrow out from (startX, startY), left to endX and then down to endY
void func_ov217_021c20b4(Arrow *arrow, int startX, int startY, int endX, int endY);
// pokebw2's Arrow_Update
void func_ov217_021c20c4(Arrow *arrow);
// pokebw2's Arrow_Start
// Shows the pieces one after another
void func_ov217_021c20cc(Arrow *arrow);

#endif // BWPORT_APP_RESEARCH_RADAR_ARROW_H
