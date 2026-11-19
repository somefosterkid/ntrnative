#include "global.h"

#include "sys_task.h"
#include "error_handling.h"
#include "heap.h"
#include "player_data.h"
#include "overlay_99_pokeathlon_course_summary.h"

void ov99_021E7060(PokeathlonCourseSummary *summary);
u8 ov99_021E7068(const PokeathlonCourseSummary *summary);
u8 ov99_021E7078(const PokeathlonCourseSummary *summary);
u8 ov99_021E7088(const PokeathlonCourseSummary *summary);
PlayerProfile *ov99_021E7098(const PokeathlonCourseSummary *summary);
u16 ov99_021E70A8(const PokeathlonCourseSummary *summary);
u16 ov99_021E70B8(const PokeathlonCourseSummary *summary);
u16 ov99_021E70C8(const PokeathlonCourseSummary *summary);
u16 ov99_021E70D8(const PokeathlonCourseSummary *summary);
u16 ov99_021E70E8(const PokeathlonCourseSummary *summary, s32 idx);

void ov99_021E7060(PokeathlonCourseSummary *summary) {
    Heap_Free(summary);
}

u8 ov99_021E7068(const PokeathlonCourseSummary *summary) {
    if (summary == NULL) {
        GF_AssertFail();
    }
    return summary->unk_17;
}

u8 ov99_021E7078(const PokeathlonCourseSummary *summary) {
    if (summary == NULL) {
        GF_AssertFail();
    }
    return summary->unk_16;
}

u8 ov99_021E7088(const PokeathlonCourseSummary *summary) {
    if (summary == NULL) {
        GF_AssertFail();
    }
    return summary->unk_18;
}

PlayerProfile *ov99_021E7098(const PokeathlonCourseSummary *summary) {
    if (summary == NULL) {
        GF_AssertFail();
    }
    return summary->profile;
}

u16 ov99_021E70A8(const PokeathlonCourseSummary *summary) {
    if (summary == NULL) {
        GF_AssertFail();
    }
    return summary->unk_0A;
}

u16 ov99_021E70B8(const PokeathlonCourseSummary *summary) {
    if (summary == NULL) {
        GF_AssertFail();
    }
    return summary->unk_06;
}

u16 ov99_021E70C8(const PokeathlonCourseSummary *summary) {
    if (summary == NULL) {
        GF_AssertFail();
    }
    return summary->unk_04;
}

u16 ov99_021E70D8(const PokeathlonCourseSummary *summary) {
    if (summary == NULL) {
        GF_AssertFail();
    }
    return summary->unk_08;
}

u16 ov99_021E70E8(const PokeathlonCourseSummary *summary, s32 idx) {
    if (summary == NULL) {
        GF_AssertFail();
    }
    return summary->unk_0C[idx];
}
