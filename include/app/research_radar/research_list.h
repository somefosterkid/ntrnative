// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/research_list.h: the
// lines in use so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_RESEARCH_LIST_H
#define BWPORT_APP_RESEARCH_RADAR_RESEARCH_LIST_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the values of RESEARCH_LIST_NEXT_TOP in Black
// Where the list goes when it ends
enum {
    RESEARCH_LIST_NEXT_TOP,
    RESEARCH_LIST_NEXT_EXIT,
};

// pokebw2's ResearchList_Create
ResearchList *func_ov217_021bab80(ResearchCommon *common, ResearchListRecovery *recovery);
// pokebw2's ResearchList_Main
void func_ov217_021babc4(ResearchList *list);
// pokebw2's ResearchList_GetNext
u32 func_ov217_021bac98(ResearchList *list);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ResearchList_Delete
void func_ov217_021babb4(ResearchList *list);
// pokebw2's ResearchList_IsEnd
BOOL func_ov217_021bac90(ResearchList *list);

#endif // BWPORT_APP_RESEARCH_RADAR_RESEARCH_LIST_H
