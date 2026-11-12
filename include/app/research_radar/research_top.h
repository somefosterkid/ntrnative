// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/research_top.h: the lines
// in use so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_RESEARCH_TOP_H
#define BWPORT_APP_RESEARCH_RADAR_RESEARCH_TOP_H

#include "types.h"
#include "struct_decls.h"

// Where the top screen goes when it ends
enum {
    RESEARCH_TOP_NEXT_LIST,
    RESEARCH_TOP_NEXT_GRAPH,
    RESEARCH_TOP_NEXT_EXIT,
};

// pokebw2's ResearchTop_Main
void func_ov217_021b9b08(ResearchTop *top);
// pokebw2's ResearchTop_IsEnd
BOOL func_ov217_021b9b80(ResearchTop *top);
// pokebw2's ResearchTop_GetNext
u32 func_ov217_021b9b90(ResearchTop *top);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ResearchTop_Delete
void func_ov217_021b9af8(ResearchTop *top);
// pokebw2's ResearchTop_Create
ResearchTop *func_ov217_021b9ad0(ResearchCommon *common);

#endif // BWPORT_APP_RESEARCH_RADAR_RESEARCH_TOP_H
