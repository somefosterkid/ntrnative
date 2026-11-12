// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/circle_graph.h: the lines
// in use so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_CIRCLE_GRAPH_H
#define BWPORT_APP_RESEARCH_RADAR_CIRCLE_GRAPH_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// An answer of the graph: its ID, its count, and the RGB components, from 0 to 31, of its slice's rim and centre
struct CircleGraphData {
    u8 id;
    u32 count;
    u8 color[3];
    u8 centerColor[3];
};

// pokebw2's CircleGraph_Create
CircleGraph *func_ov217_021c10c8(HeapID heapId);
// pokebw2's CircleGraph_SetData
void func_ov217_021c10e8(CircleGraph *graph, const CircleGraphData *data, int num);
// pokebw2's CircleGraph_Draw
void func_ov217_021c112c(CircleGraph *graph);
// pokebw2's CircleGraph_RequestGrow
void func_ov217_021c1178(CircleGraph *graph);
// pokebw2's CircleGraph_SetDepth
void func_ov217_021c11a4(CircleGraph *graph, fx16 z);
// pokebw2's CircleGraph_GetItemId
u8 func_ov217_021c11d0(CircleGraph *graph, u8 index);
// pokebw2's CircleGraph_GetLabelScreenPosById
void func_ov217_021c11fc(CircleGraph *graph, u8 id, int *x, int *y);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CircleGraph_Delete
void func_ov217_021c10d8(CircleGraph *graph);
// pokebw2's CircleGraph_Main
void func_ov217_021c1124(CircleGraph *graph);
// pokebw2's CircleGraph_RequestGrowSlow
void func_ov217_021c1154(CircleGraph *graph);
// pokebw2's CircleGraph_RequestGrowFast
void func_ov217_021c1160(CircleGraph *graph);
// pokebw2's CircleGraph_RequestShrink
void func_ov217_021c116c(CircleGraph *graph);
// pokebw2's CircleGraph_SetVisible
void func_ov217_021c1184(CircleGraph *graph, BOOL visible);
// pokebw2's CircleGraph_SetWait
void func_ov217_021c118c(CircleGraph *graph, u32 frames);
// pokebw2's CircleGraph_GetItemIndex
u8 func_ov217_021c11c8(CircleGraph *graph, u8 id);
// pokebw2's CircleGraph_GetPercentById
u8 func_ov217_021c11dc(CircleGraph *graph, u8 id);
// pokebw2's CircleGraph_GetPercent
u8 func_ov217_021c11e8(CircleGraph *graph, u8 index);
// pokebw2's CircleGraph_IsMoving
BOOL func_ov217_021c11f4(CircleGraph *graph);
// pokebw2's CircleGraph_GetLabelScreenPos
void func_ov217_021c1224(CircleGraph *graph, u8 index, int *x, int *y);

#endif // BWPORT_APP_RESEARCH_RADAR_CIRCLE_GRAPH_H
