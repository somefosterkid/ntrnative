// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/research_list_recovery.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/research_list_recovery.h"
#include "gfl/heap.h"

struct ResearchListRecovery {
    u8 cursor;
    s32 scrollY;
    s32 bgScroll;
};

// pokebw2's ResearchListRecovery_Create
ResearchListRecovery *func_ov217_021c0dc4(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(ResearchListRecovery), TRUE, "research_list_recovery.c", 32);
}

// pokebw2's ResearchListRecovery_Delete
void func_ov217_021c0ddc(ResearchListRecovery *recovery) {
    GFL_HeapFree(recovery);
}

// pokebw2's ResearchListRecovery_Init
void func_ov217_021c0de4(ResearchListRecovery *recovery) {
    recovery->cursor = 0;
    recovery->scrollY = 0;
    recovery->bgScroll = -24;
}

// pokebw2's ResearchListRecovery_GetCursor
u8 func_ov217_021c0df0(ResearchListRecovery *recovery) {
    return recovery->cursor;
}

// pokebw2's ResearchListRecovery_GetScrollY
s32 func_ov217_021c0df4(ResearchListRecovery *recovery) {
    return recovery->scrollY;
}

// pokebw2's ResearchListRecovery_GetBGScroll
s32 func_ov217_021c0df8(ResearchListRecovery *recovery) {
    return recovery->bgScroll;
}

// pokebw2's ResearchListRecovery_SetCursor
void func_ov217_021c0dfc(ResearchListRecovery *recovery, u8 cursor) {
    recovery->cursor = cursor;
}

// pokebw2's ResearchListRecovery_SetScrollY
void func_ov217_021c0e00(ResearchListRecovery *recovery, s32 scrollY) {
    recovery->scrollY = scrollY;
}

// pokebw2's ResearchListRecovery_SetBGScroll
void func_ov217_021c0e04(ResearchListRecovery *recovery, s32 bgScroll) {
    recovery->bgScroll = bgScroll;
}
