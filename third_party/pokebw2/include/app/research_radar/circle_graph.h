#ifndef POKEBW2_APP_RESEARCH_RADAR_CIRCLE_GRAPH_H
#define POKEBW2_APP_RESEARCH_RADAR_CIRCLE_GRAPH_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx.h"
#include "struct_decls.h"

// The Research Radar's circle graph of a survey's answers, a pie of up to 20 slices drawn with the geometry engine
// (circle_graph.c). Each slice is a share of the total count, in whole percents, drawn from a rim colour to a centre
// colour, and the graph grows into a full circle and shrinks away by the animations queued on it
// The names of these functions and types are ours

// An answer of the graph: its ID, its count, and the RGB components, from 0 to 31, of its slice's rim and centre
typedef struct {
    u8 id;
    u32 count;
    u8 color[3];
    u8 centerColor[3];
} CircleGraphData;

CircleGraph *CircleGraph_Create(HeapID heapId);
void CircleGraph_Delete(CircleGraph *graph);
void CircleGraph_SetData(CircleGraph *graph, const CircleGraphData *data, int num);
void CircleGraph_Main(CircleGraph *graph);
void CircleGraph_Draw(CircleGraph *graph);
void CircleGraph_RequestGrowSlow(CircleGraph *graph);
void CircleGraph_RequestGrowFast(CircleGraph *graph);
void CircleGraph_RequestShrink(CircleGraph *graph);
void CircleGraph_RequestGrow(CircleGraph *graph);
void CircleGraph_SetVisible(CircleGraph *graph, BOOL visible);
void CircleGraph_SetWait(CircleGraph *graph, u32 frames);
void CircleGraph_SetDepth(CircleGraph *graph, fx16 z);
u8 CircleGraph_GetItemIndex(CircleGraph *graph, u8 id);
u8 CircleGraph_GetItemId(CircleGraph *graph, u8 index);
u8 CircleGraph_GetPercentById(CircleGraph *graph, u8 id);
u8 CircleGraph_GetPercent(CircleGraph *graph, u8 index);
BOOL CircleGraph_IsMoving(CircleGraph *graph);
void CircleGraph_GetLabelScreenPosById(CircleGraph *graph, u8 id, int *x, int *y);
void CircleGraph_GetLabelScreenPos(CircleGraph *graph, u8 index, int *x, int *y);

#endif // POKEBW2_APP_RESEARCH_RADAR_CIRCLE_GRAPH_H
