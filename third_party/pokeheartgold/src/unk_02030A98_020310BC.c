#include "global.h"
#include "overlay_80_frontier_opponent.h"
#include "battle_castle_save.h"
#include "battle_frontier_stats.h"
#include "mail_message_types.h"
#include "save_frontier.h"

enum PokemonStat {
    STAT_HP = 0,
    STAT_ATTACK = 1,
    STAT_DEFENSE = 2,
    STAT_SPEED = 3,
    STAT_SPECIAL_ATTACK = 4,
    STAT_SPECIAL_DEFENSE = 5,
    STAT_MAX = 6,
};
u16 sub_02031108(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);
u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);

u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);
u16 sub_02031108(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);
void sub_02031188(FrontierSave *frontier);

u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID) {
    BattleFrontierStats *stats = &frontier->stats;

    if (statIndex < STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS) {
        if (statIndex >= STATS_NUM_SOLO_STATS) {
            int flagGroup, bit;

            if (hostFriendID < 16) {
                flagGroup = statIndex;
                bit = hostFriendID;
            } else {
                flagGroup = statIndex + 1;
                bit = hostFriendID - 16;
            }

            return (stats->soloStats[flagGroup] >> bit) & 1;
        }

        return stats->soloStats[statIndex];
    } else {
        if (hostFriendID == -1) {
            ((0) ? (void)0 : GF_AssertFail());
            return 0;
        }

        return stats->wfcStats[hostFriendID][statIndex - STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS];
    }
}

u16 sub_02031108(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue) {
    BattleFrontierStats *stats = &frontier->stats;

    if (newValue > 9999) {
        newValue = 9999;
    }

    if (statIndex < STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS) {
        if (statIndex >= STATS_NUM_SOLO_STATS) {
            int flagGroup, bit;

            ((hostFriendID != 0xff) ? (void)0 : GF_AssertFail());

            if (hostFriendID < 16) {
                flagGroup = statIndex;
                bit = hostFriendID;
            } else {
                flagGroup = statIndex + 1;
                bit = hostFriendID - 16;
            }

            if (newValue == 0) {
                stats->soloStats[flagGroup] &= 0xffff ^ (1 << bit);
            } else {
                stats->soloStats[flagGroup] |= 1 << bit;
            }
        } else {
            ((hostFriendID == 0xff) ? (void)0 : GF_AssertFail());
            stats->soloStats[statIndex] = newValue;
        }
    } else {
        ((hostFriendID != 0xff) ? (void)0 : GF_AssertFail());
        stats->wfcStats[hostFriendID][statIndex - STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS] = newValue;
    }

    return newValue;
}

void sub_02031188(FrontierSave *frontier) {
    BattleFrontierStats *stats = &frontier->stats;

    MI_CpuClear8(stats->wfcStats, sizeof(u16) * (STATS_NUM_STATS_TOTAL - STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS) * 32);

    for (int i = STATS_NUM_SOLO_STATS; i <= STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS - 1; i++) {
        stats->soloStats[i] = 0;
    }
}
