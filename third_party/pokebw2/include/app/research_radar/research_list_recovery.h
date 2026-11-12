#ifndef POKEBW2_APP_RESEARCH_RADAR_RESEARCH_LIST_RECOVERY_H
#define POKEBW2_APP_RESEARCH_RADAR_RESEARCH_LIST_RECOVERY_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// What the list of surveys keeps while another screen is shown, to come back to where it was
// (research_list_recovery.c)
// The names of these functions and types are ours

ResearchListRecovery *ResearchListRecovery_Create(HeapID heapId);
void ResearchListRecovery_Delete(ResearchListRecovery *recovery);
void ResearchListRecovery_Init(ResearchListRecovery *recovery);
// The list's cursor, the y in the list that its view follows, and its BG's scroll
u8 ResearchListRecovery_GetCursor(ResearchListRecovery *recovery);
s32 ResearchListRecovery_GetScrollY(ResearchListRecovery *recovery);
s32 ResearchListRecovery_GetBGScroll(ResearchListRecovery *recovery);
void ResearchListRecovery_SetCursor(ResearchListRecovery *recovery, u8 cursor);
void ResearchListRecovery_SetScrollY(ResearchListRecovery *recovery, s32 scrollY);
void ResearchListRecovery_SetBGScroll(ResearchListRecovery *recovery, s32 bgScroll);

#endif // POKEBW2_APP_RESEARCH_RADAR_RESEARCH_LIST_RECOVERY_H
