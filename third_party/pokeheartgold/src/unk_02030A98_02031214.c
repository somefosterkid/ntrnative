#include "global.h"

#include "save_frontier.h"
#include "unk_02030A98.h"

void sub_02031214(FrontierSave *frontier, int friendIndex, int unused);

void sub_02031214(FrontierSave *frontier, int friendIndex, int unused) {
    BattleFrontierStats *stats = &frontier->stats;
    MI_CpuClear8(stats->wfcStats[friendIndex], sizeof(u16) * (STATS_NUM_STATS_TOTAL - STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS));
}
