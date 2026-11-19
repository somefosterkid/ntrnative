#include "global.h"

#include "frontier_data.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_0202D230.h"

struct FrontierSave;
FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);

u16 sub_02067398(SaveData *saveData);

u16 sub_02067398(SaveData *saveData) {
    FrontierSave *frontier = Save_Frontier_GetStatic(saveData);
    u16 frontierStats = FrontierSave_GetStat(frontier, STAT_TOWER_RECORD_STREAK_SINGLE, 0xff);

    if (frontierStats < 20) {
        return 0;
    }

    FrontierData *record = Save_FrontierData_Get(saveData);
    u8 v3 = sub_0202D5DC(record, 13, 0);
    u8 v4 = sub_0202D5DC(record, 0, 0);
    u8 v5 = sub_0202D5DC(record, 1, 0);
    u8 v6 = sub_0202D5DC(record, 14, 0);
    u8 v7 = sub_0202D5DC(record, 2, 0);
    u8 v8 = sub_0202D5DC(record, 3, 0);

    if (v3 && v4 && v5) {
        return 0;
    }

    if (!v3) {
        if (v6) {
            return 4;
        }

        return 1;
    }

    if (frontierStats < 50) {
        return 0;
    }

    if (!v4) {
        if (v7) {
            return 5;
        }

        return 2;
    }

    if (frontierStats < 100) {
        return 0;
    }

    if (v5) {
        return 0;
    }

    if (v8) {
        return 6;
    }

    return 3;
}
