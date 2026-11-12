// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/research_graph.h: the
// lines in use so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_RESEARCH_GRAPH_H
#define BWPORT_APP_RESEARCH_RADAR_RESEARCH_GRAPH_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the values of RESEARCH_GRAPH_NEXT_TOP in Black
// Where the graph goes when it ends
enum {
    RESEARCH_GRAPH_NEXT_TOP,
    RESEARCH_GRAPH_NEXT_EXIT,
};

// pokebw2's ResearchGraph_Create
ResearchGraph *func_ov217_021bd5f4(ResearchCommon *common);
// pokebw2's ResearchGraph_Main
void func_ov217_021bd62c(ResearchGraph *graph);
// pokebw2's ResearchGraph_IsEnd
BOOL func_ov217_021bd708(ResearchGraph *graph);
// pokebw2's ResearchGraph_GetNext
u32 func_ov217_021bd710(ResearchGraph *graph);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ResearchGraph_Delete
void func_ov217_021bd61c(ResearchGraph *graph);

// Not referenced
extern const u32 ResearchGraph_Unused2[1];

// pokebw2's: MWCC puts the constant above in the same section as the graph's other data, as Black has it, only when
// some code reads it before its definition, even code it never emits. This accessor is that code. Nothing calls it.
static inline u32 ResearchGraph_GetUnused2(int index) {
    return ResearchGraph_Unused2[index];
}

#endif // BWPORT_APP_RESEARCH_RADAR_RESEARCH_GRAPH_H
