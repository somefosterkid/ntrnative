#ifndef POKEBW2_APP_RESEARCH_RADAR_ARROW_H
#define POKEBW2_APP_RESEARCH_RADAR_ARROW_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// An arrow drawn piece by piece with cell actors, left from a start along a row and then down a column to its end,
// which the Research Radar's graph uses to point from a graph to its answers (arrow.c)
// The names of these functions and types are ours

// The arrow's resources and the animation of each kind of piece
typedef struct {
    u32 chars;
    u32 palette;
    u32 cellAnims;
    u16 surface;
    // A piece of the row, of the column, the corner and the head
    u16 sequences[4];
} ArrowResources;

Arrow *Arrow_Create(HeapID heapId, const ArrowResources *resources);
void Arrow_Delete(Arrow *arrow);
// Lays the arrow out from (startX, startY), left to endX and then down to endY
void Arrow_SetPath(Arrow *arrow, int startX, int startY, int endX, int endY);
void Arrow_Update(Arrow *arrow);
// Shows the pieces one after another
void Arrow_Start(Arrow *arrow);
void Arrow_Hide(Arrow *arrow);

#endif // POKEBW2_APP_RESEARCH_RADAR_ARROW_H
