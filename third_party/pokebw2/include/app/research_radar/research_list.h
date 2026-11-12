#ifndef POKEBW2_APP_RESEARCH_RADAR_RESEARCH_LIST_H
#define POKEBW2_APP_RESEARCH_RADAR_RESEARCH_LIST_H

#include "types.h"
#include "struct_decls.h"

// The Research Radar's list of surveys (research_list.c)
// The names of these functions and types are ours

// Where the list goes when it ends
enum {
    RESEARCH_LIST_NEXT_TOP,
    RESEARCH_LIST_NEXT_EXIT,
};

ResearchList *ResearchList_Create(ResearchCommon *common, ResearchListRecovery *recovery);
void ResearchList_Delete(ResearchList *list);
void ResearchList_Main(ResearchList *list);
BOOL ResearchList_IsEnd(ResearchList *list);
u32 ResearchList_GetNext(ResearchList *list);

#endif // POKEBW2_APP_RESEARCH_RADAR_RESEARCH_LIST_H
