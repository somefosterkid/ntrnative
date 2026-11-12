#ifndef POKEBW2_APP_RESEARCH_RADAR_RESEARCH_TOP_H
#define POKEBW2_APP_RESEARCH_RADAR_RESEARCH_TOP_H

#include "types.h"
#include "struct_decls.h"

// The Research Radar's top screen (research_top.c)
// The names of these functions and types are ours

// Where the top screen goes when it ends
enum {
    RESEARCH_TOP_NEXT_LIST,
    RESEARCH_TOP_NEXT_GRAPH,
    RESEARCH_TOP_NEXT_EXIT,
};

ResearchTop *ResearchTop_Create(ResearchCommon *common);
void ResearchTop_Delete(ResearchTop *top);
void ResearchTop_Main(ResearchTop *top);
BOOL ResearchTop_IsEnd(ResearchTop *top);
u32 ResearchTop_GetNext(ResearchTop *top);

#endif // POKEBW2_APP_RESEARCH_RADAR_RESEARCH_TOP_H
