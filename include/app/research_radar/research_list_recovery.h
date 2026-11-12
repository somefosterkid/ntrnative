// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/research_list_recovery.h:
// the lines in use so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_RESEARCH_LIST_RECOVERY_H
#define BWPORT_APP_RESEARCH_RADAR_RESEARCH_LIST_RECOVERY_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's ResearchListRecovery_Create
ResearchListRecovery *func_ov217_021c0dc4(HeapID heapId);
// pokebw2's ResearchListRecovery_Init
void func_ov217_021c0de4(ResearchListRecovery *recovery);
// pokebw2's ResearchListRecovery_SetCursor
void func_ov217_021c0dfc(ResearchListRecovery *recovery, u8 cursor);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ResearchListRecovery_Delete
void func_ov217_021c0ddc(ResearchListRecovery *recovery);
// pokebw2's ResearchListRecovery_GetCursor
// The list's cursor, the y in the list that its view follows, and its BG's scroll
u8 func_ov217_021c0df0(ResearchListRecovery *recovery);
// pokebw2's ResearchListRecovery_GetScrollY
s32 func_ov217_021c0df4(ResearchListRecovery *recovery);
// pokebw2's ResearchListRecovery_GetBGScroll
s32 func_ov217_021c0df8(ResearchListRecovery *recovery);
// pokebw2's ResearchListRecovery_SetScrollY
void func_ov217_021c0e00(ResearchListRecovery *recovery, s32 scrollY);
// pokebw2's ResearchListRecovery_SetBGScroll
void func_ov217_021c0e04(ResearchListRecovery *recovery, s32 bgScroll);

#endif // BWPORT_APP_RESEARCH_RADAR_RESEARCH_LIST_RECOVERY_H
