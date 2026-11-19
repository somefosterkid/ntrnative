#include "global.h"

#include "save_frontier.h"
#include "unk_02030A98.h"

void sub_020311AC(FrontierSave *frontier, int friendIdx);

void sub_020311AC(FrontierSave *frontier, int friendIdx) {
    ((friendIdx != 0xff) ? (void)0 : GF_AssertFail());

    BattleFrontierStats *stats = &frontier->stats;
    int i;
    for (i = friendIdx; i < 32 - 1; i++) {
        MI_CpuCopy8(stats->wfcStats[i + 1], stats->wfcStats[i], sizeof(u16) * (STATS_NUM_STATS_TOTAL - STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS));

        for (int statIndex = STATS_NUM_SOLO_STATS; statIndex < STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS - 1; statIndex += 2) {
            u16 activeFlag = FrontierSave_GetStat(frontier, statIndex, i + 1);
            sub_02031108(frontier, statIndex, i, activeFlag);
        }
    }

    i = 32 - 1;
    MI_CpuClear8(stats->wfcStats[i], sizeof(u16) * (STATS_NUM_STATS_TOTAL - STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS));
}
